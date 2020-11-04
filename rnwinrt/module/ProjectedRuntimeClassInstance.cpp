#include "pch.h"
#include "ProjectedRuntimeClassInstance.h"
#include "ProjectedValueConverters.h"
#include "ProjectionsContext.h"

namespace WinRTTurboModule
{
    jsi::Value ProjectedRuntimeClassInstance::Get(const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IInspectable& instance)
    {
        return context->InstanceFactory.Get(instance);
    }

    ProjectedRuntimeClassInstance::ProjectedRuntimeClassInstance(const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IInspectable& instance)
        : m_context(context), m_instance(instance)
    {
    }

    ProjectedRuntimeClassInstance::~ProjectedRuntimeClassInstance()
    {
        m_context->InstanceFactory.Remove(m_instance);
    }

    jsi::Value ProjectedRuntimeClassInstance::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        EnsureInitialized();

        auto propNameUtf8 = propName.utf8(runtime);

        for (uint32_t i = 0; i < m_interfaceInstances.size(); ++i)
        {
            const auto& interfaceInstance = m_interfaceInstances[i];
            auto& interfaceDef = interfaceInstance->Interface();

            if (const auto property = interfaceDef.FindProperty(propNameUtf8))
            {
                return interfaceInstance->GetProperty(runtime, property);
            }

            // PERF: Would it make sense to cache unique and composite methods after resolution?
            if (auto method = interfaceDef.FindMethod(propNameUtf8))
            {
                // At least one interface has a method with the specified name, but it is possible that an overload with the same
                // name was added in a later instance of the interface so we need to multiplex them as a single function.
                std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>, std::shared_ptr<ProjectedFunction>>> interfaceMethods;
                for (++i ; i < m_interfaceInstances.size(); ++i)
                {
                    const auto& overloadInterfaceInstance = m_interfaceInstances[i];
                    if (auto overloadMethod = overloadInterfaceInstance->Interface().FindMethod(propNameUtf8))
                    {
                        interfaceMethods.emplace_back(overloadInterfaceInstance, std::move(overloadMethod));
                    }
                }

                if (interfaceMethods.empty())
                {
                    return interfaceInstance->GetMethod(runtime, method);
                }

                interfaceMethods.emplace_back(interfaceInstance, std::move(method));
                return jsi::Value(runtime, CreateCompositeMethod(runtime, std::move(propNameUtf8), std::move(interfaceMethods)));
            }
        }

        // TODO: Is it necessary to support Chakra's treatment of events like properties in addition to add/removeEventListener. It
        //       seems like a poor pattern to carry forward given that it only allows for a single handler for an event.

        const bool isAddEventListener = (propNameUtf8 == "addEventListener"sv);
        if (isAddEventListener || (propNameUtf8 == "removeEventListener"sv))
        {
            if (auto function = GetEventListenerFunction(runtime, isAddEventListener))
            {
                return std::move(*function);
            }
        }

        return jsi::Value::undefined();
    }

    void ProjectedRuntimeClassInstance::set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value)
    {
        EnsureInitialized();

        const auto propNameUtf8 = propName.utf8(runtime);

        for (const auto& interfaceInstance : m_interfaceInstances)
        {
            if (const auto property = interfaceInstance->Interface().FindProperty(propNameUtf8); property && !property->IsReadonly())
            {
                interfaceInstance->SetProperty(runtime, property, value);
                break;
            }
        }

        // If no property exists with the given name, then ignore the call rather than throwing. This is more-or-less consistent with EdgeHTML WebView.
    }

    std::vector<jsi::PropNameID> ProjectedRuntimeClassInstance::getPropertyNames(jsi::Runtime& runtime)
    {
        EnsureInitialized();

        std::vector<jsi::PropNameID> propNames;
        bool hasEvents = false;
        for (const auto& interfaceInstance : m_interfaceInstances)
        {
            auto& interfaceDef = interfaceInstance->Interface();
            const auto& methods = interfaceDef.Methods();
            const auto& properties = interfaceDef.Properties();

            propNames.reserve(propNames.size() + methods.size() + properties.size());

            // TODO: Do we care if there are duplicates for methods due to overloading interfaces?
            for (const auto& method : methods)
            {
                propNames.push_back(CreatePropNameID(runtime, method.first));
            }

            for (const auto& property : properties)
            {
                propNames.push_back(CreatePropNameID(runtime, property.first));
            }

            if (!hasEvents && !interfaceDef.Events().empty())
            {
                hasEvents = true;
            }
        }

        if (hasEvents)
        {
            propNames.reserve(propNames.size() + 2);
            propNames.push_back(CreatePropNameID(runtime, "addEventListener"sv));
            propNames.push_back(CreatePropNameID(runtime, "removeEventListener"sv));
        }

        return propNames;
    }

    std::optional<jsi::Value> ProjectedRuntimeClassInstance::GetEventListenerFunction(jsi::Runtime& runtime, bool isAddEventListener)
    {
        std::vector<std::shared_ptr<IProjectedInterfaceInstance>> interfaceInstancesWithEvents;
        interfaceInstancesWithEvents.reserve(m_interfaceInstances.size());
        for (const auto& interfaceInstance : m_interfaceInstances)
        {
            if (!interfaceInstance->Interface().Events().empty())
            {
                interfaceInstancesWithEvents.push_back(interfaceInstance);
            }
        }

        if (!interfaceInstancesWithEvents.empty())
        {
            // PERF: Would it make sense to cache event listener functions?
            return jsi::Value(runtime, jsi::Function::createFromHostFunction(runtime, CreatePropNameID(runtime, isAddEventListener ? "addEventListener"sv : "removeEventListener"sv), 2 /*length*/,
                [isAddEventListener, interfaceInstances{std::move(interfaceInstancesWithEvents)}](jsi::Runtime& runtime, const jsi::Value& thisVal, const jsi::Value* args, size_t numArgs)
            {
                if (numArgs < 2)
                {
                    throw jsi::JSError(runtime, "TypeError: add/removeEventListener expect (at least) 2 arguments");
                }

                const auto eventName = args[0].asString(runtime).utf8(runtime);

                for (auto& interfaceInstance : interfaceInstances)
                {
                    if (const auto event = interfaceInstance->Interface().FindEvent(eventName))
                    {
                        if (isAddEventListener)
                        {
                            interfaceInstance->AddEventListener(runtime, event, args[1]);
                        }
                        else
                        {
                            interfaceInstance->RemoveEventListener(runtime, event, args[1]);
                        }
                    }
                }

                return jsi::Value::undefined();
            }));
        }

        return {};
    }

    void ProjectedRuntimeClassInstance::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            const auto iids = winrt::get_interfaces(m_instance);
            m_interfaceInstances.reserve(iids.size());

            for (const auto& iid : iids)
            {
                if (const auto projectedInterface = ProjectedInterface::Get(iid))
                {
                    m_interfaceInstances.push_back(projectedInterface->CreateInstance(m_context, m_instance));
                }
            }

            m_isInitialized = true;
        }
    }

    jsi::Function CreateCompositeConstructor(jsi::Runtime& runtime, std::string className, std::vector<std::shared_ptr<IProjectedInterfaceInstance>> factoryInterfaceInstances)
    {
        // TODO: Does it matter that the function name is arbitrarily set to the runtimeclass name or that the function length
        //       is set to 0. The name is probably better as the class name than the ABI factory method name which could vary, and
        //       length is possible to compute as the maximum of all "overloads" but it doesn't seem like it would be worthwhile.
        return jsi::Function::createFromHostFunction(runtime, CreatePropNameID(runtime, className), 0 /*length*/,
            [className{ std::move(className) }, factoryInterfaceInstances{ std::move(factoryInterfaceInstances) }](jsi::Runtime& runtime, const jsi::Value& thisVal, const jsi::Value* args, size_t numArgs)
        {
            // C++/WinRT generates factory interfaces so the constructor overloads actually have different names rather than
            // relying on arity to differentiate them. Conveivably, there might be multiple factory interfaces and/or the special
            // ClassicCOM IActivationFactory for the zero-param case. A poorly written API might also have multiple overloads with
            // the same arity as it didn't consider JS. Thus we use TryInvokeAnyMethod rather than TryInvokeMethod.

            for (auto& factoryInterfaceInstance : factoryInterfaceInstances)
            {
                if (const auto method = factoryInterfaceInstance->Interface().FindMethod(static_cast<uint16_t>(numArgs)))
                {
                    return factoryInterfaceInstance->InvokeMethod(runtime, method, args, static_cast<uint16_t>(numArgs));
                }
            }

            throw jsi::JSError(runtime, "TypeError: No constructor overload exists for " + className + " with " + std::to_string(numArgs) + " args");
        });
    }

    jsi::Value CreateCompositeMethod(jsi::Runtime& runtime, std::string methodName, std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>, std::shared_ptr<ProjectedFunction>>> interfaceMethods)
    {
        if (interfaceMethods.size() == 1)
        {
            const auto& interfaceMethod = interfaceMethods[0];
            return interfaceMethod.first->GetMethod(runtime, interfaceMethod.second);
        }

        // TODO: Does it matter if we don't bother determining the maximum arity for the sake of the length param on the function?
        return jsi::Value(runtime, jsi::Function::createFromHostFunction(runtime, CreatePropNameID(runtime, methodName), 0 /*length*/,
            [methodName{ std::move(methodName) }, interfaceMethods{std::move(interfaceMethods)}](jsi::Runtime& runtime, const jsi::Value& thisVal, const jsi::Value* args, size_t numArgs)
        {
            for (auto& interfaceMethod : interfaceMethods)
            {
                if (interfaceMethod.second->CanInvoke(static_cast<uint16_t>(numArgs)))
                {
                    return interfaceMethod.first->InvokeMethod(runtime, interfaceMethod.second, args, static_cast<uint16_t>(numArgs));
                }
            }

            throw jsi::JSError(runtime, "TypeError: No function overload exists for " + methodName + " with " + std::to_string(numArgs) + " args");
        }));
    }

}