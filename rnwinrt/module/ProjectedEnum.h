#pragma once
#include "ProjectedValueProvider.h"

namespace WinRTTurboModule
{
    class ProjectedEnum final : public jsi::HostObject
    {
    public:
        using InitializerFunction = std::unordered_map<std::string_view, int64_t> (*)();
        static std::shared_ptr<IProjectedValueProvider> Create(
            const std::string_view& name, InitializerFunction initializer);

        ProjectedEnum(const std::string_view& name, InitializerFunction initializer);
        virtual ~ProjectedEnum() = default;

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        void EnsureInitialized();

        const std::string_view m_name;
        const InitializerFunction m_initializer;
        std::unordered_map<std::string_view, int64_t> m_propertyMap;
        bool m_isInitialized{};
    };
}