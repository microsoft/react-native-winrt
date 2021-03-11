#pragma once
#include <rnwinrt/Projections.g.h>
#include <rnwinrt/base.h>

namespace WinRTTurboModule
{
    jsi::Value __hostFunction_WinRTTurboModuleSpecJSI_initialize(
        jsi::Runtime& runtime, react::TurboModule& turboModule, const jsi::Value*, size_t);

    class WinRTTurboModule final : public react::TurboModule
    {
    public:
        WinRTTurboModule(std::shared_ptr<react::CallInvoker> invoker);

        // Functions exposed to JS
        void initialize(jsi::Runtime& runtime);

    private:
        const std::shared_ptr<react::CallInvoker> m_invoker;
        std::shared_ptr<ProjectionsContext> m_context;
    };

    jsi::Value __hostFunction_WinRTTurboModuleSpecJSI_initialize(
        jsi::Runtime& runtime, react::TurboModule& turboModule, const jsi::Value*, size_t);
}

std::shared_ptr<react::TurboModule> CreateWinRTTurboModule(
    const std::shared_ptr<facebook::react::CallInvoker>& jsInvoker);
