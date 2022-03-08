// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "pch.h"

#include "WinRTTurboModule.h"

#include <rnwinrt/base.cpp>

using namespace facebook;
using namespace rnwinrt;

jsi::Value WinRTTurboModuleSpecJSI_initialize(
    jsi::Runtime& runtime, react::TurboModule& turboModule, const jsi::Value*, size_t);

thread_local runtime_context* current_thread_context = nullptr;

WinRTTurboModule::WinRTTurboModule(std::shared_ptr<react::CallInvoker> invoker) :
    TurboModule("WinRTTurboModule", invoker), m_invoker(std::move(invoker))
{
    m_rootNamespaces.resize(root_namespaces.size());
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
    if (auto ptr = std::exchange(current_thread_context, nullptr))
    {
        ptr->release();
    }
}

// HostObject overrides
jsi::Value WinRTTurboModule::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
{
    auto name = propName.utf8(runtime);
    if (auto itr = find_by_name(root_namespaces, name); itr != root_namespaces.end())
    {
        return jsi::Value(runtime, get_namespace(runtime, itr - root_namespaces.begin()));
    }

    return TurboModule::get(runtime, propName);
}

// Functions exposed to JS
void WinRTTurboModule::initialize(jsi::Runtime& runtime)
{
    if (!m_initialized)
    {
        m_initialized = true;

        auto global = runtime.global();
        for (std::size_t i = 0; i < root_namespaces.size(); ++i)
        {
            global.setProperty(runtime, make_string(runtime, root_namespaces[i]->name), get_namespace(runtime, i));
        }
    }
}

jsi::Value& WinRTTurboModule::get_namespace(jsi::Runtime& runtime, std::size_t index)
{
    assert(index < root_namespaces.size());
    auto& slot = m_rootNamespaces[index];
    if (slot.isUndefined())
    {
        if (!current_thread_context)
        {
            current_thread_context = new runtime_context(
                runtime, [invoker = m_invoker](std::function<void()> fn) { invoker->invokeAsync(std::move(fn)); });
        }

        slot = root_namespaces[index]->create(runtime);
    }

    return slot;
}

runtime_context* rnwinrt::current_runtime_context()
{
    auto result = current_thread_context;
    if (!result)
    {
        // WinRT module not initialized for the current thread. This is likely because of a logic error in the runtime
        // where we are trying to access data from a background thread instead of caching the context on creation
        winrt::terminate();
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
