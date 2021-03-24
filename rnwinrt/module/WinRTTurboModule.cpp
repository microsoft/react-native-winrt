#include "pch.h"

#include "WinRTTurboModule.h"

#include <rnwinrt/base.cpp>

using namespace facebook;
using namespace jswinrt;

jsi::Value WinRTTurboModuleSpecJSI_initialize(
    jsi::Runtime& runtime, react::TurboModule& turboModule, const jsi::Value*, size_t);

thread_local runtime_context* current_thread_context = nullptr;

WinRTTurboModule::WinRTTurboModule(std::shared_ptr<react::CallInvoker> invoker) :
    TurboModule("WinRTTurboModule", invoker), m_invoker(std::move(invoker))
{
    methodMap_["initialize"] = MethodMetadata{ 0, WinRTTurboModuleSpecJSI_initialize };

    APTTYPE type;
    APTTYPEQUALIFIER typeQualifier;
    if (::CoGetApartmentType(&type, &typeQualifier) == CO_E_NOTINITIALIZED)
    {
        winrt::terminate();
    }
}

WinRTTurboModule::~WinRTTurboModule()
{
    current_thread_context->release();
    current_thread_context = nullptr;
}

// Functions exposed to JS
void WinRTTurboModule::initialize(jsi::Runtime& runtime)
{
    if (!m_initialized)
    {
        m_initialized = true;

        assert(!current_thread_context);
        current_thread_context = new runtime_context(
            runtime, [invoker = m_invoker](std::function<void()> fn) { invoker->invokeAsync(std::move(fn)); });

        auto global = runtime.global();
        for (auto data : root_namespaces)
        {
            global.setProperty(runtime, make_string(runtime, data->name), data->create(runtime));
        }
    }
}

runtime_context* jswinrt::current_runtime_context()
{
    auto result = current_thread_context;
    if (!result)
    {
        // WinRT module not initialized for the current thread. This is likely because of a logic error in the runtime
        // where we are trying to access data from a background thread instead of caching the context on creation
        std::terminate();
    }

    return result;
}

jsi::Value WinRTTurboModuleSpecJSI_initialize(
    jsi::Runtime& runtime, react::TurboModule& turboModule, const jsi::Value*, size_t)
{
    static_cast<WinRTTurboModule*>(&turboModule)->initialize(runtime);
    return jsi::Value::undefined();
}

std::shared_ptr<react::TurboModule> CreateWinRTTurboModule(
    const std::shared_ptr<facebook::react::CallInvoker>& jsInvoker)
{
    return std::make_shared<WinRTTurboModule>(jsInvoker);
}
