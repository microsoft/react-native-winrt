#pragma once

namespace WinRTTurboModule
{
    struct DECLSPEC_NOVTABLE IProjectedValueProvider
    {
        virtual std::string_view Name() const = 0;
        virtual jsi::Value Value(jsi::Runtime& runtime) = 0;
    };

    class ProjectedHostObjectProvider final : public IProjectedValueProvider
    {
    public:
        ProjectedHostObjectProvider(const std::string_view& name, const std::shared_ptr<jsi::HostObject>& hostObject);
        virtual ~ProjectedHostObjectProvider() = default;

        virtual std::string_view Name() const override;
        virtual jsi::Value Value(jsi::Runtime& runtime) override;

    private:
        const std::string_view m_name;
        const std::shared_ptr<jsi::HostObject> m_hostObject;
        std::optional<jsi::Object> m_object;
    };
}