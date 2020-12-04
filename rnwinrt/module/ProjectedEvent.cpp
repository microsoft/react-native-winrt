#include "pch.h"

#include "ProjectedEvent.h"

namespace WinRTTurboModule
{
    ProjectedEventInstance::ProjectedEventInstance(std::shared_ptr<IProjectedEventBase> event) :
        m_event(std::move(event))
    {
    }

    ProjectedEventInstance::~ProjectedEventInstance()
    {
        FAIL_FAST_IF(!m_listeners.empty());
    }

    std::vector<std::pair<jsi::Object, winrt::event_token>>::iterator ProjectedEventInstance::FindEntry(
        jsi::Runtime& runtime, const jsi::Object& object)
    {
        return std::find_if(
            m_listeners.begin(), m_listeners.end(), [&](const std::pair<jsi::Object, winrt::event_token>& entry) {
                return jsi::Object::strictEquals(runtime, entry.first, object);
            });
    }

    bool ProjectedEventInstance::HasEntry(jsi::Runtime& runtime, const jsi::Object& object)
    {
        return FindEntry(runtime, object) != m_listeners.end();
    }

    void ProjectedEventInstance::AddEntry(jsi::Object object, winrt::event_token token)
    {
        m_listeners.emplace_back(std::move(object), token);
    }

    std::optional<std::pair<jsi::Object, winrt::event_token>> ProjectedEventInstance::RemoveEntry(
        jsi::Runtime& runtime, const jsi::Object& object)
    {
        if (const auto it = FindEntry(runtime, object); it != m_listeners.end())
        {
            auto token = std::move(*it);
            m_listeners.erase(it);
            return std::move(token);
        }
        return {};
    }
}