#pragma once
#include "JsiHelpers.h"
#include "ProjectedEventRegistrar.h"
#include "ProjectedRuntimeClassInstanceFactory.h"

namespace WinRTTurboModule
{
    struct ProjectionsContext final : public std::enable_shared_from_this<ProjectionsContext>
    {
        static std::shared_ptr<ProjectionsContext> Create(
            jsi::Runtime& runtime, std::shared_ptr<react::CallInvoker> invoker);
        static std::shared_ptr<ProjectionsContext> TryGetForCurrentThread();

        ProjectionsContext(jsi::Runtime& runtime, std::shared_ptr<react::CallInvoker> invoker);
        ~ProjectionsContext();

        // TODO: Is there actually a safe way to hold runtime such that we can determine when it is no longer valid?
        jsi::Runtime& Runtime;
        const std::shared_ptr<CallInvokerWrapper> Invoker;
        ProjectedRuntimeClassInstanceFactory InstanceFactory;
        ProjectedEventRegistrar EventRegistrar;

    private:
        std::unordered_map<void*, std::variant<jsi::WeakObject, std::weak_ptr<jsi::HostObject>>>
            RuntimeClassInstanceCache;

        static std::map<DWORD, std::weak_ptr<ProjectionsContext>> s_instanceMap;
        static wil::srwlock s_lock;
    };
}