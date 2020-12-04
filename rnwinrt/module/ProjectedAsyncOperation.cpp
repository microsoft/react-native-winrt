#include "pch.h"

#include "JsiHelpers.h"
#include "ProjectedAsyncOperation.h"
#include "ProjectedFunction.h"
#include "ProjectedInterface.h"

namespace WinRTTurboModule
{
    ProjectedAsyncOperationBaseMethods::ProjectedAsyncOperationBaseMethods(
        std::shared_ptr<ProjectionsContext> context) :
        m_threadId(GetCurrentThreadId()),
        m_context(std::move(context))
    {
    }

    std::optional<jsi::Value> ProjectedAsyncOperationBaseMethods::TryGetMethod(
        const jsi::PropNameID& propName, const std::string& propNameUtf8)
    {
        auto& runtime = m_context->Runtime;
        if (propNameUtf8 == "then"sv)
        {
            return jsi::Value(
                runtime, jsi::Function::createFromHostFunction(runtime, propName, 3,
                             [strongThis{ shared_from_this() }](jsi::Runtime& runtime, const jsi::Value& thisVal,
                                 const jsi::Value* args, size_t count) mutable {
                                 const auto undefined = jsi::Value::undefined();
                                 return strongThis->HandleThen(count >= 1 ? args[0] : undefined,
                                     count >= 2 ? args[1] : undefined, count >= 3 ? args[2] : undefined);
                             }));
        }
        else if (propNameUtf8 == "done"sv)
        {
            return jsi::Value(runtime,
                jsi::Function::createFromHostFunction(runtime, propName, 3,
                    [strongThis{ shared_from_this() }](jsi::Runtime& runtime, const jsi::Value& thisVal,
                        const jsi::Value* args, size_t count) mutable {
                        const auto undefined = jsi::Value::undefined();
                        return strongThis->HandleThen(count >= 1 ? args[0] : undefined,
                            count >= 2 ? args[1] : undefined, count >= 3 ? args[2] : undefined, true /*forDone*/);
                    }));
        }
        else if (propNameUtf8 == "catch"sv)
        {
            return jsi::Value(
                runtime, jsi::Function::createFromHostFunction(runtime, propName, 1,
                             [strongThis{ shared_from_this() }](jsi::Runtime& runtime, const jsi::Value& thisVal,
                                 const jsi::Value* args, size_t count) mutable {
                                 const auto undefined = jsi::Value::undefined();
                                 return strongThis->HandleThen(undefined, count >= 1 ? args[0] : undefined, undefined);
                             }));
        }
        else if (propNameUtf8 == "finally"sv)
        {
            return jsi::Value(
                runtime, jsi::Function::createFromHostFunction(runtime, propName, 2,
                             [strongThis{ shared_from_this() }](jsi::Runtime& runtime, const jsi::Value& thisVal,
                                 const jsi::Value* args, size_t count) mutable {
                                 // TODO: Technically "finally" callbacks should be invoked without args, but since it
                                 // is JS, passing args probably doesn't matter.
                                 const auto undefined = jsi::Value::undefined();
                                 const auto& function = count >= 1 ? args[0] : undefined;
                                 return strongThis->HandleThen(function, function, undefined);
                             }));
        }

        return {};
    }

    jsi::Value ProjectedAsyncOperationBaseMethods::HandleThen(
        const jsi::Value& onResolved, const jsi::Value& onRejected, const jsi::Value& onProgress, bool forDone)
    {
        FAIL_FAST_IF(m_threadId != ::GetCurrentThreadId());

        auto& runtime = m_context->Runtime;
        auto continuation = std::make_shared<Continuation>();
        if (!onResolved.isUndefined())
        {
            continuation->OnResolved = onResolved.asObject(runtime).asFunction(runtime);
        }

        if (!onRejected.isUndefined())
        {
            continuation->OnRejected = onRejected.asObject(runtime).asFunction(runtime);
        }

        if (!onProgress.isUndefined())
        {
            continuation->OnProgress = onProgress.asObject(runtime).asFunction(runtime);
        }

        auto result = forDone ? jsi::Value::undefined() : PromiseWrapper::Create(runtime, continuation->Promise);

        if (m_result)
        {
            // The async operation is already complete so we can immediately call the handlers and resolve the promise.
            // It is still supposed to be async and requeue so it doesn't happen synchronously to calling then.
            m_context->Invoker->CallAsync(
                [strongThis{ shared_from_this() }, continuation{ std::move(continuation) }]() {
                    strongThis->DispatchContinuation(*continuation);
                });
        }
        else
        {
            m_continuations.push_back(std::move(continuation));
        }

        return result;
    }

    void ProjectedAsyncOperationBaseMethods::OnCompleted(jsi::Value result, bool succeeded)
    {
        FAIL_FAST_IF(m_threadId != ::GetCurrentThreadId());

        m_result = std::move(result);
        m_succeeded = succeeded;

        // move m_continuations into the local variables so they will be freed on completion.
        const auto continuations = std::move(m_continuations);
        for (const auto& continuation : continuations)
        {
            DispatchContinuation(*continuation);
        }
    }

    void ProjectedAsyncOperationBaseMethods::DispatchContinuation(const Continuation& continuation) const
    {
        FAIL_FAST_IF(!m_result);

        auto& runtime = m_context->Runtime;
        bool succeeded = false;
        std::optional<jsi::Value> localResult;

        try
        {
            if (m_succeeded)
            {
                if (continuation.OnResolved)
                {
                    localResult = continuation.OnResolved->call(runtime, *m_result);
                }
                succeeded = true;
            }
            else
            {
                if (continuation.OnRejected)
                {
                    localResult = continuation.OnRejected->call(runtime, *m_result);
                    succeeded = true;
                }
            }
        }
        catch (winrt::hresult_error& e)
        {
            localResult = CreateError(runtime, e);
        }
        catch (std::exception& e)
        {
            localResult = CreateError(runtime, e);
        }

        const auto& effectiveResult = localResult ? *localResult : *m_result;
        if (!continuation.Promise)
        {
            // In the "done" scenario, throw unhandled errors instead of swallowing them.
            if (!succeeded)
            {
                throw jsi::JSError(runtime, jsi::Value(runtime, effectiveResult));
            }
        }
        else if (succeeded)
        {
            // Either initially succeeded and there was no resolved handler; or there was a resolved handler and did not
            // throw; or initially failed, but the rejected handler did not throw. In the first case, the original
            // result should be passed to resolve. Note that if the localResult happens to be a Promise itself, that JS
            // will coalesce them on calling resolve.
            continuation.Promise->Resolve(runtime, effectiveResult);
        }
        else
        {
            // The initial error was either not handled by an OnRejected handler or whichever of OnResolved or
            // OnRejected that was called threw an error which still means that the promise should be rejected.
            continuation.Promise->Reject(runtime, effectiveResult);
        }
    }

    void ProjectedAsyncOperationBaseMethods::OnProgress(jsi::Value progress)
    {
        FAIL_FAST_IF(m_threadId != ::GetCurrentThreadId());

        auto& runtime = m_context->Runtime;
        for (const auto& continuation : m_continuations)
        {
            if (continuation->OnProgress)
            {
                continuation->OnProgress->call(runtime, jsi::Value(runtime, progress));
            }
        }
    }

    ProjectedAsyncOperationBase::ProjectedAsyncOperationBase(std::shared_ptr<ProjectionsContext> context) :
        m_methods(std::make_shared<ProjectedAsyncOperationBaseMethods>(std::move(context)))
    {
    }

    jsi::Value ProjectedAsyncOperationBase::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        const auto propNameUtf8 = propName.utf8(runtime);
        if (auto method = m_methods->TryGetMethod(propName, propNameUtf8))
        {
            return std::move(*method);
        }
        else if (propNameUtf8 == "cancel"sv)
        {
            static const auto s_asyncInfo =
                ProjectedInterface::Get(winrt::guid_of<winrt::Windows::Foundation::IAsyncInfo>());
            FAIL_FAST_IF_NULL(s_asyncInfo);

            const auto cancelMethod = s_asyncInfo->FindMethod(propNameUtf8);
            FAIL_FAST_IF_NULL(cancelMethod);

            return cancelMethod->GetFunction(
                Instance().as<winrt::Windows::Foundation::IAsyncInfo>(), m_methods->Context());
        }
        else if (propNameUtf8 == "operation"sv)
        {
            if (!m_operation)
            {
                m_operation = ProjectedRuntimeClassInstance::Get(m_methods->Context(), Instance());
            }
            return jsi::Value(runtime, *m_operation);
        }

        return jsi::Value::undefined();
    }

    void ProjectedAsyncOperationBase::set(
        jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value)
    {
        throw jsi::JSError(runtime, "TypeError: Cannot assign to property '" + propName.utf8(runtime) +
                                        "' to a projected WinRT AsyncOperation");
    }

    std::vector<jsi::PropNameID> ProjectedAsyncOperationBase::getPropertyNames(jsi::Runtime& runtime)
    {
        std::vector<jsi::PropNameID> propNames;
        propNames.reserve(6);

        propNames.push_back(CreatePropNameID(runtime, "cancel"sv));
        propNames.push_back(CreatePropNameID(runtime, "catch"sv));
        propNames.push_back(CreatePropNameID(runtime, "done"sv));
        propNames.push_back(CreatePropNameID(runtime, "finally"sv));
        propNames.push_back(CreatePropNameID(runtime, "then"sv));
        propNames.push_back(CreatePropNameID(runtime, "operation"sv));

        return propNames;
    }
}