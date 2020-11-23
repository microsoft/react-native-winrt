#include "pch.h"

#include "ProjectedRuntimeClassInstance.h"
#include "ProjectedValueConverters.h"
#include <rnwinrt\ProjectedValueConverters.g.h>

namespace WinRTTurboModule
{
    bool ConvertValueToBoolean(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
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
        std::string multibyteString(8, '\0');
        winrt::check_bool(::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, reinterpret_cast<PCWSTR>(&value), 1,
            &multibyteString[0], static_cast<int32_t>(multibyteString.size()), L'\0' /*lpDefaultChar*/,
            nullptr /*lpUsedDefaultChar*/));
        return jsi::String::createFromUtf8(context->Runtime, multibyteString);
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