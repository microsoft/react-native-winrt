#pragma once
#include "ProjectedAsyncOperation.h"
#include "ProjectedRuntimeClassInstance.h"
#include "ProjectionsContext.h"

namespace WinRTTurboModule
{
    // Boolean
    inline jsi::Value ConvertBooleanToValue(const std::shared_ptr<ProjectionsContext>& context, const bool& value)
    {
        return jsi::Value(value);
    }

    bool ConvertValueToBoolean(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // Integer/Float
    template <typename T>
    inline jsi::Value ConvertNumberToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        return jsi::Value(static_cast<double>(value));
    }

    template <typename T>
    inline T ConvertValueToNumber(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return static_cast<T>(value.asNumber());
    }

    // Enum
    template <typename T>
    inline jsi::Value ConvertEnumToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        return jsi::Value(static_cast<double>(static_cast<int64_t>(value)));
    }

    template <typename T>
    inline T ConvertValueToEnum(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return static_cast<T>(static_cast<int64_t>(value.asNumber()));
    }

    // char16_t
    jsi::Value ConvertCharToValue(const std::shared_ptr<ProjectionsContext>& context, const char16_t& value);
    char16_t ConvertValueToChar(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // winrt::hstring
    inline jsi::Value ConvertStringToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::hstring& value)
    {
        return CreateString(context->Runtime, static_cast<std::wstring_view>(value));
    }
    winrt::hstring ConvertValueToString(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // winrt::guid
    jsi::Value ConvertGuidToValue(const std::shared_ptr<ProjectionsContext>& context, const winrt::guid& value);
    winrt::guid ConvertValueToGuid(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // Structs: Converters should generally by code-gen'd aside from a few special types but it is possible that some
    // are missing. For example, there are IDL issues like Windows.UI.Core.CorePhysicalKeyStatus being WebHostHidden but
    // by types that are not such as Windows.Web.UI.Interop.IWebViewControlAcceleratorKeyPressedEventArgs (which
    // probably should also be WebHostHidden).
    template <typename T>
    jsi::Value ConvertStructToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion from native struct to JS not implemented for "sv) + typeid(T).name());
    }

    template <typename T>
    T ConvertValueToStruct(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion from JS to native struct not implemented for "sv) + typeid(T).name());
    }

    // winrt::hresult
    template <>
    inline jsi::Value ConvertStructToValue<winrt::hresult>(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::hresult& value)
    {
        return ConvertNumberToValue<int32_t>(context, static_cast<int32_t>(value));
    }

    template <>
    inline winrt::hresult ConvertValueToStruct<winrt::hresult>(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return winrt::hresult(ConvertValueToNumber<int32_t>(context, value));
    }

    // winrt::Windows::Foundation::DateTime <=> std::chrono::time_point<clock, TimeSpan>;
    template <>
    jsi::Value ConvertStructToValue<winrt::Windows::Foundation::DateTime>(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::DateTime& value);
    template <>
    winrt::Windows::Foundation::DateTime ConvertValueToStruct<winrt::Windows::Foundation::DateTime>(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // winrt::Windows::Foundation::TimeSpan <=> std::chrono::duration<int64_t, impl::filetime_period>;
    template <>
    jsi::Value ConvertStructToValue<winrt::Windows::Foundation::TimeSpan>(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::TimeSpan& value);
    template <>
    winrt::Windows::Foundation::TimeSpan ConvertValueToStruct<winrt::Windows::Foundation::TimeSpan>(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    template <typename T, auto CTV>
    jsi::Value ConvertComArrayToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::com_array<T>& value)
    {
        const auto size = value.size();
        auto jsArray = jsi::Array(context->Runtime, size);
        for (uint32_t i = 0; i < size; ++i)
        {
            jsArray.setValueAtIndex(context->Runtime, i, CTV(context, value[i]));
        }
        return jsArray;
    }

    template <typename T, auto CVT>
    struct ReadOnlyArrayWrapper final
    {
        ReadOnlyArrayWrapper() = default;

        ReadOnlyArrayWrapper(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
        {
            auto& runtime = context->Runtime;
            const auto array = value.asObject(runtime).asArray(runtime);
            const auto size = array.size(runtime);

            if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>)
            {
                m_nativeArray = winrt::com_array<T>(static_cast<uint32_t>(size), nullptr);
            }
            else
            {
                m_nativeArray = winrt::com_array<T>(static_cast<uint32_t>(size));
            }

            for (uint32_t i = 0; i < size; ++i)
            {
                m_nativeArray[i] = CVT(context, array.getValueAtIndex(runtime, i));
            }
        }

        ReadOnlyArrayWrapper(const ReadOnlyArrayWrapper&) = delete;
        ReadOnlyArrayWrapper(ReadOnlyArrayWrapper&&) = default;

        ~ReadOnlyArrayWrapper() = default;

        operator winrt::array_view<const T>()
        {
            return winrt::array_view<const T>(static_cast<const T*>(m_nativeArray.data()),
                static_cast<const T*>(m_nativeArray.data() + m_nativeArray.size()));
        }

    private:
        winrt::com_array<T> m_nativeArray;
    };

    template <typename T, auto CVT>
    ReadOnlyArrayWrapper<T, CVT> ConvertValueToReadOnlyArrayView(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return { context, value };
    }

    template <typename T, auto CTV>
    struct WriteOnlyArrayWrapper final
    {
        WriteOnlyArrayWrapper() = default;

        WriteOnlyArrayWrapper(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value) :
            m_context(context), m_jsArray(value.asObject(m_context->Runtime).asArray(m_context->Runtime))
        {
            const auto size = m_jsArray->size(m_context->Runtime);
            if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>)
            {
                m_nativeArray = winrt::com_array<T>(static_cast<uint32_t>(size), nullptr);
            }
            else
            {
                m_nativeArray = winrt::com_array<T>(static_cast<uint32_t>(size));
            }
        }

        WriteOnlyArrayWrapper(const WriteOnlyArrayWrapper&) = delete;
        WriteOnlyArrayWrapper(WriteOnlyArrayWrapper&&) = default;

        // TODO: Should an exception here be allowed to propagate, potentially fatally?
        ~WriteOnlyArrayWrapper() noexcept
        try
        {
            const auto size = m_nativeArray.size();
            for (uint32_t i = 0; i < size; ++i)
            {
                m_jsArray->setValueAtIndex(m_context->Runtime, i, CTV(m_context, m_nativeArray[i]));
            }
        }
        CATCH_LOG_RETURN()

        operator winrt::array_view<T>&()
        {
            return m_nativeArray;
        }

    private:
        const std::shared_ptr<ProjectionsContext> m_context;
        std::optional<jsi::Array> m_jsArray;
        winrt::com_array<T> m_nativeArray;
    };

    template <typename T, auto CTV>
    WriteOnlyArrayWrapper<T, CTV> ConvertValueToWriteOnlyArrayView(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return { context, value };
    }

    // Interfaces
    std::optional<jsi::Value> TryConvertPropertyValueToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IPropertyValue& value);
    winrt::Windows::Foundation::IInspectable TryConvertValueToPropertyValue(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    template <typename T>
    jsi::Value ConvertInterfaceToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        if (!value)
        {
            return jsi::Value(nullptr);
        }

        if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IPropertyValue>)
        {
            if (auto convertedValue = TryConvertPropertyValueToValue(context, value))
            {
                return std::move(*convertedValue);
            }
        }
        else if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
        {
            if (const auto propertyValue = value.try_as<winrt::Windows::Foundation::IPropertyValue>())
            {
                if (auto convertedValue = TryConvertPropertyValueToValue(context, propertyValue))
                {
                    return std::move(*convertedValue);
                }
            }
        }

        // The purpose of the explicit QI as IInspectable is to reduce the probability of aliasing where we end up
        // instantiatin multiple instances of ProjectedRuntimeClassInstance rather than reusing cached instances. The
        // lookup is based on the raw ABI pointer as void* but since by definition WinRT COM interface inherit
        // IInspectable (and IUnknown) it might a different vtable entry and a different pointer. So even if T ==
        // winrt::Windows::Foundation::IInspectable we should QI just to be sure because a static_cast of a WinRT COM
        // interface as IInspectable may have been performed pointing to a different vtable entry and giving a different
        // pointer. In most cases a class will always return a particular implementation of IInspectable given use of a
        // single implementation of IUnknown::QueryInterface.
        return ProjectedRuntimeClassInstance::Get(context, value.as<winrt::Windows::Foundation::IInspectable>());
    }

    winrt::Windows::Foundation::IInspectable TryGetInterfaceFromHostObject(
        jsi::Runtime& runtime, const jsi::Value& value);

    template <typename T>
    T ConvertValueToInterface(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        if (value.isNull() || value.isUndefined())
        {
            return nullptr;
        }

        if (const auto inspectable = TryGetInterfaceFromHostObject(context->Runtime, value))
        {
            if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
            {
                return inspectable;
            }
            else if (const auto queried = inspectable.try_as<T>())
            {
                return queried;
            }
            else
            {
                throw jsi::JSError(
                    context->Runtime, std::string("TypeError: Could not convert COM interface to ") + typeid(T).name());
            }
        }

        if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable> ||
                      std::is_same_v<T, winrt::Windows::Foundation::IPropertyValue>)
        {
            if (auto inspectable = TryConvertValueToPropertyValue(context, value))
            {
                if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
                {
                    return inspectable;
                }
                else
                {
                    return inspectable.as<winrt::Windows::Foundation::IPropertyValue>();
                }
            }
        }

        // TODO: Special case allowing JS to pass types like Array for IVector/IVectorView and Map for IMap/IMapView.

        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Cannot derive a WinRT interface for the JS value. Expecting: ") + typeid(T).name());
    }

    // ProjectedAsyncOperation is an imitation of Promise except it also supports some special features like Chakra such
    // as a progress callback and exposing the underlying IAsyncOperation as an "operation" property. It is better than
    // Chakra's as it implements Promise rather than just PromiseLike. That said, to avoid code bloat, the interfaces
    // for the underlying operation are not specialized (exception IAsyncAction since it isn't generic anyway) and
    // exposing them would give the same functionality as the ProjectedAsyncOperation Proimise wrapper. If there is
    // strong reasons for their projection it could be done.
    jsi::Value ConvertAsyncActionToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IAsyncAction& value);

    template <typename A, auto CPV>
    jsi::Value ConvertAsyncActionWithProgressToValue(const std::shared_ptr<ProjectionsContext>& context, const A& value)
    {
        return jsi::Value(context->Runtime, jsi::Object::createFromHostObject(context->Runtime,
                                                std::shared_ptr<jsi::HostObject>(new ProjectedAsyncOperation(context,
                                                    value, NativeToValueConverter<IsAsyncAction>(nullptr), &CPV))));
    }

    template <typename A, auto CTV>
    jsi::Value ConvertAsyncOperationToValue(const std::shared_ptr<ProjectionsContext>& context, const A& value)
    {
        return jsi::Value(
            context->Runtime, jsi::Object::createFromHostObject(context->Runtime,
                                  std::shared_ptr<jsi::HostObject>(new ProjectedAsyncOperation(context, value, &CTV))));
    }

    template <typename A, auto CTV, auto CPV>
    jsi::Value ConvertAsyncOperationWithProgressToValue(
        const std::shared_ptr<ProjectionsContext>& context, const A& value)
    {
        return jsi::Value(context->Runtime,
            jsi::Object::createFromHostObject(context->Runtime,
                std::shared_ptr<jsi::HostObject>(new ProjectedAsyncOperation(context, value, &CTV, &CPV))));
    }

    template <typename I, auto CTV>
    jsi::Value ConvertReferenceToValue(const std::shared_ptr<ProjectionsContext>& context, const I& value)
    {
        if (value)
        {
            return CTV(context, value.Value());
        }
        return jsi::Value(nullptr);
    }

    template <typename I, auto CVT>
    I ConvertValueToReference(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        if (value.isNull() || value.isUndefined())
        {
            return nullptr;
        }
        return winrt::box_value(CVT(context, value)).as<I>();
    }

    template <typename I, auto CTV>
    jsi::Value ConvertReferenceArrayToValue(const std::shared_ptr<ProjectionsContext>& context, const I& value)
    {
        if (value)
        {
            return ConvertComArrayToValue<decltype(std::declval(I).Value()), CTV>(context, value.Value());
        }
        return jsi::Value(nullptr);
    }

    template <typename I, auto CVT>
    I ConvertValueToReferenceArray(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        if (value.isNull() || value.isUndefined())
        {
            return nullptr;
        }

        // It doesn't seem like C++/WinRT actually provides an implementation of IReferenceArray<T> like it does for
        // IReference<T> nor does it even map the basic array types supported by Windows::Foundation::PropertyValue. It
        // can be done but since the public SDK doesn't actually make use of it, perhaps it it is not necessary to
        // implement.
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion to native reference array to JS not implemented for "sv) +
                typeid(I).name());
    }

    // Delegates
    template <typename T>
    jsi::Value ConvertDelegateToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion from native delegate to JS not implemented for "sv) + typeid(T).name());
    }

    template <typename T>
    T ConvertValueToDelegate(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion from JS to native delegate not implemented for "sv) + typeid(T).name());
    }

    // Generic Delegates. These are special-cased because MidlRT special-cases generics so we don't need to generally
    // support code-gen for generic delegates. Some are explicitly omitted below because there is no purpose in
    // implementing the generic async-related delegates when the corresponding generic interfaces are not specialized
    // and included in the interface map.

    template <typename T, auto RC>
    jsi::Value ConvertEventHandlerToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        static auto s_function = ProjectedFunction::Create(
            "invoke"sv, ProjectedFunctionOverload::Create<T>(
                            &T::operator(), &ConvertValueToInterface<winrt::Windows::Foundation::IInspectable>, &RC));
        return jsi::Value(context->Runtime, s_function->GetFunction<T>(value, context));
    }

    template <typename T, auto RC>
    T ConvertValueToEventHandler(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return T([context, function{ value.asObject(context->Runtime).asFunction(context->Runtime) }](
                     const winrt::Windows::Foundation::IInspectable& sender, const auto& args) {
            context->Invoker->CallSync([&]() {
                function.call(context->Runtime,
                    ConvertInterfaceToValue<winrt::Windows::Foundation::IInspectable>(context, sender),
                    RC(context, args));
            });
        });
    }

    template <typename T, auto SC, auto RC>
    jsi::Value ConvertTypedEventHandlerToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        static auto s_function =
            ProjectedFunction::Create("invoke"sv, ProjectedFunctionOverload::Create<T>(&T::operator(), &SC, &RC));
        return jsi::Value(context->Runtime, s_function->GetFunction<T>(value, context));
    }

    template <typename T, auto SC, auto RC>
    T ConvertValueToTypedEventHandler(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return T([context, function{ value.asObject(context->Runtime).asFunction(context->Runtime) }](
                     const auto& sender, const auto& args) {
            context->Invoker->CallSync(
                [&]() { function.call(context->Runtime, SC(context, sender), RC(context, args)); });
        });
    }

    template <typename K, typename V>
    jsi::Value ConvertMapChangedEventHandlerToValue(const std::shared_ptr<ProjectionsContext>& context,
        const winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>& value)
    {
        static auto s_function = ProjectedFunction::Create("invoke"sv,
            ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>(
                &winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>::operator(),
                &ConvertValueToInterface<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>,
                &ConvertValueToInterface<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>));
        return jsi::Value(context->Runtime,
            s_function->GetFunction<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>(
                value, context));
    }

    template <typename K, typename V>
    winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V> ConvertValueToMapChangedEventHandler(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>(
            [context, function{ value.asObject(context->Runtime).asFunction(context->Runtime) }](
                const winrt::Windows::Foundation::Collections::IObservableMap<K, V>& sender,
                const winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>& event) {
                context->Invoker->CallSync([&]() {
                    function.call(context->Runtime,
                        ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>(
                            context, sender),
                        ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>(
                            context, event));
                });
            });
    }

    template <typename T>
    jsi::Value ConvertVectorChangedEventHandlerToValue(const std::shared_ptr<ProjectionsContext>& context,
        const winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>& value)
    {
        static auto s_function = ProjectedFunction::Create("invoke"sv,
            ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>(
                &winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>::operator(),
                &ConvertValueToInterface<winrt::Windows::Foundation::Collections::IObservableVector<T>>,
                &ConvertValueToInterface<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>));
        return jsi::Value(context->Runtime,
            s_function->GetFunction<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>(
                value, context));
    }

    template <typename T>
    winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T> ConvertValueToVectorChangedEventHandler(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>(
            [context, function{ value.asObject(context->Runtime).asFunction(context->Runtime) }](
                const winrt::Windows::Foundation::Collections::IObservableVector<T>& sender,
                const winrt::Windows::Foundation::Collections::IVectorChangedEventArgs& event) {
                context->Invoker->CallSync([&]() {
                    function.call(context->Runtime,
                        ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IObservableVector<T>>(
                            context, sender),
                        ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>(
                            context, event));
                });
            });
    }
}