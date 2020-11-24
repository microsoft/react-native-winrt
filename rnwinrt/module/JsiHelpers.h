#pragma once
namespace WinRTTurboModule
{
    struct ProjectionsContext;

    inline jsi::PropNameID CreatePropNameID(jsi::Runtime& runtime, const std::string_view& value)
    {
        return jsi::PropNameID::forUtf8(runtime, reinterpret_cast<const uint8_t*>(value.data()), value.size());
    }

    inline jsi::String CreateString(jsi::Runtime& runtime, const std::string_view& value)
    {
        return jsi::String::createFromUtf8(runtime, reinterpret_cast<const uint8_t*>(value.data()), value.size());
    }

    jsi::String CreateString(jsi::Runtime& runtime, const std::wstring_view& value);

    jsi::Value CreateError(jsi::Runtime& runtime, const std::string_view& message);
    jsi::Value CreateError(jsi::Runtime& runtime, const std::exception& exception);
    jsi::Value CreateError(jsi::Runtime& runtime, const winrt::hresult_error& error);

    struct PromiseWrapper final
    {
        static jsi::Value Create(jsi::Runtime& runtime, std::shared_ptr<PromiseWrapper>& wrapper);
        PromiseWrapper(jsi::Function resolve, jsi::Function reject);

        void Resolve(jsi::Runtime& runtime, const jsi::Value& value);
        void Reject(jsi::Runtime& runtime, const jsi::Value& error);

    private:
        jsi::Function m_resolve;
        jsi::Function m_reject;
        bool m_completed{};
    };

    struct CallInvokerWrapper final
    {
        CallInvokerWrapper(const std::shared_ptr<react::CallInvoker>& CallInvokerWrapper);

        // Run the function synchronously if on the JS thread or asynchronously if not.
        void Call(std::function<void()>&& func);

        // Always queue the function to run asynchronously.
        void CallAsync(std::function<void()>&& func);

        // Run the function synchronously if on the JS thread or asynchronously and block until complete if not.
        void CallSync(std::function<void()>&& func);

        const DWORD ThreadId;

    private:
        const std::shared_ptr<react::CallInvoker> m_callInvokerWrapper;
    };
}

template <typename T>
using NativeToValueConverter = jsi::Value (*)(const std::shared_ptr<WinRTTurboModule::ProjectionsContext>&, const T&);

template <typename T>
using ValueToNativeConverter = T (*)(const std::shared_ptr<WinRTTurboModule::ProjectionsContext>&, const jsi::Value&);