#pragma once
#include "JsiHelpers.h"
#include "ProjectedValueProvider.h"

namespace WinRTTurboModule
{
    class ProjectedNamespace final : public jsi::HostObject
    {
    public:
        using InitializerFunction = std::vector<std::shared_ptr<IProjectedValueProvider>>(*)(const std::shared_ptr<ProjectionsContext>&);
        static std::shared_ptr<IProjectedValueProvider> Create(const std::string_view& name, const std::shared_ptr<ProjectionsContext>& context, InitializerFunction initializer);

        ProjectedNamespace(const std::string_view& name, const std::shared_ptr<ProjectionsContext>& context, InitializerFunction initializer);
        virtual ~ProjectedNamespace() = default;

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        void EnsureInitialized();

        const std::string_view m_name;
        const std::shared_ptr<ProjectionsContext> m_context;
        std::unordered_map<std::string_view, std::shared_ptr<IProjectedValueProvider>> m_propertyMap;
        const InitializerFunction m_initializer;
        bool m_isInitialized{};
    };
}