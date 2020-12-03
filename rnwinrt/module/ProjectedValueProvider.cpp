#include "pch.h"

#include "ProjectedValueProvider.h"

namespace WinRTTurboModule
{
    ProjectedHostObjectProvider::ProjectedHostObjectProvider(
        std::string_view name, std::shared_ptr<jsi::HostObject> hostObject) :
        m_name(name),
        m_hostObject(std::move(hostObject))
    {
    }

    std::string_view ProjectedHostObjectProvider::Name() const
    {
        return m_name;
    }

    jsi::Value ProjectedHostObjectProvider::Value(jsi::Runtime& runtime)
    {
        if (!m_object)
        {
            m_object = jsi::Object::createFromHostObject(runtime, m_hostObject);
        }
        return jsi::Value(runtime, *m_object);
    }
}