#include "pch.h"

#include "ProjectedEventRegistrar.h"
#include "ProjectionsContext.h"

namespace WinRTTurboModule
{
    ProjectedEventRegistrar::ProjectedEventRegistrar(ProjectionsContext& context) : m_context(context)
    {
    }

    std::shared_ptr<ProjectedEventInstance> ProjectedEventRegistrar::Get(
        const winrt::Windows::Foundation::IInspectable& inspectable, const std::shared_ptr<IProjectedEventBase>& event)
    {
#if _DEBUG
        FAIL_FAST_IF(m_context.Invoker->ThreadId != ::GetCurrentThreadId());
#endif

        // Do an opportunistic cleanup to prevent unbounded growth of the map with dead objects over time.
        CleanupMapIfApplicable();

        const auto emplaceResult = m_map.emplace(winrt::get_abi(inspectable), Entry{});
        auto& entry = emplaceResult.first->second;
        if (emplaceResult.second)
        {
            entry.WeakRef = inspectable;
        }

        // Most objects would have a small number of event handlers, thus to reduce memory usage and lookup times we can
        // just use plain a std::vector with O(n) lookup.
        const auto it = std::find_if(entry.EventInstances.begin(), entry.EventInstances.end(),
            [&event](const std::shared_ptr<ProjectedEventInstance>& instance) { return instance->Event() == event; });

        if (it != entry.EventInstances.end())
        {
            return *it;
        }

        auto instance = std::make_shared<ProjectedEventInstance>(event);
        entry.EventInstances.push_back(instance);
        return instance;
    }

    void ProjectedEventRegistrar::CleanupMapIfApplicable()
    {
        constexpr uint64_t c_cleanupInterval = 5 * 60 * 1000; // ticks (ms) since last cleanup attempted.
        constexpr uint32_t c_cleanupThreshold = 100; // number of calls since last cleanup attempted.

        if (++m_requestsSinceCleanup >= c_cleanupThreshold)
        {
            if (const auto ticks = GetTickCount64(); ticks - m_lastCleanUp > c_cleanupInterval)
            {
                m_requestsSinceCleanup = 0;
                m_lastCleanUp = GetTickCount64();

                auto it = m_map.begin();
                while (it != m_map.end())
                {
                    if (it->second.WeakRef)
                    {
                        ++it;
                    }
                    else
                    {
                        it = m_map.erase(it);
                    }
                }
            }
        }
    }
}