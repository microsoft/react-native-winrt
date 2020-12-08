
#include "base.h"

#include <rnwinrt/ProjectedValueConverters.g.h>
#include <rnwinrt/Projections.g.h>

namespace WinRTTurboModule
{
    jsi::String CreateString(jsi::Runtime& runtime, std::wstring_view value)
    {
        if (value.empty())
        {
            return jsi::String::createFromAscii(runtime, "");
        }

        // WideCharToMultiByte is requesting the size in utf8 characters required for 'value' without null termination
        // as std::string constructed with a fill will actually allocated 'outputLength + 1' characters and asign the
        // last as the null terminator automatically.

        const auto outputLength = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, value.data(),
            static_cast<int32_t>(value.size()), nullptr /*lpMultiByteStr*/, 0 /*cbMultiByte*/,
            nullptr /*lpDefaultChar*/, nullptr /*lpUsedDefaultChar*/);
        winrt::check_bool(outputLength);

        std::string multibyteString(outputLength, '\0');
        winrt::check_bool(
            ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, value.data(), static_cast<int32_t>(value.size()),
                &multibyteString[0], outputLength, nullptr /*lpDefaultChar*/, nullptr /*lpUsedDefaultChar*/));

        return jsi::String::createFromUtf8(runtime, multibyteString);
    }

    jsi::Value CreateError(jsi::Runtime& runtime, std::string_view message)
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
                [&wrapper](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
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

    CallInvokerWrapper::CallInvokerWrapper(std::shared_ptr<react::CallInvoker> callInvokerWrapper) :
        m_callInvokerWrapper(std::move(callInvokerWrapper)), ThreadId(GetCurrentThreadId())
    {
    }

    void CallInvokerWrapper::Call(std::function<void()> func)
    {
        if (ThreadId == GetCurrentThreadId())
        {
            func();
        }
        else
        {
            m_callInvokerWrapper->invokeAsync(std::move(func));
        }
    }

    void CallInvokerWrapper::CallAsync(std::function<void()> func)
    {
        m_callInvokerWrapper->invokeAsync(std::move(func));
    }

    void CallInvokerWrapper::CallSync(std::function<void()> func)
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

            m_callInvokerWrapper->invokeAsync([event, func{ std::move(func) }]() {
                func();
                event.SetEvent();
            });

            event.wait();
        }
    }
}

namespace WinRTTurboModule
{
    bool ConvertValueToBoolean(const std::shared_ptr<ProjectionsContext>&, const jsi::Value& value)
    {
        if (value.isBool())
        {
            return value.getBool();
        }

        if (value.isNumber())
        {
            return value.getNumber() != 0;
        }

        return !value.isNull() && !value.isUndefined();
    }

    jsi::Value ConvertCharToValue(const std::shared_ptr<ProjectionsContext>& context, const char16_t& value)
    {
        char buffer[8];
        auto bytes = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, reinterpret_cast<PCWSTR>(&value), 1, buffer,
            static_cast<int>(std::size(buffer)), nullptr, nullptr);
        winrt::check_bool(bytes);
        return jsi::String::createFromUtf8(context->Runtime, reinterpret_cast<uint8_t*>(buffer), bytes);
    }

    char16_t ConvertValueToChar(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        const auto stringWide = ConvertValueToString(context, value);
        return stringWide.empty() ? 0 : stringWide[0];
    }

    winrt::hstring ConvertValueToString(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        const auto stringUtf8 = value.asString(context->Runtime).utf8(context->Runtime);
        if (stringUtf8.empty())
        {
            return {};
        }

        // MultiByteToWideChar is requesting the size in wide characters required for 'stringUtf8' without null
        // termination as WindowsPreallocateStringBuffer will actually allocated 'outputLength + 1' characters and asign
        // the last as the null terminator automatically.

        const auto outputLength =
            ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, reinterpret_cast<LPCSTR>(stringUtf8.data()),
                static_cast<int32_t>(stringUtf8.size()), nullptr /*lpWideCharStr*/, 0 /*cchWideChar*/);
        winrt::check_bool(outputLength);

        PWSTR stringRawWide;
        HSTRING_BUFFER stringBuffer;
        winrt::check_hresult(
            ::WindowsPreallocateStringBuffer(static_cast<uint32_t>(outputLength), &stringRawWide, &stringBuffer));

        const auto freeBufferOnError = wil::scope_exit([&]() {
            if (stringRawWide)
            {
                WindowsDeleteStringBuffer(stringBuffer);
            }
        });

        winrt::check_bool(
            ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, reinterpret_cast<LPCSTR>(stringUtf8.data()),
                static_cast<int32_t>(stringUtf8.size()), stringRawWide, outputLength));

        winrt::hstring result;
        winrt::check_hresult(::WindowsPromoteStringBuffer(stringBuffer, wil::put_abi(result)));
        stringRawWide = nullptr;

        return result;
    }

    // Lengths are not null-terminated.
    constexpr uint32_t c_uuidLength = (8 + 1 + 4 + 1 + 4 + 1 + 4 + 1 + 12);
    constexpr uint32_t c_guidLength = 2 + c_uuidLength;

    jsi::Value ConvertGuidToValue(const std::shared_ptr<ProjectionsContext>& context, const winrt::guid& value)
    {
        // StringFromGUID2 formats the output as a GUID rather than a UUID but EdgeHTML would represent a GUID
        // from a WinRT API as a UUID as string without the braces. For consistency, we will do the same here.
        wchar_t guidWide[c_guidLength + 1]{};
        winrt::check_hresult(StringFromGUID2(value, guidWide, ARRAYSIZE(guidWide)));

        // Skip the '{' to extract the UUID.
        const PWSTR uuidStartWide = guidWide + 1;

        char guidUtf8[c_uuidLength + 1]{};
        const auto outputLength = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, uuidStartWide, c_uuidLength,
            guidUtf8, c_uuidLength, L'\0' /*lpDefaultChar*/, nullptr /*lpUsedDefaultChar*/);
        winrt::check_bool(outputLength);
        if (outputLength != c_uuidLength)
        {
            throw winrt::hresult_error(E_UNEXPECTED);
        }

        // The 'guidUtf8' was zero-init'd so it is already null-terminated; It is technically UTF-8 but equivalent to
        // ASCII in this case.
        return jsi::Value(context->Runtime, jsi::String::createFromAscii(context->Runtime, guidUtf8));
    }

    winrt::guid ConvertValueToGuid(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        // IIDFromString calls wIIDFromString which calls wGUIDFromString. It accepts only GUIDs with the braces
        // rather than UUIDs without, but based on how EdgeHTML supported projections, it would convert a guid to
        // string as a UUID. So we need to be able to parse either here.

        auto& runtime = context->Runtime;
        const auto stringUtf8 = value.asString(runtime).utf8(runtime);
        const auto stringLength = stringUtf8.length();
        if ((stringLength != c_uuidLength) && (stringLength != c_guidLength))
        {
            throw jsi::JSError(runtime, "TypeError: Invalid GUID length");
        }

        const PCSTR uuidStartUtf8 = (stringLength == c_uuidLength) ? stringUtf8.data() : stringUtf8.data() + 1;

        wchar_t guidWide[c_guidLength + 1]{};
        const PWSTR uuidStartWide = guidWide + 1;

        const auto outputLength = ::MultiByteToWideChar(
            CP_UTF8, MB_ERR_INVALID_CHARS, uuidStartUtf8, c_uuidLength, uuidStartWide, c_uuidLength);
        winrt::check_bool(outputLength);
        if (outputLength != c_uuidLength)
        {
            throw jsi::JSError(runtime, "TypeError: GUID contains unexpected characters");
        }

        // The 'guidWide' was zero-init'd so it is already null-terminated;
        guidWide[0] = L'{';
        guidWide[ARRAYSIZE(guidWide) - 2] = L'}';

        winrt::guid guid;
        winrt::check_hresult(IIDFromString(guidWide, reinterpret_cast<IID*>(&guid)));

        return guid;
    }

    // TODO: Use std::chrono for conversions rather than using magic numbers directly.
    constexpr int64_t c_jsFileTimeEpochDelta = 116444736 * wil::filetime_duration::one_millisecond;

    template <>
    jsi::Value ConvertStructToValue<winrt::Windows::Foundation::DateTime>(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::DateTime& value)
    {
        auto& runtime = context->Runtime;
        return runtime.global()
            .getPropertyAsFunction(runtime, "Date")
            .callAsConstructor(runtime,
                static_cast<double>(std::min(0ll, static_cast<int64_t>(winrt::clock::to_file_time(value).value) -
                                                      c_jsFileTimeEpochDelta) /
                                    wil::filetime_duration::one_millisecond));
    }

    winrt::Windows::Foundation::DateTime ConvertNumberToDateTime(const jsi::Value& value)
    {
        return winrt::clock::from_file_time(winrt::file_time(static_cast<uint64_t>(
            (static_cast<int64_t>(static_cast<int64_t>(value.asNumber())) * wil::filetime_duration::one_millisecond) +
            c_jsFileTimeEpochDelta)));
    }

    template <>
    winrt::Windows::Foundation::DateTime ConvertValueToStruct<winrt::Windows::Foundation::DateTime>(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        if (value.isNumber())
        {
            return ConvertNumberToDateTime(value);
        }

        auto& runtime = context->Runtime;
        const auto object = value.asObject(runtime);
        return ConvertNumberToDateTime(object.getPropertyAsFunction(runtime, "valueOf").callWithThis(runtime, object));
    }

    template <>
    jsi::Value ConvertStructToValue<winrt::Windows::Foundation::TimeSpan>(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::TimeSpan& value)
    {
        return ConvertNumberToValue<int64_t>(
            context, std::chrono::duration_cast<std::chrono::duration<int64_t, std::milli>>(value).count());
    }

    template <>
    winrt::Windows::Foundation::TimeSpan ConvertValueToStruct<winrt::Windows::Foundation::TimeSpan>(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return std::chrono::duration_cast<winrt::Windows::Foundation::TimeSpan>(
            std::chrono::duration<int64_t, std::milli>(ConvertValueToNumber<int64_t>(context, value)));
    }

    jsi::Value ConvertAsyncActionToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IAsyncAction& value)
    {
        return jsi::Value(
            context->Runtime, jsi::Object::createFromHostObject(context->Runtime,
                                  std::shared_ptr<jsi::HostObject>(new ProjectedAsyncOperation(context, value))));
    }

    std::optional<jsi::Value> TryConvertPropertyValueToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IPropertyValue& value)
    {
        switch (value.Type())
        {
        case winrt::Windows::Foundation::PropertyType::Empty:
            return {};

        case winrt::Windows::Foundation::PropertyType::UInt8:
            return ConvertNumberToValue(context, value.GetUInt8());

        case winrt::Windows::Foundation::PropertyType::Int16:
            return ConvertNumberToValue(context, value.GetInt16());

        case winrt::Windows::Foundation::PropertyType::UInt16:
            return ConvertNumberToValue(context, value.GetUInt16());

        case winrt::Windows::Foundation::PropertyType::Int32:
            return ConvertNumberToValue(context, value.GetInt32());

        case winrt::Windows::Foundation::PropertyType::UInt32:
            return ConvertNumberToValue(context, value.GetUInt32());

        case winrt::Windows::Foundation::PropertyType::Int64:
            return ConvertNumberToValue(context, value.GetInt64());

        case winrt::Windows::Foundation::PropertyType::UInt64:
            return ConvertNumberToValue(context, value.GetUInt64());

        case winrt::Windows::Foundation::PropertyType::Single:
            return ConvertNumberToValue(context, value.GetSingle());

        case winrt::Windows::Foundation::PropertyType::Double:
            return ConvertNumberToValue(context, value.GetDouble());

        case winrt::Windows::Foundation::PropertyType::Char16:
            return ConvertCharToValue(context, value.GetChar16());

        case winrt::Windows::Foundation::PropertyType::Boolean:
            return ConvertBooleanToValue(context, value.GetBoolean());

        case winrt::Windows::Foundation::PropertyType::String:
            return ConvertStringToValue(context, value.GetString());

        case winrt::Windows::Foundation::PropertyType::Inspectable:
            return {};

        case winrt::Windows::Foundation::PropertyType::DateTime:
            return ConvertStructToValue(context, value.GetDateTime());

        case winrt::Windows::Foundation::PropertyType::TimeSpan:
            return ConvertStructToValue(context, value.GetTimeSpan());

        case winrt::Windows::Foundation::PropertyType::Guid:
            return ConvertGuidToValue(context, value.GetGuid());

        case winrt::Windows::Foundation::PropertyType::Point:
            return ConvertStructToValue(context, value.GetPoint());

        case winrt::Windows::Foundation::PropertyType::Size:
            return ConvertStructToValue(context, value.GetSize());

        case winrt::Windows::Foundation::PropertyType::Rect:
            return ConvertStructToValue(context, value.GetRect());

        case winrt::Windows::Foundation::PropertyType::OtherType:
            return {}; // Likely requires using IReference<T>::Value().

        case winrt::Windows::Foundation::PropertyType::UInt8Array: {
            winrt::com_array<uint8_t> array;
            value.GetUInt8Array(array);
            return ConvertComArrayToValue<uint8_t, ConvertNumberToValue<uint8_t>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::Int16Array: {
            winrt::com_array<int16_t> array;
            value.GetInt16Array(array);
            return ConvertComArrayToValue<int16_t, ConvertNumberToValue<int16_t>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::UInt16Array: {
            winrt::com_array<uint16_t> array;
            value.GetUInt16Array(array);
            return ConvertComArrayToValue<uint16_t, ConvertNumberToValue<uint16_t>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::Int32Array: {
            winrt::com_array<int32_t> array;
            value.GetInt32Array(array);
            return ConvertComArrayToValue<int32_t, ConvertNumberToValue<int32_t>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::UInt32Array: {
            winrt::com_array<uint32_t> array;
            value.GetUInt32Array(array);
            return ConvertComArrayToValue<uint32_t, ConvertNumberToValue<uint32_t>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::Int64Array: {
            winrt::com_array<int64_t> array;
            value.GetInt64Array(array);
            return ConvertComArrayToValue<int64_t, ConvertNumberToValue<int64_t>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::UInt64Array: {
            winrt::com_array<uint64_t> array;
            value.GetUInt64Array(array);
            return ConvertComArrayToValue<uint64_t, ConvertNumberToValue<uint64_t>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::SingleArray: {
            winrt::com_array<float> array;
            value.GetSingleArray(array);
            return ConvertComArrayToValue<float, ConvertNumberToValue<float>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::DoubleArray: {
            winrt::com_array<double> array;
            value.GetDoubleArray(array);
            return ConvertComArrayToValue<double, ConvertNumberToValue<double>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::Char16Array: {
            winrt::com_array<char16_t> array;
            value.GetChar16Array(array);
            return ConvertComArrayToValue<char16_t, ConvertCharToValue>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::BooleanArray: {
            winrt::com_array<bool> array;
            value.GetBooleanArray(array);
            return ConvertComArrayToValue<bool, ConvertBooleanToValue>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::StringArray: {
            winrt::com_array<winrt::hstring> array;
            value.GetStringArray(array);
            return ConvertComArrayToValue<winrt::hstring, ConvertStringToValue>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::InspectableArray: {
            winrt::com_array<winrt::Windows::Foundation::IInspectable> array;
            value.GetInspectableArray(array);
            return ConvertComArrayToValue<winrt::Windows::Foundation::IInspectable,
                ConvertInterfaceToValue<winrt::Windows::Foundation::IInspectable>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::DateTimeArray: {
            winrt::com_array<winrt::Windows::Foundation::DateTime> array;
            value.GetDateTimeArray(array);
            return ConvertComArrayToValue<winrt::Windows::Foundation::DateTime,
                ConvertStructToValue<winrt::Windows::Foundation::DateTime>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::TimeSpanArray: {
            winrt::com_array<winrt::Windows::Foundation::TimeSpan> array;
            value.GetTimeSpanArray(array);
            return ConvertComArrayToValue<winrt::Windows::Foundation::TimeSpan,
                ConvertStructToValue<winrt::Windows::Foundation::TimeSpan>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::GuidArray: {
            winrt::com_array<winrt::guid> array;
            value.GetGuidArray(array);
            return ConvertComArrayToValue<winrt::guid, ConvertGuidToValue>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::PointArray: {
            winrt::com_array<winrt::Windows::Foundation::Point> array;
            value.GetPointArray(array);
            return ConvertComArrayToValue<winrt::Windows::Foundation::Point,
                ConvertStructToValue<winrt::Windows::Foundation::Point>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::SizeArray: {
            winrt::com_array<winrt::Windows::Foundation::Size> array;
            value.GetSizeArray(array);
            return ConvertComArrayToValue<winrt::Windows::Foundation::Size,
                ConvertStructToValue<winrt::Windows::Foundation::Size>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::RectArray: {
            winrt::com_array<winrt::Windows::Foundation::Rect> array;
            value.GetRectArray(array);
            return ConvertComArrayToValue<winrt::Windows::Foundation::Rect,
                ConvertStructToValue<winrt::Windows::Foundation::Rect>>(context, array);
        }

        case winrt::Windows::Foundation::PropertyType::OtherTypeArray:
            return {}; // Likely requires using IReferenceArray<T>::Value().

        default:
            FAIL_FAST();
        }
    }

    winrt::Windows::Foundation::IInspectable TryConvertValueToPropertyValue(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        // WARNING: Because of inherent ambiguities between JS and strongly typed ABI (e.g. number versus int64_t), it
        //          is far safer to use the Windows.Foundation.PropertyValue factory to explicitly convert values. It
        //          might be safer to remove the glue code below to better enforce safe practices.
        if (value.isBool())
        {
            return winrt::Windows::Foundation::PropertyValue::CreateBoolean(ConvertValueToBoolean(context, value));
        }
        else if (value.isNumber())
        {
            // Not possible to disambiguate which numeric type is desired.
            return winrt::Windows::Foundation::PropertyValue::CreateDouble(
                ConvertValueToNumber<double>(context, value));
        }
        else if (value.isString())
        {
            // Not possible to differentiate with a GUID because JS represents GUIDs as strings. Similarly Char16 is
            // ambiguously represented as string in JS.
            return winrt::Windows::Foundation::PropertyValue::CreateString(ConvertValueToString(context, value));
        }
        else if (value.isObject())
        {
            auto& runtime = context->Runtime;
            const auto object = value.asObject(runtime);
            if (object.isArray(runtime))
            {
                const auto array = object.asArray(runtime);

                // Can only handle non-empty arrays as there is no way to assume the type otherwise.
                if (array.size(runtime) > 0)
                {
                    const auto element = array.getValueAtIndex(runtime, 0);
                    if (element.isBool())
                    {
                        return winrt::Windows::Foundation::PropertyValue::CreateBooleanArray(
                            ConvertValueToReadOnlyArrayView<bool, ConvertValueToBoolean>(context, value));
                    }
                    else if (element.isNumber())
                    {
                        return winrt::Windows::Foundation::PropertyValue::CreateDoubleArray(
                            ConvertValueToReadOnlyArrayView<double, ConvertValueToNumber<double>>(context, value));
                    }
                    else if (element.isString())
                    {
                        return winrt::Windows::Foundation::PropertyValue::CreateStringArray(
                            ConvertValueToReadOnlyArrayView<winrt::hstring, ConvertValueToString>(context, value));
                    }
                    else if (element.isObject())
                    {
                        const auto elementObject = value.asObject(runtime);
                        if (elementObject.isHostObject(runtime))
                        {
                            return winrt::Windows::Foundation::PropertyValue::CreateInspectableArray(
                                ConvertValueToReadOnlyArrayView<winrt::Windows::Foundation::IInspectable,
                                    ConvertValueToInterface<winrt::Windows::Foundation::IInspectable>>(context, value));
                        }

                        const bool elementHasPoint =
                            elementObject.hasProperty(runtime, "x") && elementObject.hasProperty(runtime, "y");
                        if (elementObject.hasProperty(runtime, "width") && elementObject.hasProperty(runtime, "height"))
                        {
                            if (elementHasPoint)
                            {
                                return winrt::Windows::Foundation::PropertyValue::CreateRectArray(
                                    ConvertValueToReadOnlyArrayView<winrt::Windows::Foundation::Rect,
                                        ConvertValueToStruct<winrt::Windows::Foundation::Rect>>(context, value));
                            }
                            return winrt::Windows::Foundation::PropertyValue::CreateSizeArray(
                                ConvertValueToReadOnlyArrayView<winrt::Windows::Foundation::Size,
                                    ConvertValueToStruct<winrt::Windows::Foundation::Size>>(context, value));
                        }
                        else if (elementHasPoint)
                        {
                            return winrt::Windows::Foundation::PropertyValue::CreatePointArray(
                                ConvertValueToReadOnlyArrayView<winrt::Windows::Foundation::Point,
                                    ConvertValueToStruct<winrt::Windows::Foundation::Point>>(context, value));
                        }
                    }

                    // TODO: Could support DateTime arrays too.
                }
            }
            else
            {
                const bool hasPoint = object.hasProperty(runtime, "x") && object.hasProperty(runtime, "y");
                if (object.hasProperty(runtime, "width") && object.hasProperty(runtime, "height"))
                {
                    if (hasPoint)
                    {
                        return winrt::Windows::Foundation::PropertyValue::CreateRect(
                            ConvertValueToStruct<winrt::Windows::Foundation::Rect>(context, value));
                    }
                    return winrt::Windows::Foundation::PropertyValue::CreateSize(
                        ConvertValueToStruct<winrt::Windows::Foundation::Size>(context, value));
                }
                else if (hasPoint)
                {
                    return winrt::Windows::Foundation::PropertyValue::CreatePoint(
                        ConvertValueToStruct<winrt::Windows::Foundation::Point>(context, value));
                }

                // TODO: Could support DateTime too.
            }
        }

        return nullptr;
    }

    winrt::Windows::Foundation::IInspectable TryGetInterfaceFromHostObject(
        jsi::Runtime& runtime, const jsi::Value& value)
    {
        if (value.isObject())
        {
            const auto object = value.asObject(runtime);
            if (object.isHostObject<ProjectedRuntimeClassInstance>(runtime))
            {
                const auto hostObject = object.getHostObject(runtime);
                if (const auto runtimeClass = std::dynamic_pointer_cast<ProjectedRuntimeClassInstance>(hostObject))
                {
                    return runtimeClass->Instance();
                }
                else if (const auto asyncOperation = std::dynamic_pointer_cast<ProjectedAsyncOperationBase>(hostObject))
                {
                    return asyncOperation->Instance();
                }

                throw jsi::JSError(runtime, "TypeError: Cannot derive a WinRT object of from HostObject");
            }
        }
        return nullptr;
    }
}

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

namespace WinRTTurboModule
{
    ProjectedRuntimeClassInstanceFactory::ProjectedRuntimeClassInstanceFactory(ProjectionsContext& context) :
        m_context(context)
    {
    }

    jsi::Value ProjectedRuntimeClassInstanceFactory::Get(const winrt::Windows::Foundation::IInspectable& instance)
    {
        WI_ASSERT(m_context.Invoker->ThreadId == ::GetCurrentThreadId());

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
        WI_ASSERT(m_context.Invoker->ThreadId == ::GetCurrentThreadId());
        return m_instanceCache.erase(winrt::get_abi(instance));
    }
}

namespace WinRTTurboModule
{
    ProjectedEventRegistrar::ProjectedEventRegistrar(ProjectionsContext& context) : m_context(context)
    {
    }

    std::shared_ptr<ProjectedEventInstance> ProjectedEventRegistrar::Get(
        const winrt::Windows::Foundation::IInspectable& inspectable, const std::shared_ptr<IProjectedEventBase>& event)
    {
        WI_ASSERT(m_context.Invoker->ThreadId == ::GetCurrentThreadId());

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

namespace WinRTTurboModule
{
    std::shared_ptr<IProjectedValueProvider> ProjectedNamespace::Create(
        std::string_view name, std::shared_ptr<ProjectionsContext> context, InitializerFunction initializer)
    {
        return std::make_shared<ProjectedHostObjectProvider>(
            name, std::make_shared<ProjectedNamespace>(name, std::move(context), initializer));
    }

    ProjectedNamespace::ProjectedNamespace(
        std::string_view name, std::shared_ptr<ProjectionsContext> context, InitializerFunction initializer) :
        m_name(name),
        m_context(std::move(context)), m_initializer(initializer)
    {
    }

    jsi::Value ProjectedNamespace::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        EnsureInitialized();

        const auto it = m_propertyMap.find(propName.utf8(runtime));
        if (it != m_propertyMap.end())
        {
            return it->second->Value(runtime);
        }

        return jsi::Value::undefined();
    }

    void ProjectedNamespace::set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value&)
    {
        throw jsi::JSError(runtime,
            "TypeError: Cannot assign to property '" + propName.utf8(runtime) + "' to a projected WinRT namespace");
    }

    std::vector<jsi::PropNameID> ProjectedNamespace::getPropertyNames(jsi::Runtime& runtime)
    {
        EnsureInitialized();

        // PERF: It would be nice if these were copyable but PropNameID is only moveable. It might be possible to store
        // and reuse jsi::String, which might avoid deep copies. Could we return an empty vector like TurboModule given
        // it inherits it from HostObject?
        std::vector<jsi::PropNameID> propNames;
        propNames.reserve(m_propertyMap.size());

        for (const auto& property : m_propertyMap)
        {
            propNames.push_back(CreatePropNameID(runtime, property.first));
        }

        return propNames;
    }

    void ProjectedNamespace::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            const auto providers = m_initializer(m_context);
            m_propertyMap.reserve(providers.size());
            for (auto&& provider : providers)
            {
                m_propertyMap[provider->Name()] = std::move(provider);
            }
            m_isInitialized = true;
        }
    }
}

namespace WinRTTurboModule
{
    std::shared_ptr<IProjectedValueProvider> ProjectedEnum::Create(
        std::string_view name, InitializerFunction initializer)
    {
        return std::make_shared<ProjectedHostObjectProvider>(name, std::make_shared<ProjectedEnum>(name, initializer));
    }

    ProjectedEnum::ProjectedEnum(std::string_view name, InitializerFunction initializer) :
        m_name(name), m_initializer(initializer)
    {
    }

    jsi::Value ProjectedEnum::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        EnsureInitialized();

        const auto it = m_propertyMap.find(propName.utf8(runtime));
        if (it != m_propertyMap.end())
        {
            // jsi::Value uses double for int and float constructors and there doesn't appear to be BigInt support yet.
            return jsi::Value(static_cast<double>(it->second));
        }

        return jsi::Value::undefined();
    }

    void ProjectedEnum::set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value&)
    {
        throw jsi::JSError(runtime, std::string("TypeError: Cannot assign to property '") + propName.utf8(runtime) +
                                        "' to a projected WinRT enum");
    }

    std::vector<jsi::PropNameID> ProjectedEnum::getPropertyNames(jsi::Runtime& runtime)
    {
        // PERF: It would be nice if these were copyable but PropNameID is only moveable. It might be possible to store
        // and reuse jsi::String, which might avoid deep copies. Could we return an empty vector like TurboModule given
        // it inherits it from HostObject?
        std::vector<jsi::PropNameID> propNames;
        propNames.reserve(m_propertyMap.size());

        for (const auto& property : m_propertyMap)
        {
            propNames.push_back(CreatePropNameID(runtime, property.first));
        }

        return propNames;
    }

    void ProjectedEnum::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            m_propertyMap = m_initializer();
            m_isInitialized = true;
        }
    }
}

namespace WinRTTurboModule
{
    // Special helper for use with Object.defineProperty
    class ProjectedPropertyDefinition final : public jsi::HostObject
    {
    public:
        ProjectedPropertyDefinition(std::shared_ptr<IProjectedInterfaceInstance> interfaceInstance,
            std::shared_ptr<IProjectedPropertyBase> property) :
            m_interfaceInstance(std::move(interfaceInstance)),
            m_property(std::move(property))
        {
        }

        virtual ~ProjectedPropertyDefinition() = default;

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override
        {
            const auto propNameUtf8 = propName.utf8(runtime);

            if (!propNameUtf8.compare("get"))
            {
                if (!m_get)
                {
                    m_get = jsi::Function::createFromHostFunction(runtime, propName, 0,
                        [interfaceInstance{ m_interfaceInstance }, property{ m_property }](jsi::Runtime& runtime,
                            const jsi::Value&, const jsi::Value*,
                            size_t) mutable { return interfaceInstance->GetProperty(runtime, property); });
                }
                return jsi::Value(runtime, m_get.value());
            }

            if (!m_property->IsReadonly() && !propNameUtf8.compare("set"))
            {
                if (!m_set)
                {
                    m_set = jsi::Function::createFromHostFunction(runtime, propName, 1,
                        [interfaceInstance{ m_interfaceInstance }, property{ m_property }](jsi::Runtime& runtime,
                            const jsi::Value&, const jsi::Value* args, size_t count) mutable {
                            if (count != 1)
                            {
                                throw jsi::JSError(runtime, "TypeError: Property setter expects 1 argument");
                            }
                            interfaceInstance->SetProperty(runtime, property, args[0]);
                            return jsi::Value::undefined();
                        });
                }
                return jsi::Value(runtime, m_set.value());
            }

            return jsi::Value::undefined();
        }

        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value&) override
        {
            throw jsi::JSError(runtime, std::string("TypeError: Cannot assign to property '") + propName.utf8(runtime) +
                                            "' to a projected property definition");
        }

        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override
        {
            std::vector<jsi::PropNameID> propNames;
            propNames.push_back(jsi::PropNameID::forAscii(runtime, "get"));

            if (!m_property->IsReadonly())
            {
                propNames.push_back(jsi::PropNameID::forAscii(runtime, "set"));
            }

            return propNames;
        }

    private:
        const std::shared_ptr<IProjectedInterfaceInstance> m_interfaceInstance;
        const std::shared_ptr<IProjectedPropertyBase> m_property;
        std::optional<jsi::Function> m_get;
        std::optional<jsi::Function> m_set;
    };

    ProjectedActivationFactory::ProjectedActivationFactory(std::string_view name,
        std::shared_ptr<ProjectionsContext> context, GetActivationFactory getActivationFactory,
        std::set<winrt::guid> factoryInterfaceIds) :
        m_name(name),
        m_context(std::move(context)), m_getActivationFactory(getActivationFactory),
        m_factoryInterfaceIds(std::move(factoryInterfaceIds))
    {
    }

    std::string_view ProjectedActivationFactory::Name() const
    {
        return m_name;
    }

    jsi::Value ProjectedActivationFactory::Value(jsi::Runtime& runtime)
    {
        if (!m_object)
        {
            // Create it directly rather than using "ProjectedRuntimeClassInstance::Get" as the factories/statics do not
            // need to be added to the lookup map given that they are strongly referenced singletons like namespaces.
            auto projectedRuntimeClassInstance =
                std::make_shared<ProjectedRuntimeClassInstance>(m_context, m_getActivationFactory());

            if (m_factoryInterfaceIds.empty())
            {
                // PERF 1: We could theoretically defer construction of the factory until calls to
                //         ProjectedRuntimeClassInstance require it, but for simplicity we can do so now. It migjt
                //         theoretically be possible to also prepopulate IIDs to save on calling GetIIDs if we also had
                //         code generation list the statics.

                // PERF 2: We are intentionally holding a strong ref to it rather than using the caching mechanism used
                //         with instances (see Get(instance)) given that this object is a factory. Arguably weak-ref
                //         based caching would be more memory efficient as the JS GC free references to HostObjects, so
                //         we could switch if memory is more important than CPU usage.

                m_object = jsi::Object::createFromHostObject(runtime, std::move(projectedRuntimeClassInstance));
            }
            else
            {
                // It's not possible with JSI to construct as a function object from both a native function and a
                // HostObject. WinRT runtimeclasses support both constructors and static methods and we need the object
                // to be a function so that it can be called like a constructor, but the static methods need to be
                // treated as properties on the function. Since we can use IHostObject::get for this purpose, they need
                // to be added onto the JSI object itself.
                const auto& interfaceInstances = projectedRuntimeClassInstance->Interfaces();

                std::vector<std::shared_ptr<IProjectedInterfaceInstance>> factoryInterfaceInstances;
                std::vector<std::shared_ptr<IProjectedInterfaceInstance>> staticInterfaceInstances;
                for (const auto& interfaceInstance : interfaceInstances)
                {
                    if (m_factoryInterfaceIds.find(interfaceInstance->Interface().InterfaceId()) ==
                        m_factoryInterfaceIds.end())
                    {
                        staticInterfaceInstances.push_back(interfaceInstance);
                    }
                    else
                    {
                        factoryInterfaceInstances.push_back(interfaceInstance);
                    }
                }

                // Create a function that multiplexes all methods on all factory interfaces as C++/WinRT generates
                // interfaces for factories using the ABI names rather than overloading "CreateInstance".
                m_object =
                    CreateCompositeConstructor(runtime, std::string(m_name), std::move(factoryInterfaceInstances));

                // Exclude "factoryInterfaceInstances" from being injected as properties as there is no point in adding
                // the constructors again using their ABI names given that we are doing all of this to support
                // constructors!
                const auto objectClass = runtime.global().getPropertyAsFunction(runtime, "Object");
                std::optional<jsi::Function> defineProperty;

                std::map<std::string_view, std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>,
                                               std::shared_ptr<ProjectedFunction>>>>
                    methodMap;
                for (auto& staticInterfaceInstance : staticInterfaceInstances)
                {
                    if (const auto& properties = staticInterfaceInstance->Interface().Properties(); !properties.empty())
                    {
                        if (!defineProperty)
                        {
                            defineProperty = objectClass.getPropertyAsFunction(runtime, "defineProperty");
                        }

                        for (auto&& property : properties)
                        {
                            defineProperty->call(runtime, *m_object, CreateString(runtime, property.first),
                                jsi::Value(runtime, jsi::Object::createFromHostObject(
                                                        runtime, std::make_shared<ProjectedPropertyDefinition>(
                                                                     staticInterfaceInstance, property.second))));
                        }
                    }

                    // It is possible that multiple interfaces implement the same method rather than just having
                    // overloads for that method on the same interface. So unlike properties, we need to multiplex the
                    // methods to ensure we call the right overload.
                    for (const auto& method : staticInterfaceInstance->Interface().Methods())
                    {
                        methodMap[method.first].emplace_back(staticInterfaceInstance, method.second);
                    }
                }

                for (auto&& method : methodMap)
                {
                    m_object->setProperty(runtime, CreateString(runtime, method.first),
                        CreateCompositeMethod(runtime, std::string(method.first), std::move(method.second)));
                }

                if (auto addEventListener =
                        projectedRuntimeClassInstance->GetEventListenerFunction(runtime, true /*isAddEventListener*/))
                {
                    m_object->setProperty(runtime, "addEventListener", std::move(*addEventListener));

                    auto removeEventListener =
                        projectedRuntimeClassInstance->GetEventListenerFunction(runtime, false /*isAddEventListener*/);
                    m_object->setProperty(runtime, "removeEventListener", std::move(*removeEventListener));
                }

                objectClass.getPropertyAsFunction(runtime, "seal").call(runtime, *m_object);
            }
        }
        return jsi::Value(runtime, *m_object);
    }
}

namespace WinRTTurboModule
{
    std::shared_ptr<ProjectedFunction> ProjectedFunction::Create(std::string_view name, InitializerFunction initializer)
    {
        return std::make_shared<ProjectedFunction>(name, initializer);
    }

    std::shared_ptr<ProjectedFunction> ProjectedFunction::Create(
        std::string_view name, std::shared_ptr<IProjectedFunctionOverloadBase> implementation)
    {
        return std::make_shared<ProjectedFunction>(name, std::move(implementation));
    }

    ProjectedFunction::ProjectedFunction(std::string_view name, InitializerFunction initializer) :
        m_name(name), m_initializer(initializer)
    {
    }

    ProjectedFunction::ProjectedFunction(
        std::string_view name, std::shared_ptr<IProjectedFunctionOverloadBase> implementation) :
        m_name(name),
        m_initializer(nullptr), m_maxArity(implementation->Arity()), m_isInitialized(true)
    {
        m_overloads.emplace(implementation->Arity(), std::move(implementation));
    }

    std::string_view ProjectedFunction::Name() const noexcept
    {
        return m_name;
    }

    bool ProjectedFunction::CanInvoke(uint16_t arity)
    {
        EnsureInitialized();
        return m_overloads.find(arity) != m_overloads.end();
    }

    std::shared_ptr<IProjectedFunctionOverloadBase>& ProjectedFunction::GetOverload(
        jsi::Runtime& runtime, uint16_t numArgs)
    {
        EnsureInitialized();
        const auto it = m_overloads.find(numArgs);
        if (it == m_overloads.end())
        {
            throw jsi::JSError(runtime, "TypeError: No function overload for " + std::string(m_name) + " exists with " +
                                            std::to_string(numArgs) + " args");
        }
        return it->second;
    }

    void ProjectedFunction::EnsureInitialized()
    {
        if (m_overloads.empty())
        {
            // PERF: Consider optimizing the trivial case of a single declaration with no overloads.
            auto overloads = m_initializer();
            for (auto&& overload : overloads)
            {
                const auto arity = overload->Arity();
                m_maxArity = std::max(m_maxArity, arity);

                // JS only supports overloading by the number of function params so code generation should either
                // have omitted all ambiguous functions or picked one to use. If not we will take the last one.
                WI_ASSERT(m_overloads.find(arity) == m_overloads.end());
                m_overloads[arity] = std::move(overload);
            }
            m_isInitialized = true;
        }
    }
}

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

namespace WinRTTurboModule
{
    std::unordered_map<winrt::guid, std::shared_ptr<ProjectedInterface>> ProjectedInterface::s_interfaceMap;

    std::shared_ptr<ProjectedInterface> ProjectedInterface::Get(const winrt::guid& iid)
    {
        EnsureInterfaceMap();
        const auto it = s_interfaceMap.find(iid);
        return (it == s_interfaceMap.end()) ? nullptr : it->second;
    }

    ProjectedInterface::ProjectedInterface(
        const winrt::guid& iid, InstanceFactory factory, InitializerFunction initializer) :
        m_iid(iid),
        m_factory(factory), m_initializer(initializer)
    {
    }

    const winrt::guid& ProjectedInterface::InterfaceId() const noexcept
    {
        return m_iid;
    }

    std::shared_ptr<ProjectedFunction> ProjectedInterface::FindMethod(std::string_view name)
    {
        const auto methods = Methods();
        const auto it = methods.find(name);
        return it == methods.end() ? nullptr : it->second;
    }

    std::shared_ptr<ProjectedFunction> ProjectedInterface::FindMethod(std::string_view name, uint16_t numArgs)
    {
        auto method = FindMethod(name);
        return method->CanInvoke(numArgs) ? method : nullptr;
    }

    std::shared_ptr<ProjectedFunction> ProjectedInterface::FindMethod(uint16_t numArgs)
    {
        for (const auto& method : Methods())
        {
            if (method.second->CanInvoke(numArgs))
            {
                return method.second;
            }
        }
        return nullptr;
    }

    const std::unordered_map<std::string_view, std::shared_ptr<ProjectedFunction>>& ProjectedInterface::Methods()
    {
        EnsureInitialized();
        return m_methodMap;
    }

    std::shared_ptr<IProjectedPropertyBase> ProjectedInterface::FindProperty(std::string_view name)
    {
        const auto properties = Properties();
        const auto it = properties.find(name);
        return it == properties.end() ? nullptr : it->second;
    }

    const std::unordered_map<std::string_view, std::shared_ptr<IProjectedPropertyBase>>& ProjectedInterface::
        Properties()
    {
        EnsureInitialized();
        return m_propertyMap;
    }

    std::shared_ptr<IProjectedEventBase> ProjectedInterface::FindEvent(std::string_view name)
    {
        const auto events = Events();
        const auto it = events.find(name);
        return it == events.end() ? nullptr : it->second;
    }

    const std::unordered_map<std::string_view, std::shared_ptr<IProjectedEventBase>>& ProjectedInterface::Events()
    {
        EnsureInitialized();
        return m_eventMap;
    }

    std::shared_ptr<IProjectedInterfaceInstance> ProjectedInterface::CreateInstance(
        std::shared_ptr<ProjectionsContext> context, winrt::Windows::Foundation::IInspectable instance)
    {
        return m_factory(std::move(context), std::move(instance), shared_from_this());
    }

    void ProjectedInterface::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            const auto data = m_initializer();

            m_propertyMap.reserve(data.Properties.size());
            for (auto&& property : data.Properties)
            {
                m_propertyMap[property->Name()] = std::move(property);
            }

            m_methodMap.reserve(data.Methods.size());
            for (auto&& method : data.Methods)
            {
                m_methodMap[method->Name()] = std::move(method);
            }

            m_eventMap.reserve(data.Events.size());
            for (auto&& event : data.Events)
            {
                m_eventMap[event->Name()] = std::move(event);
            }

            m_isInitialized = true;
        }
    }

    namespace
    {
        ProjectedInterfaceData InitIActivationFactoryInterface()
        {
            return { {},
                { ProjectedFunction::Create("activateInstance"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::IActivationFactory>(
                        &winrt::Windows::Foundation::IActivationFactory::ActivateInstance<
                            winrt::Windows::Foundation::IInspectable>,
                        ConvertInterfaceToValue<winrt::Windows::Foundation::IInspectable>)) },
                {} };
        }

        // TODO: For the sake of Windows.Storage.Streams.IBufferAccess, IBufferByteAccess should be projected (unlike
        // Chakra) which would make IRandomAccessStream far more usable with JS.
    }

    void ProjectedInterface::EnsureInterfaceMap()
    {
        if (s_interfaceMap.empty())
        {
            auto projectedInterfaces = GetProjectedInterfaces();
            s_interfaceMap.reserve(projectedInterfaces.size() + 1);

            // IActivationFactory is a Classic COM interface and cannot be generated from a winmd,
            s_interfaceMap[__uuidof(::IActivationFactory)] =
                ProjectedInterface::Create<winrt::Windows::Foundation::IActivationFactory>(
                    InitIActivationFactoryInterface);
            for (auto&& projectedInterface : projectedInterfaces)
            {
                s_interfaceMap[projectedInterface->InterfaceId()] = std::move(projectedInterface);
            }
        }
    }
}

namespace WinRTTurboModule
{
    jsi::Value ProjectedRuntimeClassInstance::Get(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IInspectable& instance)
    {
        return context->InstanceFactory.Get(instance);
    }

    ProjectedRuntimeClassInstance::ProjectedRuntimeClassInstance(
        std::shared_ptr<ProjectionsContext> context, winrt::Windows::Foundation::IInspectable instance) :
        m_context(std::move(context)),
        m_instance(std::move(instance))
    {
    }

    ProjectedRuntimeClassInstance::~ProjectedRuntimeClassInstance()
    {
        m_context->InstanceFactory.Remove(m_instance);
    }

    jsi::Value ProjectedRuntimeClassInstance::get(jsi::Runtime& runtime, const jsi::PropNameID& propName)
    {
        EnsureInitialized();

        auto propNameUtf8 = propName.utf8(runtime);

        for (uint32_t i = 0; i < m_interfaceInstances.size(); ++i)
        {
            const auto& interfaceInstance = m_interfaceInstances[i];
            auto& interfaceDef = interfaceInstance->Interface();

            if (const auto property = interfaceDef.FindProperty(propNameUtf8))
            {
                return interfaceInstance->GetProperty(runtime, property);
            }

            // PERF: Would it make sense to cache unique and composite methods after resolution?
            if (auto method = interfaceDef.FindMethod(propNameUtf8))
            {
                // At least one interface has a method with the specified name, but it is possible that an overload with
                // the same name was added in a later instance of the interface so we need to multiplex them as a single
                // function.
                std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>, std::shared_ptr<ProjectedFunction>>>
                    interfaceMethods;
                for (++i; i < m_interfaceInstances.size(); ++i)
                {
                    const auto& overloadInterfaceInstance = m_interfaceInstances[i];
                    if (auto overloadMethod = overloadInterfaceInstance->Interface().FindMethod(propNameUtf8))
                    {
                        interfaceMethods.emplace_back(overloadInterfaceInstance, std::move(overloadMethod));
                    }
                }

                if (interfaceMethods.empty())
                {
                    return interfaceInstance->GetMethod(runtime, method);
                }

                interfaceMethods.emplace_back(interfaceInstance, std::move(method));
                return jsi::Value(
                    runtime, CreateCompositeMethod(runtime, std::move(propNameUtf8), std::move(interfaceMethods)));
            }
        }

        // TODO: Is it necessary to support Chakra's treatment of events like properties in addition to
        //       add/removeEventListener. It seems like a poor pattern to carry forward given that it only allows for a
        //       single handler for an event.

        const bool isAddEventListener = (propNameUtf8 == "addEventListener"sv);
        if (isAddEventListener || (propNameUtf8 == "removeEventListener"sv))
        {
            if (auto function = GetEventListenerFunction(runtime, isAddEventListener))
            {
                return std::move(*function);
            }
        }

        return jsi::Value::undefined();
    }

    void ProjectedRuntimeClassInstance::set(
        jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value)
    {
        EnsureInitialized();

        const auto propNameUtf8 = propName.utf8(runtime);

        for (const auto& interfaceInstance : m_interfaceInstances)
        {
            if (const auto property = interfaceInstance->Interface().FindProperty(propNameUtf8);
                property && !property->IsReadonly())
            {
                interfaceInstance->SetProperty(runtime, property, value);
                break;
            }
        }

        // If no property exists with the given name, then ignore the call rather than throwing. This is more-or-less
        // consistent with EdgeHTML WebView.
    }

    std::vector<jsi::PropNameID> ProjectedRuntimeClassInstance::getPropertyNames(jsi::Runtime& runtime)
    {
        EnsureInitialized();

        std::vector<jsi::PropNameID> propNames;
        bool hasEvents = false;
        for (const auto& interfaceInstance : m_interfaceInstances)
        {
            auto& interfaceDef = interfaceInstance->Interface();
            const auto& methods = interfaceDef.Methods();
            const auto& properties = interfaceDef.Properties();

            propNames.reserve(propNames.size() + methods.size() + properties.size());

            // TODO: Do we care if there are duplicates for methods due to overloading interfaces?
            for (const auto& method : methods)
            {
                propNames.push_back(CreatePropNameID(runtime, method.first));
            }

            for (const auto& property : properties)
            {
                propNames.push_back(CreatePropNameID(runtime, property.first));
            }

            if (!hasEvents && !interfaceDef.Events().empty())
            {
                hasEvents = true;
            }
        }

        if (hasEvents)
        {
            propNames.reserve(propNames.size() + 2);
            propNames.push_back(CreatePropNameID(runtime, "addEventListener"sv));
            propNames.push_back(CreatePropNameID(runtime, "removeEventListener"sv));
        }

        return propNames;
    }

    std::optional<jsi::Value> ProjectedRuntimeClassInstance::GetEventListenerFunction(
        jsi::Runtime& runtime, bool isAddEventListener)
    {
        std::vector<std::shared_ptr<IProjectedInterfaceInstance>> interfaceInstancesWithEvents;
        interfaceInstancesWithEvents.reserve(m_interfaceInstances.size());
        for (const auto& interfaceInstance : m_interfaceInstances)
        {
            if (!interfaceInstance->Interface().Events().empty())
            {
                interfaceInstancesWithEvents.push_back(interfaceInstance);
            }
        }

        if (!interfaceInstancesWithEvents.empty())
        {
            // PERF: Would it make sense to cache event listener functions?
            return jsi::Value(runtime,
                jsi::Function::createFromHostFunction(runtime,
                    CreatePropNameID(runtime, isAddEventListener ? "addEventListener"sv : "removeEventListener"sv),
                    2 /*length*/,
                    [isAddEventListener, interfaceInstances{ std::move(interfaceInstancesWithEvents) }](
                        jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t numArgs) {
                        if (numArgs < 2)
                        {
                            throw jsi::JSError(
                                runtime, "TypeError: add/removeEventListener expect (at least) 2 arguments");
                        }

                        const auto eventName = args[0].asString(runtime).utf8(runtime);

                        for (auto& interfaceInstance : interfaceInstances)
                        {
                            if (const auto event = interfaceInstance->Interface().FindEvent(eventName))
                            {
                                if (isAddEventListener)
                                {
                                    interfaceInstance->AddEventListener(runtime, event, args[1]);
                                }
                                else
                                {
                                    interfaceInstance->RemoveEventListener(runtime, event, args[1]);
                                }
                            }
                        }

                        return jsi::Value::undefined();
                    }));
        }

        return {};
    }

    void ProjectedRuntimeClassInstance::EnsureInitialized()
    {
        if (!m_isInitialized)
        {
            const auto iids = winrt::get_interfaces(m_instance);
            m_interfaceInstances.reserve(iids.size());

            for (const auto& iid : iids)
            {
                if (const auto projectedInterface = ProjectedInterface::Get(iid))
                {
                    m_interfaceInstances.push_back(projectedInterface->CreateInstance(m_context, m_instance));
                }
            }

            m_isInitialized = true;
        }
    }

    jsi::Function CreateCompositeConstructor(jsi::Runtime& runtime, std::string className,
        std::vector<std::shared_ptr<IProjectedInterfaceInstance>> factoryInterfaceInstances)
    {
        // TODO: Does it matter that the function name is arbitrarily set to the runtimeclass name or that the function
        //       length is set to 0. The name is probably better as the class name than the ABI factory method name
        //       which could vary, and length is possible to compute as the maximum of all "overloads" but it doesn't
        //       seem like it would be worthwhile.
        auto id = CreatePropNameID(runtime, className);
        return jsi::Function::createFromHostFunction(runtime, std::move(id), 0 /*length*/,
            [className{ std::move(className) }, factoryInterfaceInstances{ std::move(factoryInterfaceInstances) }](
                jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t numArgs) {
                // C++/WinRT generates factory interfaces so the constructor overloads actually have different names
                // rather than relying on arity to differentiate them. Conveivably, there might be multiple factory
                // interfaces and/or the special ClassicCOM IActivationFactory for the zero-param case. A poorly written
                // API might also have multiple overloads with the same arity as it didn't consider JS. Thus we use
                // TryInvokeAnyMethod rather than TryInvokeMethod.

                for (auto& factoryInterfaceInstance : factoryInterfaceInstances)
                {
                    if (const auto method =
                            factoryInterfaceInstance->Interface().FindMethod(static_cast<uint16_t>(numArgs)))
                    {
                        return factoryInterfaceInstance->InvokeMethod(
                            runtime, method, args, static_cast<uint16_t>(numArgs));
                    }
                }

                throw jsi::JSError(runtime, "TypeError: No constructor overload exists for " + className + " with " +
                                                std::to_string(numArgs) + " args");
            });
    }

    jsi::Value CreateCompositeMethod(jsi::Runtime& runtime, std::string methodName,
        std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>, std::shared_ptr<ProjectedFunction>>>
            interfaceMethods)
    {
        if (interfaceMethods.size() == 1)
        {
            const auto& interfaceMethod = interfaceMethods[0];
            return interfaceMethod.first->GetMethod(runtime, interfaceMethod.second);
        }

        // TODO: Does it matter if we don't bother determining the maximum arity for the sake of the length param on the
        // function?
        auto id = CreatePropNameID(runtime, methodName);
        return jsi::Value(
            runtime, jsi::Function::createFromHostFunction(runtime, std::move(id), 0 /*length*/,
                         [methodName{ std::move(methodName) }, interfaceMethods{ std::move(interfaceMethods) }](
                             jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t numArgs) {
                             for (auto& interfaceMethod : interfaceMethods)
                             {
                                 if (interfaceMethod.second->CanInvoke(static_cast<uint16_t>(numArgs)))
                                 {
                                     return interfaceMethod.first->InvokeMethod(
                                         runtime, interfaceMethod.second, args, static_cast<uint16_t>(numArgs));
                                 }
                             }

                             throw jsi::JSError(runtime, "TypeError: No function overload exists for " + methodName +
                                                             " with " + std::to_string(numArgs) + " args");
                         }));
    }
}

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
            return jsi::Value(runtime, jsi::Function::createFromHostFunction(runtime, propName, 3,
                                           [strongThis{ shared_from_this() }](jsi::Runtime&, const jsi::Value&,
                                               const jsi::Value* args, size_t count) mutable {
                                               const auto undefined = jsi::Value::undefined();
                                               return strongThis->HandleThen(count >= 1 ? args[0] : undefined,
                                                   count >= 2 ? args[1] : undefined, count >= 3 ? args[2] : undefined);
                                           }));
        }
        else if (propNameUtf8 == "done"sv)
        {
            return jsi::Value(runtime, jsi::Function::createFromHostFunction(runtime, propName, 3,
                                           [strongThis{ shared_from_this() }](jsi::Runtime&, const jsi::Value&,
                                               const jsi::Value* args, size_t count) mutable {
                                               const auto undefined = jsi::Value::undefined();
                                               return strongThis->HandleThen(count >= 1 ? args[0] : undefined,
                                                   count >= 2 ? args[1] : undefined, count >= 3 ? args[2] : undefined,
                                                   true /*forDone*/);
                                           }));
        }
        else if (propNameUtf8 == "catch"sv)
        {
            return jsi::Value(
                runtime, jsi::Function::createFromHostFunction(runtime, propName, 1,
                             [strongThis{ shared_from_this() }](
                                 jsi::Runtime&, const jsi::Value&, const jsi::Value* args, size_t count) mutable {
                                 const auto undefined = jsi::Value::undefined();
                                 return strongThis->HandleThen(undefined, count >= 1 ? args[0] : undefined, undefined);
                             }));
        }
        else if (propNameUtf8 == "finally"sv)
        {
            return jsi::Value(
                runtime, jsi::Function::createFromHostFunction(runtime, propName, 2,
                             [strongThis{ shared_from_this() }](jsi::Runtime&, const jsi::Value&,
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

    void ProjectedAsyncOperationBase::set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value&)
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
