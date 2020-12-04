#include "pch.h"

#include "ProjectionsContext.h"

namespace WinRTTurboModule
{
    std::map<DWORD, std::weak_ptr<ProjectionsContext>> ProjectionsContext::s_instanceMap;
    wil::srwlock ProjectionsContext::s_lock;

    std::shared_ptr<ProjectionsContext> ProjectionsContext::Create(
        jsi::Runtime& runtime, std::shared_ptr<react::CallInvoker> invoker)
    {
        auto context = std::make_shared<ProjectionsContext>(runtime, std::move(invoker));

        const auto lock = s_lock.lock_exclusive();
        s_instanceMap.emplace(::GetCurrentThreadId(), context);

        return context;
    }

    std::shared_ptr<ProjectionsContext> ProjectionsContext::TryGetForCurrentThread()
    {
        const auto lock = s_lock.lock_shared();
        if (const auto it = s_instanceMap.find(::GetCurrentThreadId()); it != s_instanceMap.end())
        {
            if (auto context = it->second.lock())
            {
                return context;
            }
        }
        return nullptr;
    }

    ProjectionsContext::ProjectionsContext(jsi::Runtime& runtime, std::shared_ptr<react::CallInvoker> invoker) :
        Runtime(runtime), Invoker(std::make_shared<CallInvokerWrapper>(std::move(invoker))), InstanceFactory(*this),
        EventRegistrar(*this)
    {
    }

    ProjectionsContext::~ProjectionsContext()
    {
        const auto lock = s_lock.lock_exclusive();
        s_instanceMap.erase(::GetCurrentThreadId());
    }
}