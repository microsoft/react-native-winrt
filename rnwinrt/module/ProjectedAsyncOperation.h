#pragma once
#include "ProjectedRuntimeClassInstance.h"

namespace WinRTTurboModule
{
    struct ProjectedAsyncOperationBaseMethods final :
        public std::enable_shared_from_this<ProjectedAsyncOperationBaseMethods>
    {
        ProjectedAsyncOperationBaseMethods(const std::shared_ptr<ProjectionsContext>& context);
        virtual ~ProjectedAsyncOperationBaseMethods() = default;

        std::optional<jsi::Value> TryGetMethod(const jsi::PropNameID& propName, const std::string& propNameUtf8);

        void OnCompleted(jsi::Value result, bool succeeded);
        void OnProgress(jsi::Value progress);

        std::shared_ptr<ProjectionsContext> Context() const
        {
            return m_context;
        }

    private:
        jsi::Value HandleThen(const jsi::Value& onResolved, const jsi::Value& onRejected, const jsi::Value& onProgress,
            bool forDone = false);

        struct Continuation
        {
            std::shared_ptr<PromiseWrapper> Promise;
            std::optional<jsi::Function> OnResolved;
            std::optional<jsi::Function> OnRejected;
            std::optional<jsi::Function> OnProgress;
        };
        void DispatchContinuation(const Continuation& continuation) const;

        const std::shared_ptr<ProjectionsContext> m_context;
        std::vector<std::shared_ptr<Continuation>> m_continuations;
        std::optional<jsi::Value> m_result;
        const DWORD m_threadId;
        bool m_succeeded{};
    };

    struct ProjectedAsyncOperationBase : public jsi::HostObject
    {
        ProjectedAsyncOperationBase(const std::shared_ptr<ProjectionsContext>& context);
        virtual ~ProjectedAsyncOperationBase() = default;

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) final;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value) final;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) final;

        virtual winrt::Windows::Foundation::IInspectable Instance() = 0;

    protected:
        std::shared_ptr<ProjectedAsyncOperationBaseMethods> m_methods;

    private:
        std::optional<jsi::Value> m_operation;
    };

    struct IsAsyncAction
    {
    };
    struct NoAsyncProgress
    {
    };

    template <typename T, typename R = IsAsyncAction, typename P = NoAsyncProgress>
    struct ProjectedAsyncOperation final : public ProjectedAsyncOperationBase
    {
        ProjectedAsyncOperation(const std::shared_ptr<ProjectionsContext>& context, const T& instance,
            NativeToValueConverter<R> valueConverter = nullptr, NativeToValueConverter<P> progressConverter = nullptr) :
            ProjectedAsyncOperationBase(context),
            m_instance(instance)
        {
            AwaitCompletion(context, instance, valueConverter, m_methods);

            if constexpr (!std::is_same_v<P, NoAsyncProgress>)
            {
                // Progress(...) internally calls winrt::impl::make_agile_delegate so the lambda should be called on the
                // original thread rather than on some arbitrary background thread.
                m_instance.Progress(
                    [progressConverter, methods{ m_methods }, context{ std::shared_ptr<ProjectionsContext>(context) }](
                        const auto& sender, const auto& progress) {
                        methods->OnProgress(progressConverter(context, progress));
                    });
            }
        }

        virtual ~ProjectedAsyncOperation() = default;

        virtual winrt::Windows::Foundation::IInspectable Instance() override
        {
            return m_instance;
        }

    private:
        static winrt::fire_and_forget AwaitCompletion(const std::shared_ptr<ProjectionsContext> context, T instance,
            NativeToValueConverter<R> converter, std::shared_ptr<ProjectedAsyncOperationBaseMethods> methods)
        {
            auto& runtime = context->Runtime;
            const auto& invoker = context->Invoker;

            try
            {
                if constexpr (std::is_same_v<R, IsAsyncAction>)
                {
                    // "Call" is necessary because RNW uses ChakraCore or V8 with a MTA thread as opposed to an ASTA
                    // even though JS is single-threaded; therefore, we must call back into JS on the original thread
                    // to avoid trouble. Call will queue the lambda on the JS thread if the current thread is not the
                    // same as that of JS else execute it synchronously.

                    // PERF: If co_await causes code-bloat, it is reasonable to just use a Completed delegate instead.
                    co_await instance;
                    invoker->Call(
                        [&runtime, methods]() { methods->OnCompleted(jsi::Value::undefined(), true /*succeeded*/); });
                }
                else
                {
                    invoker->Call([methods, converter, context{ std::move(context) }, result{ co_await instance }]() {
                        methods->OnCompleted(converter(context, result), true /*succeeded*/);
                    });
                }
            }
            catch (winrt::hresult_error e)
            {
                invoker->Call([&runtime, methods, e{ std::move(e) }]() {
                    methods->OnCompleted(CreateError(runtime, e), false /*succeeded*/);
                });
            }
            catch (std::exception e)
            {
                invoker->Call([&runtime, methods, e{ std::move(e) }]() {
                    methods->OnCompleted(CreateError(runtime, e), false /*succeeded*/);
                });
            }
        }

        T m_instance;
    };
}