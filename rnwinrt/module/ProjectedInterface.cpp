#include "pch.h"

#include "ProjectedInterface.h"
#include "ProjectedValueConverters.h"
#include <rnwinrt/Projections.g.h>

namespace WinRTTurboModule
{
    std::unordered_map<winrt::guid, std::shared_ptr<ProjectedInterface>> ProjectedInterface::s_interfaceMap;

    std::shared_ptr<ProjectedInterface> ProjectedInterface::Get(const winrt::guid& iid)
    {
        EnsureInterfaceMap();
        const auto it = s_interfaceMap.find(iid);
        return (it == s_interfaceMap.end()) ? nullptr : it->second;
    }

    ProjectedInterface::ProjectedInterface(
        const winrt::guid& iid, InstanceFactory factory, InitializerFunction initializer) :
        m_iid(iid),
        m_factory(factory), m_initializer(initializer)
    {
    }

    const winrt::guid& ProjectedInterface::InterfaceId() const noexcept
    {
        return m_iid;
    }

    std::shared_ptr<ProjectedFunction> ProjectedInterface::FindMethod(std::string_view name)
    {
        const auto methods = Methods();
        const auto it = methods.find(name);
        return it == methods.end() ? nullptr : it->second;
    }

    std::shared_ptr<ProjectedFunction> ProjectedInterface::FindMethod(std::string_view name, uint16_t numArgs)
    {
        auto method = FindMethod(name);
        return method->CanInvoke(numArgs) ? method : nullptr;
    }

    std::shared_ptr<ProjectedFunction> ProjectedInterface::FindMethod(uint16_t numArgs)
    {
        for (const auto& method : Methods())
        {
            if (method.second->CanInvoke(numArgs))
            {
                return method.second;
            }
        }
        return nullptr;
    }

    const std::unordered_map<std::string_view, std::shared_ptr<ProjectedFunction>>& ProjectedInterface::Methods()
    {
        EnsureInitialized();
        return m_methodMap;
    }

    std::shared_ptr<IProjectedPropertyBase> ProjectedInterface::FindProperty(std::string_view name)
    {
        const auto properties = Properties();
        const auto it = properties.find(name);
        return it == properties.end() ? nullptr : it->second;
    }

    const std::unordered_map<std::string_view, std::shared_ptr<IProjectedPropertyBase>>& ProjectedInterface::
        Properties()
    {
        EnsureInitialized();
        return m_propertyMap;
    }

    std::shared_ptr<IProjectedEventBase> ProjectedInterface::FindEvent(std::string_view name)
    {
        const auto events = Events();
        const auto it = events.find(name);
        return it == events.end() ? nullptr : it->second;
    }

    const std::unordered_map<std::string_view, std::shared_ptr<IProjectedEventBase>>& ProjectedInterface::Events()
    {
        EnsureInitialized();
        return m_eventMap;
    }

    std::shared_ptr<IProjectedInterfaceInstance> ProjectedInterface::CreateInstance(
        std::shared_ptr<ProjectionsContext> context, winrt::Windows::Foundation::IInspectable instance)
    {
        return m_factory(std::move(context), std::move(instance), shared_from_this());
    }

    void ProjectedInterface::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            const auto data = m_initializer();

            m_propertyMap.reserve(data.Properties.size());
            for (auto&& property : data.Properties)
            {
                m_propertyMap[property->Name()] = std::move(property);
            }

            m_methodMap.reserve(data.Methods.size());
            for (auto&& method : data.Methods)
            {
                m_methodMap[method->Name()] = std::move(method);
            }

            m_eventMap.reserve(data.Events.size());
            for (auto&& event : data.Events)
            {
                m_eventMap[event->Name()] = std::move(event);
            }

            m_isInitialized = true;
        }
    }

    namespace
    {
        ProjectedInterfaceData InitIActivationFactoryInterface()
        {
            return { {},
                { ProjectedFunction::Create("activateInstance"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::IActivationFactory>(
                        &winrt::Windows::Foundation::IActivationFactory::ActivateInstance<
                            winrt::Windows::Foundation::IInspectable>,
                        ConvertInterfaceToValue<winrt::Windows::Foundation::IInspectable>)) },
                {} };
        }

        // TODO: For the sake of Windows.Storage.Streams.IBufferAccess, IBufferByteAccess should be projected (unlike
        // Chakra) which would make IRandomAccessStream far more usable with JS.
    }

    void ProjectedInterface::EnsureInterfaceMap()
    {
        if (s_interfaceMap.empty())
        {
            auto projectedInterfaces = GetProjectedInterfaces();
            s_interfaceMap.reserve(projectedInterfaces.size() + 1);

            // IActivationFactory is a Classic COM interface and cannot be generated from a winmd,
            s_interfaceMap[__uuidof(::IActivationFactory)] =
                ProjectedInterface::Create<winrt::Windows::Foundation::IActivationFactory>(
                    InitIActivationFactoryInterface);
            for (auto&& projectedInterface : projectedInterfaces)
            {
                s_interfaceMap[projectedInterface->InterfaceId()] = std::move(projectedInterface);
            }
        }
    }
}
