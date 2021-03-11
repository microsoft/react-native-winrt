#include "pch.h"
#include "WinRTTurboModule.h"

#include <rnwinrt/base.cpp>

namespace WinRTTurboModule
{
    WinRTTurboModule::WinRTTurboModule(std::shared_ptr<react::CallInvoker> invoker) :
        TurboModule("WinRTTurboModule", invoker), m_invoker(std::move(invoker))
    {
        methodMap_["initialize"] = MethodMetadata{ 0, __hostFunction_WinRTTurboModuleSpecJSI_initialize };

        APTTYPE type;
        APTTYPEQUALIFIER typeQualifier;
        FAIL_FAST_IF(CoGetApartmentType(&type, &typeQualifier) == CO_E_NOTINITIALIZED);
    }

    void WinRTTurboModule::initialize(jsi::Runtime& runtime)
    {
        if (!m_context)
        {
            m_context = ProjectionsContext::Create(runtime, m_invoker);

            auto global = runtime.global();
            for (const auto& provider : GetProjectedNamespaces(m_context))
            {
                global.setProperty(runtime, CreateString(runtime, provider->Name()), provider->Value(runtime));
            }
        }
    }

    jsi::Value __hostFunction_WinRTTurboModuleSpecJSI_initialize(
        jsi::Runtime& runtime, react::TurboModule& turboModule, const jsi::Value*, size_t)
    {
        static_cast<WinRTTurboModule*>(&turboModule)->initialize(runtime);
        return jsi::Value::undefined();
    }

}

std::shared_ptr<react::TurboModule> CreateWinRTTurboModule(
    const std::shared_ptr<facebook::react::CallInvoker>& jsInvoker)
{
    return std::make_shared<WinRTTurboModule::WinRTTurboModule>(jsInvoker);
}