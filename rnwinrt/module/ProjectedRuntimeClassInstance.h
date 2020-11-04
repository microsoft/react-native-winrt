#pragma once
#include "ProjectedInterface.h"
#include "ProjectedValueProvider.h"

namespace WinRTTurboModule
{
    class ProjectedRuntimeClassInstance final : public jsi::HostObject
    {
    public:
        static jsi::Value Get(const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IInspectable& instance);

        ProjectedRuntimeClassInstance(const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IInspectable& instance);
        virtual ~ProjectedRuntimeClassInstance();

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

        std::optional<jsi::Value> GetEventListenerFunction(jsi::Runtime& runtime, bool isAddEventListener);

        inline auto Instance() const noexcept
        {
            return m_instance;
        }

        inline const auto& Interfaces() noexcept
        {
            EnsureInitialized();
            return m_interfaceInstances;
        }

    private:
        void EnsureInitialized();

        const std::shared_ptr<ProjectionsContext> m_context;
        const winrt::Windows::Foundation::IInspectable m_instance;
        std::vector<std::shared_ptr<IProjectedInterfaceInstance>> m_interfaceInstances;
        bool m_isInitialized{};
    };

    jsi::Function CreateCompositeConstructor(jsi::Runtime& runtime, std::string className, std::vector<std::shared_ptr<IProjectedInterfaceInstance>> factoryInterfaceInstances);
    jsi::Value CreateCompositeMethod(jsi::Runtime& runtime, std::string methodName, std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>, std::shared_ptr<ProjectedFunction>>> interfaceMethods);
}