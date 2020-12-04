#pragma once
#include "JsiHelpers.h"
#include "ProjectionsContext.h"

namespace WinRTTurboModule
{
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
}