#include "pch.h"

#include "ProjectedActivationFactory.h"
#include "ProjectedFunction.h"
#include "ProjectedProperty.h"
#include "ProjectedValueConverters.h"

namespace WinRTTurboModule
{
    // Special helper for use with Object.defineProperty
    class ProjectedPropertyDefinition final : public jsi::HostObject
    {
    public:
        ProjectedPropertyDefinition(const std::shared_ptr<IProjectedInterfaceInstance>& interfaceInstance,
            const std::shared_ptr<IProjectedPropertyBase>& property) :
            m_interfaceInstance(interfaceInstance),
            m_property(property)
        {
        }

        virtual ~ProjectedPropertyDefinition() = default;

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override
        {
            const auto propNameUtf8 = propName.utf8(runtime);

            if (!propNameUtf8.compare("get"))
            {
                if (!m_get)
                {
                    m_get = jsi::Function::createFromHostFunction(runtime, propName, 0,
                        [interfaceInstance{ m_interfaceInstance }, property{ m_property }](jsi::Runtime& runtime,
                            const jsi::Value& thisVal, const jsi::Value* args,
                            size_t count) mutable { return interfaceInstance->GetProperty(runtime, property); });
                }
                return jsi::Value(runtime, m_get.value());
            }

            if (!m_property->IsReadonly() && !propNameUtf8.compare("set"))
            {
                if (!m_set)
                {
                    m_set = jsi::Function::createFromHostFunction(runtime, propName, 1,
                        [interfaceInstance{ m_interfaceInstance }, property{ m_property }](jsi::Runtime& runtime,
                            const jsi::Value& thisVal, const jsi::Value* args, size_t count) mutable {
                            if (count != 1)
                            {
                                throw jsi::JSError(runtime, "TypeError: Property setter expects 1 argument");
                            }
                            interfaceInstance->SetProperty(runtime, property, args[0]);
                            return jsi::Value::undefined();
                        });
                }
                return jsi::Value(runtime, m_set.value());
            }

            return jsi::Value::undefined();
        }

        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value) override
        {
            throw jsi::JSError(runtime, std::string("TypeError: Cannot assign to property '") + propName.utf8(runtime) +
                                            "' to a projected property definition");
        }

        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override
        {
            std::vector<jsi::PropNameID> propNames;
            propNames.push_back(jsi::PropNameID::forAscii(runtime, "get"));

            if (!m_property->IsReadonly())
            {
                propNames.push_back(jsi::PropNameID::forAscii(runtime, "set"));
            }

            return propNames;
        }

    private:
        const std::shared_ptr<IProjectedInterfaceInstance> m_interfaceInstance;
        const std::shared_ptr<IProjectedPropertyBase> m_property;
        std::optional<jsi::Function> m_get;
        std::optional<jsi::Function> m_set;
    };

    ProjectedActivationFactory::ProjectedActivationFactory(const std::string_view& name,
        const std::shared_ptr<ProjectionsContext>& context, GetActivationFactory getActivationFactory,
        std::set<winrt::guid>&& factoryInterfaceIds) :
        m_name(name),
        m_context(context), m_getActivationFactory(getActivationFactory),
        m_factoryInterfaceIds(std::forward<std::set<winrt::guid>>(factoryInterfaceIds))
    {
    }

    std::string_view ProjectedActivationFactory::Name() const
    {
        return m_name;
    }

    jsi::Value ProjectedActivationFactory::Value(jsi::Runtime& runtime)
    {
        if (!m_object)
        {
            // Create it directly rather than using "ProjectedRuntimeClassInstance::Get" as the factories/statics do not
            // need to be added to the lookup map given that they are strongly referenced singletons like namespaces.
            auto projectedRuntimeClassInstance =
                std::make_shared<ProjectedRuntimeClassInstance>(m_context, m_getActivationFactory());

            if (m_factoryInterfaceIds.empty())
            {
                // PERF 1: We could theoretically defer construction of the factory until calls to
                //         ProjectedRuntimeClassInstance require it, but for simplicity we can do so now. It migjt
                //         theoretically be possible to also prepopulate IIDs to save on calling GetIIDs if we also had
                //         code generation list the statics.

                // PERF 2: We are intentionally holding a strong ref to it rather than using the caching mechanism used
                //         with instances (see Get(instance)) given that this object is a factory. Arguably weak-ref
                //         based caching would be more memory efficient as the JS GC free references to HostObjects, so
                //         we could switch if memory is more important than CPU usage.

                m_object = jsi::Object::createFromHostObject(runtime, std::move(projectedRuntimeClassInstance));
            }
            else
            {
                // It's not possible with JSI to construct as a function object from both a native function and a
                // HostObject. WinRT runtimeclasses support both constructors and static methods and we need the object
                // to be a function so that it can be called like a constructor, but the static methods need to be
                // treated as properties on the function. Since we can use IHostObject::get for this purpose, they need
                // to be added onto the JSI object itself.
                const auto& interfaceInstances = projectedRuntimeClassInstance->Interfaces();

                std::vector<std::shared_ptr<IProjectedInterfaceInstance>> factoryInterfaceInstances;
                std::vector<std::shared_ptr<IProjectedInterfaceInstance>> staticInterfaceInstances;
                for (const auto& interfaceInstance : interfaceInstances)
                {
                    if (m_factoryInterfaceIds.find(interfaceInstance->Interface().InterfaceId()) ==
                        m_factoryInterfaceIds.end())
                    {
                        staticInterfaceInstances.push_back(interfaceInstance);
                    }
                    else
                    {
                        factoryInterfaceInstances.push_back(interfaceInstance);
                    }
                }

                // Create a function that multiplexes all methods on all factory interfaces as C++/WinRT generates
                // interfaces for factories using the ABI names rather than overloading "CreateInstance".
                m_object =
                    CreateCompositeConstructor(runtime, std::string(m_name), std::move(factoryInterfaceInstances));

                // Exclude "factoryInterfaceInstances" from being injected as properties as there is no point in adding
                // the constructors again using their ABI names given that we are doing all of this to support
                // constructors!
                const auto objectClass = runtime.global().getPropertyAsFunction(runtime, "Object");
                std::optional<jsi::Function> defineProperty;

                std::map<std::string_view, std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>,
                                               std::shared_ptr<ProjectedFunction>>>>
                    methodMap;
                for (auto& staticInterfaceInstance : staticInterfaceInstances)
                {
                    if (const auto& properties = staticInterfaceInstance->Interface().Properties(); !properties.empty())
                    {
                        if (!defineProperty)
                        {
                            defineProperty = objectClass.getPropertyAsFunction(runtime, "defineProperty");
                        }

                        for (auto&& property : properties)
                        {
                            defineProperty->call(runtime, *m_object, CreateString(runtime, property.first),
                                jsi::Value(runtime, jsi::Object::createFromHostObject(
                                                        runtime, std::make_shared<ProjectedPropertyDefinition>(
                                                                     staticInterfaceInstance, property.second))));
                        }
                    }

                    // It is possible that multiple interfaces implement the same method rather than just having
                    // overloads for that method on the same interface. So unlike properties, we need to multiplex the
                    // methods to ensure we call the right overload.
                    for (const auto& method : staticInterfaceInstance->Interface().Methods())
                    {
                        methodMap[method.first].emplace_back(staticInterfaceInstance, method.second);
                    }
                }

                for (auto&& method : methodMap)
                {
                    m_object->setProperty(runtime, CreateString(runtime, method.first),
                        CreateCompositeMethod(runtime, std::string(method.first), std::move(method.second)));
                }

                if (auto addEventListener =
                        projectedRuntimeClassInstance->GetEventListenerFunction(runtime, true /*isAddEventListener*/))
                {
                    m_object->setProperty(runtime, "addEventListener", std::move(*addEventListener));

                    auto removeEventListener =
                        projectedRuntimeClassInstance->GetEventListenerFunction(runtime, false /*isAddEventListener*/);
                    m_object->setProperty(runtime, "removeEventListener", std::move(*removeEventListener));
                }

                objectClass.getPropertyAsFunction(runtime, "seal").call(runtime, *m_object);
            }
        }
        return jsi::Value(runtime, *m_object);
    }
}
