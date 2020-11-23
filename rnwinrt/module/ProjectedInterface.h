#pragma once
#include "ProjectedEvent.h"
#include "ProjectedEventRegistrar.h"
#include "ProjectedFunction.h"
#include "ProjectedProperty.h"

namespace WinRTTurboModule
{
    class ProjectedInterface;

    struct DECLSPEC_NOVTABLE IProjectedInterfaceInstance
    {
        virtual ProjectedInterface& Interface() const noexcept = 0;

        virtual jsi::Value GetMethod(jsi::Runtime& runtime, const std::shared_ptr<ProjectedFunction>& method) = 0;
        virtual jsi::Value InvokeMethod(jsi::Runtime& runtime, const std::shared_ptr<ProjectedFunction>& method,
            const jsi::Value* args, const uint16_t numArgs) = 0;

        virtual jsi::Value GetProperty(
            jsi::Runtime& runtime, const std::shared_ptr<IProjectedPropertyBase>& property) = 0;
        virtual void SetProperty(jsi::Runtime& runtime, const std::shared_ptr<IProjectedPropertyBase>& property,
            const jsi::Value& value) = 0;

        virtual void AddEventListener(
            jsi::Runtime& runtime, const std::shared_ptr<IProjectedEventBase>& event, const jsi::Value& value) = 0;
        virtual void RemoveEventListener(
            jsi::Runtime& runtime, const std::shared_ptr<IProjectedEventBase>& event, const jsi::Value& value) = 0;
    };

    template <typename I>
    class ProjectedInterfaceInstance final : public IProjectedInterfaceInstance
    {
    public:
        static std::shared_ptr<IProjectedInterfaceInstance> Create(const std::shared_ptr<ProjectionsContext>& context,
            const winrt::Windows::Foundation::IInspectable& instance,
            std::shared_ptr<ProjectedInterface>&& projectedInterface)
        {
            // Use "new" instead of std::make_shared to avoid specializing
            // std::shared_ptr<ProjectedInterfaceInstance<I>> to reduce the sizes of object files.
            return std::shared_ptr<ProjectedInterfaceInstance>(new ProjectedInterfaceInstance<I>(
                context, instance, std::forward<std::shared_ptr<ProjectedInterface>>(projectedInterface)));
        }

        ProjectedInterfaceInstance(const std::shared_ptr<ProjectionsContext>& context,
            const winrt::Windows::Foundation::IInspectable& instance,
            std::shared_ptr<ProjectedInterface>&& projectedInterface) :
            m_context(context),
            m_inspectableInstance(instance),
            m_projectedInterface(std::forward<std::shared_ptr<ProjectedInterface>>(projectedInterface))
        {
        }

        virtual ProjectedInterface& Interface() const noexcept override
        {
            return *m_projectedInterface;
        }

        virtual jsi::Value GetMethod(jsi::Runtime& runtime, const std::shared_ptr<ProjectedFunction>& method) override
        {
            EnsureInitialized();
            return method->GetFunction<I>(m_queriedInstance, m_context);
        }

        virtual jsi::Value InvokeMethod(jsi::Runtime& runtime, const std::shared_ptr<ProjectedFunction>& method,
            const jsi::Value* args, const uint16_t numArgs) override
        {
            EnsureInitialized();
            return method->Invoke<I>(m_queriedInstance, m_context, args, numArgs);
        }

        // Use "static_cast" instead of std::static_pointer_cast to avoid specializing
        // std::shard_ptr<IProjectedProperty<I>>.
        virtual jsi::Value GetProperty(
            jsi::Runtime& runtime, const std::shared_ptr<IProjectedPropertyBase>& property) override
        {
            EnsureInitialized();
            return static_cast<IProjectedProperty<I>*>(property.get())->Get(m_queriedInstance, m_context);
        }

        virtual void SetProperty(jsi::Runtime& runtime, const std::shared_ptr<IProjectedPropertyBase>& property,
            const jsi::Value& value) override
        {
            EnsureInitialized();
            static_cast<IProjectedProperty<I>*>(property.get())->Set(m_queriedInstance, m_context, value);
        }

        virtual void AddEventListener(
            jsi::Runtime& runtime, const std::shared_ptr<IProjectedEventBase>& event, const jsi::Value& value) override
        {
            EnsureInitialized();
            m_context->EventRegistrar.Get(m_inspectableInstance, event)->Add(m_queriedInstance, m_context, value);
        }

        virtual void RemoveEventListener(
            jsi::Runtime& runtime, const std::shared_ptr<IProjectedEventBase>& event, const jsi::Value& value) override
        {
            EnsureInitialized();
            m_context->EventRegistrar.Get(m_inspectableInstance, event)->Remove(m_queriedInstance, m_context, value);
        }

    private:
        void EnsureInitialized()
        {
            if (!m_queriedInstance)
            {
                m_queriedInstance = m_inspectableInstance.as<I>();
            }
        }

        const std::shared_ptr<ProjectionsContext> m_context;
        const winrt::Windows::Foundation::IInspectable m_inspectableInstance;
        const std::shared_ptr<ProjectedInterface> m_projectedInterface;
        I m_queriedInstance{ nullptr };
    };

    struct ProjectedInterfaceData
    {
        std::vector<std::shared_ptr<IProjectedPropertyBase>> Properties;
        std::vector<std::shared_ptr<ProjectedFunction>> Methods;
        std::vector<std::shared_ptr<IProjectedEventBase>> Events;
    };

    // ProjectedInterface is not templated by the winrt interface to reduce the amount of specialized code. Avoiding
    // specializing STL contains greatly reduces the size of object files and static libs. For example, even
    // std::enable_shared_from_this<ProjectedInterface<I>> is very expensive.
    class ProjectedInterface final : public std::enable_shared_from_this<ProjectedInterface>
    {
    public:
        using InitializerFunction = ProjectedInterfaceData (*)();
        using InstanceFactory = std::shared_ptr<IProjectedInterfaceInstance> (*)(
            const std::shared_ptr<ProjectionsContext>&, const winrt::Windows::Foundation::IInspectable&,
            std::shared_ptr<ProjectedInterface>&&);

        static std::shared_ptr<ProjectedInterface> Get(const winrt::guid& iid);

        template <typename I>
        static std::shared_ptr<ProjectedInterface> Create(InitializerFunction initializer)
        {
            return std::make_shared<ProjectedInterface>(
                winrt::guid_of<I>(), &ProjectedInterfaceInstance<I>::Create, initializer);
        }

        ProjectedInterface(const winrt::guid& iid, InstanceFactory factory, InitializerFunction initializer);
        virtual ~ProjectedInterface() = default;

        const winrt::guid& InterfaceId() const noexcept;

        std::shared_ptr<ProjectedFunction> FindMethod(const std::string_view& name);
        std::shared_ptr<ProjectedFunction> FindMethod(const std::string_view& name, const uint16_t numArgs);
        std::shared_ptr<ProjectedFunction> FindMethod(const uint16_t numArgs);
        const std::unordered_map<std::string_view, std::shared_ptr<ProjectedFunction>>& Methods();

        std::shared_ptr<IProjectedPropertyBase> FindProperty(const std::string_view& name);
        const std::unordered_map<std::string_view, std::shared_ptr<IProjectedPropertyBase>>& Properties();

        std::shared_ptr<IProjectedEventBase> FindEvent(const std::string_view& name);
        const std::unordered_map<std::string_view, std::shared_ptr<IProjectedEventBase>>& Events();

        std::shared_ptr<IProjectedInterfaceInstance> CreateInstance(const std::shared_ptr<ProjectionsContext>& context,
            const winrt::Windows::Foundation::IInspectable& instance);

    private:
        void EnsureInitialized();

        static void EnsureInterfaceMap();
        static std::unordered_map<winrt::guid, std::shared_ptr<ProjectedInterface>> s_interfaceMap;

        std::unordered_map<std::string_view, std::shared_ptr<IProjectedPropertyBase>> m_propertyMap;
        std::unordered_map<std::string_view, std::shared_ptr<ProjectedFunction>> m_methodMap;
        std::unordered_map<std::string_view, std::shared_ptr<IProjectedEventBase>> m_eventMap;

        const winrt::guid& m_iid;
        const InitializerFunction m_initializer;
        const InstanceFactory m_factory;
        bool m_isInitialized{};
    };
}