#include "ProjectedRuntimeClassInstanceFactory.h"
#include "ProjectedRuntimeClassInstance.h"
#include "ProjectionsContext.h"
#include "pch.h"

namespace WinRTTurboModule
{
    ProjectedRuntimeClassInstanceFactory::ProjectedRuntimeClassInstanceFactory(ProjectionsContext& context) :
        m_context(context)
    {
    }

    jsi::Value ProjectedRuntimeClassInstanceFactory::Get(const winrt::Windows::Foundation::IInspectable& instance)
    {
#if _DEBUG
        FAIL_FAST_IF(m_context.Invoker->ThreadId != ::GetCurrentThreadId());
#endif

        // It is a not infrequent scenario where the same WinRT object will be accessed multiple times. To avoid
        // aliasing it in the wrapper HostObject multiple times, we should return a strong reference to the object to
        // the caller but hold a weak ref in case it accessed again for the JS garbage collector releases the Object and
        // its underlying HostObject.

        // Despite the use of a cache it is possible to have a cache miss when keying off of the void* pointer for
        // IInspectable because IInspectable could point to a different v-table entry since all WinRT interfaces inherit
        // IInspectable. It is also possible the class does something atypical like have QueryInterface return an
        // entirely different implementation depending on the interface.

        const auto key = winrt::get_abi(instance);
        if (const auto it = m_instanceCache.find(key); it != m_instanceCache.end())
        {
            if (m_weakObjectSupported)
            {
                if (auto value = std::get<jsi::WeakObject>(it->second).lock(m_context.Runtime); !value.isUndefined())
                {
                    return value;
                }
            }
            else if (auto hostObject = std::get<std::weak_ptr<jsi::HostObject>>(it->second).lock())
            {
                return jsi::Value(
                    m_context.Runtime, jsi::Object::createFromHostObject(m_context.Runtime, std::move(hostObject)));
            }
            m_instanceCache.erase(it);
        }

        auto hostObject =
            std::shared_ptr<jsi::HostObject>(new ProjectedRuntimeClassInstance(m_context.shared_from_this(), instance));
        const auto object = jsi::Object::createFromHostObject(m_context.Runtime, hostObject);

        if (m_weakObjectSupported)
        {
            try
            {
                m_instanceCache.emplace(key, jsi::WeakObject(m_context.Runtime, object));
            }
            catch (std::logic_error&)
            {
                // TODO: Looks like we are using V8 which doesn't support WeakObject per
                //       V8Runtime::createWeakObject/V8Runtime::lockWeakObject in
                //       https://github.com/microsoft/v8-jsi/blob/master/src/V8JsiRuntime.cpp so until it is fixed, we
                //       should be more careful to avoid aliasing and doing extra work (e.g. requesting a property
                //       multiple times) or come up with some sort of scheme like holding strong refs temporarily.
                m_weakObjectSupported = false;
            }
        }

        // Do not combine as an else condition as we want to cache the first time when m_weakObjectSupported -> false.
        if (!m_weakObjectSupported)
        {
            m_instanceCache.emplace(key, std::move(hostObject));
        }

        return jsi::Value(m_context.Runtime, object);
    }

    bool ProjectedRuntimeClassInstanceFactory::Remove(const winrt::Windows::Foundation::IInspectable& instance)
    {
#if _DEBUG
        FAIL_FAST_IF(m_context.Invoker->ThreadId != ::GetCurrentThreadId());
#endif
        return m_instanceCache.erase(winrt::get_abi(instance));
    }
}