#include "pch.h"

#include "JsiHelpers.h"
#include "ProjectedValueConverters.h"
#include "ProjectionsContext.h"

namespace WinRTTurboModule
{
    jsi::String CreateString(jsi::Runtime& runtime, const std::wstring_view& value)
    {
        if (value.empty())
        {
            return jsi::String::createFromAscii(runtime, "");
        }

        // WideCharToMultiByte is requesting the size in utf8 characters required for 'value' without null termination
        // as std::string constructed with a fill will actually allocated 'outputLength + 1' characters and asign the
        // last as the null terminator automatically.

        const auto outputLength =
            ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, value.data(), static_cast<int32_t>(value.size()),
                nullptr /*lpMultiByteStr*/, 0 /*cbMultiByte*/, L'\0' /*lpDefaultChar*/, nullptr /*lpUsedDefaultChar*/);
        winrt::check_bool(outputLength);

        std::string multibyteString(outputLength, '\0');
        winrt::check_bool(
            ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, value.data(), static_cast<int32_t>(value.size()),
                &multibyteString[0], outputLength, L'\0' /*lpDefaultChar*/, nullptr /*lpUsedDefaultChar*/));

        return jsi::String::createFromUtf8(runtime, multibyteString);
    }

    jsi::Value CreateError(jsi::Runtime& runtime, const std::string_view& message)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "message", CreateString(runtime, message));
        return jsi::Value(runtime, result);
    }

    jsi::Value CreateError(jsi::Runtime& runtime, const std::exception& exception)
    {
        return CreateError(runtime, exception.what());
    }

    jsi::Value CreateError(jsi::Runtime& runtime, const winrt::hresult_error& error)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "message", CreateString(runtime, error.message()));
        result.setProperty(runtime, "number", jsi::Value(static_cast<int32_t>(error.code())));
        return jsi::Value(runtime, result);
    }

    jsi::Value PromiseWrapper::Create(jsi::Runtime& runtime, std::shared_ptr<PromiseWrapper>& wrapper)
    {
        wrapper = {};

        jsi::Function callback =
            jsi::Function::createFromHostFunction(runtime, jsi::PropNameID::forAscii(runtime, "fn"), 2,
                [&wrapper](jsi::Runtime& runtime, const jsi::Value& thisVal, const jsi::Value* args, size_t count) {
                    FAIL_FAST_IF(count < 2);
                    wrapper = std::make_shared<PromiseWrapper>(
                        args[0].asObject(runtime).asFunction(runtime), args[1].asObject(runtime).asFunction(runtime));
                    return jsi::Value::undefined();
                });

        jsi::Function s_promiseConstructor = runtime.global().getPropertyAsFunction(runtime, "Promise");
        return s_promiseConstructor.callAsConstructor(runtime, callback);
    }

    PromiseWrapper::PromiseWrapper(jsi::Function resolve, jsi::Function reject) :
        m_resolve(std::move(resolve)), m_reject(std::move(reject))
    {
    }

    void PromiseWrapper::Resolve(jsi::Runtime& runtime, const jsi::Value& result)
    {
        FAIL_FAST_IF(m_completed);
        m_completed = true;
        m_resolve.call(runtime, result);
    }

    void PromiseWrapper::Reject(jsi::Runtime& runtime, const jsi::Value& result)
    {
        FAIL_FAST_IF(m_completed);
        m_completed = true;
        m_reject.call(runtime, result);
    }

    CallInvokerWrapper::CallInvokerWrapper(const std::shared_ptr<react::CallInvoker>& CallInvokerWrapper) :
        m_callInvokerWrapper(CallInvokerWrapper), ThreadId(GetCurrentThreadId())
    {
    }

    void CallInvokerWrapper::Call(std::function<void()>&& func)
    {
        if (ThreadId == GetCurrentThreadId())
        {
            func();
        }
        else
        {
            m_callInvokerWrapper->invokeAsync(std::forward<std::function<void()>>(func));
        }
    }

    void CallInvokerWrapper::CallAsync(std::function<void()>&& func)
    {
        m_callInvokerWrapper->invokeAsync(std::forward<std::function<void()>>(func));
    }

    void CallInvokerWrapper::CallSync(std::function<void()>&& func)
    {
        if (ThreadId == GetCurrentThreadId())
        {
            func();
        }
        else
        {
            // TODO: Use COM mechanism when crossing thread boundaries in order to propagate errors more cleanly.
            wil::shared_event event;
            event.create();

            m_callInvokerWrapper->invokeAsync([event, func{ std::forward<std::function<void()>>(func) }]() {
                func();
                event.SetEvent();
            });

            event.wait();
        }
    }
}