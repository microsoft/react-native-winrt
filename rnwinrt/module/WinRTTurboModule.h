#pragma once
#include "JsiHelpers.h"
#include "ProjectionsContext.h"

namespace WinRTTurboModule
{
    class JSI_EXPORT WinRTTurboModuleSpecJSI : public react::TurboModule
    {
    protected:
        WinRTTurboModuleSpecJSI(const std::shared_ptr<react::CallInvoker>& invoker);

    public:
        virtual void initialize(jsi::Runtime& runtime) = 0;
    };

    class WinRTTurboModule final : public WinRTTurboModuleSpecJSI
    {
    public:
        WinRTTurboModule(const std::shared_ptr<react::CallInvoker>& invoker);

        // Functions exposed to JS
        void initialize(jsi::Runtime& runtime) override;

    private:
        const std::shared_ptr<react::CallInvoker> m_invoker;
        std::shared_ptr<ProjectionsContext> m_context;
    };
}