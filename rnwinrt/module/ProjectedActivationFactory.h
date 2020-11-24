#pragma once
#include "ProjectedRuntimeClassInstance.h"
#include "ProjectedValueProvider.h"

namespace WinRTTurboModule
{
    class ProjectedActivationFactory final : public IProjectedValueProvider
    {
    public:
        using GetActivationFactory = winrt::Windows::Foundation::IInspectable (*)();

        template <typename T>
        static std::shared_ptr<IProjectedValueProvider> Create(const std::string_view& name,
            const std::shared_ptr<ProjectionsContext>& context, std::set<winrt::guid>&& factoryInterfaceIds)
        {
            return std::make_shared<ProjectedActivationFactory>(name, context,
                &winrt::get_activation_factory<T, winrt::Windows::Foundation::IInspectable>,
                std::forward<std::set<winrt::guid>>(factoryInterfaceIds));
        }

        ProjectedActivationFactory(const std::string_view& name, const std::shared_ptr<ProjectionsContext>& context,
            GetActivationFactory getActivationFactory, std::set<winrt::guid>&& m_factoryInterfaceIds);
        virtual ~ProjectedActivationFactory() = default;

        virtual std::string_view Name() const override;
        virtual jsi::Value Value(jsi::Runtime& runtime) override;

    private:
        const std::string_view m_name;
        const std::shared_ptr<ProjectionsContext> m_context;
        const GetActivationFactory m_getActivationFactory;
        const std::set<winrt::guid> m_factoryInterfaceIds;

        std::optional<jsi::Object> m_object;
    };
}