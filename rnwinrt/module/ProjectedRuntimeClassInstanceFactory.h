#pragma once

namespace WinRTTurboModule
{
    struct ProjectionsContext;

    struct ProjectedRuntimeClassInstanceFactory
    {
        explicit ProjectedRuntimeClassInstanceFactory(ProjectionsContext& context);

        jsi::Value Get(const winrt::Windows::Foundation::IInspectable& instance);
        bool Remove(const winrt::Windows::Foundation::IInspectable& instance);

    private:
        ProjectionsContext& m_context;
        std::unordered_map<void*, std::variant<jsi::WeakObject, std::weak_ptr<jsi::HostObject>>> m_instanceCache;
        bool m_weakObjectSupported{ true };
    };
}