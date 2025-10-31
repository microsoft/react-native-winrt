#include "pch.h"

#include "base.h"

#include "Windows.Foundation.g.h"
#include "Windows.Foundation.Collections.g.h"
#include "Windows.Foundation.Metadata.g.h"

#include <winrt/Windows.Foundation.h>

namespace rnwinrt::namespaces::Windows::Foundation
{
    static constexpr const static_projection_data* const children[] = {
        &rnwinrt::classes::Windows::Foundation::Deferral::data,
        &rnwinrt::classes::Windows::Foundation::PropertyValue::data,
        &rnwinrt::enums::Windows::Foundation::AsyncStatus::data,
        &rnwinrt::enums::Windows::Foundation::PropertyType::data,
        &rnwinrt::namespaces::Windows::Foundation::Collections::data,
        &rnwinrt::namespaces::Windows::Foundation::Metadata::data,
    };

    constexpr const static_namespace_data data{ "Foundation"sv, children };
}

namespace rnwinrt::enums::Windows::Foundation::AsyncStatus
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "canceled"sv, 2, "2"sv },
        { "completed"sv, 1, "1"sv },
        { "error"sv, 3, "3"sv },
        { "started"sv, 0, "0"sv },
    };

    constexpr const static_enum_data data{ "AsyncStatus"sv, mappings };
}

namespace rnwinrt::enums::Windows::Foundation::PropertyType
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "empty"sv, 0, "0"sv },
        { "uInt8"sv, 1, "1"sv },
        { "int16"sv, 2, "2"sv },
        { "uInt16"sv, 3, "3"sv },
        { "int32"sv, 4, "4"sv },
        { "uInt32"sv, 5, "5"sv },
        { "int64"sv, 6, "6"sv },
        { "uInt64"sv, 7, "7"sv },
        { "single"sv, 8, "8"sv },
        { "double"sv, 9, "9"sv },
        { "char16"sv, 10, "10"sv },
        { "boolean"sv, 11, "11"sv },
        { "string"sv, 12, "12"sv },
        { "inspectable"sv, 13, "13"sv },
        { "dateTime"sv, 14, "14"sv },
        { "timeSpan"sv, 15, "15"sv },
        { "guid"sv, 16, "16"sv },
        { "point"sv, 17, "17"sv },
        { "size"sv, 18, "18"sv },
        { "rect"sv, 19, "19"sv },
        { "otherType"sv, 20, "20"sv },
        { "uInt8Array"sv, 1025, "1025"sv },
        { "int16Array"sv, 1026, "1026"sv },
        { "uInt16Array"sv, 1027, "1027"sv },
        { "int32Array"sv, 1028, "1028"sv },
        { "uInt32Array"sv, 1029, "1029"sv },
        { "int64Array"sv, 1030, "1030"sv },
        { "uInt64Array"sv, 1031, "1031"sv },
        { "singleArray"sv, 1032, "1032"sv },
        { "doubleArray"sv, 1033, "1033"sv },
        { "char16Array"sv, 1034, "1034"sv },
        { "booleanArray"sv, 1035, "1035"sv },
        { "stringArray"sv, 1036, "1036"sv },
        { "inspectableArray"sv, 1037, "1037"sv },
        { "dateTimeArray"sv, 1038, "1038"sv },
        { "timeSpanArray"sv, 1039, "1039"sv },
        { "guidArray"sv, 1040, "1040"sv },
        { "pointArray"sv, 1041, "1041"sv },
        { "sizeArray"sv, 1042, "1042"sv },
        { "rectArray"sv, 1043, "1043"sv },
        { "otherTypeArray"sv, 1044, "1044"sv },
    };

    constexpr const static_enum_data data{ "PropertyType"sv, mappings };
}

namespace rnwinrt::classes::Windows::Foundation::Deferral
{
    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 1)
        {
            auto arg0 = convert_value_to_native<winrt::Windows::Foundation::DeferralCompletedHandler>(runtime, args[0]);
            return convert_native_to_value(runtime, winrt::Windows::Foundation::Deferral(arg0));
        }
        throw_no_constructor(runtime, "Windows.Foundation"sv, "Deferral"sv, count);
    }

    constexpr const static_activatable_class_data data{ "Windows.Foundation"sv, "Deferral"sv, constructor_function, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Foundation::PropertyValue
{
    static constexpr const static_class_data::function_mapping function_data[] = {
        { "createBoolean",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateBoolean(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createBoolean"sv, count);
            }
        },
        { "createBooleanArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateBooleanArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createBooleanArray"sv, count);
            }
        },
        { "createChar16",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateChar16(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createChar16"sv, count);
            }
        },
        { "createChar16Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const char16_t>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateChar16Array(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createChar16Array"sv, count);
            }
        },
        { "createDateTime",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::DateTime>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateDateTime(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createDateTime"sv, count);
            }
        },
        { "createDateTimeArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::DateTime>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateDateTimeArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createDateTimeArray"sv, count);
            }
        },
        { "createDouble",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<double>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateDouble(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createDouble"sv, count);
            }
        },
        { "createDoubleArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const double>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateDoubleArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createDoubleArray"sv, count);
            }
        },
        { "createEmpty",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateEmpty();
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createEmpty"sv, count);
            }
        },
        { "createGuid",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::guid>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateGuid(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createGuid"sv, count);
            }
        },
        { "createGuidArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::guid>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateGuidArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createGuidArray"sv, count);
            }
        },
        { "createInspectable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IInspectable>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateInspectable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createInspectable"sv, count);
            }
        },
        { "createInspectableArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IInspectable>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateInspectableArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createInspectableArray"sv, count);
            }
        },
        { "createInt16",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int16_t>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateInt16(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createInt16"sv, count);
            }
        },
        { "createInt16Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int16_t>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateInt16Array(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createInt16Array"sv, count);
            }
        },
        { "createInt32",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateInt32(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createInt32"sv, count);
            }
        },
        { "createInt32Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateInt32Array(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createInt32Array"sv, count);
            }
        },
        { "createInt64",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int64_t>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateInt64(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createInt64"sv, count);
            }
        },
        { "createInt64Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int64_t>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateInt64Array(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createInt64Array"sv, count);
            }
        },
        { "createPoint",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Point>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreatePoint(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createPoint"sv, count);
            }
        },
        { "createPointArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::Point>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreatePointArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createPointArray"sv, count);
            }
        },
        { "createRect",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Rect>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateRect(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createRect"sv, count);
            }
        },
        { "createRectArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::Rect>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateRectArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createRectArray"sv, count);
            }
        },
        { "createSingle",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<float>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateSingle(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createSingle"sv, count);
            }
        },
        { "createSingleArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const float>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateSingleArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createSingleArray"sv, count);
            }
        },
        { "createSize",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Size>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateSize(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createSize"sv, count);
            }
        },
        { "createSizeArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::Size>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateSizeArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createSizeArray"sv, count);
            }
        },
        { "createString",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateString(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createString"sv, count);
            }
        },
        { "createStringArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateStringArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createStringArray"sv, count);
            }
        },
        { "createTimeSpan",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::TimeSpan>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateTimeSpan(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createTimeSpan"sv, count);
            }
        },
        { "createTimeSpanArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::TimeSpan>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateTimeSpanArray(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createTimeSpanArray"sv, count);
            }
        },
        { "createUInt16",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<uint16_t>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateUInt16(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createUInt16"sv, count);
            }
        },
        { "createUInt16Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const uint16_t>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateUInt16Array(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createUInt16Array"sv, count);
            }
        },
        { "createUInt32",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<uint32_t>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateUInt32(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createUInt32"sv, count);
            }
        },
        { "createUInt32Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const uint32_t>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateUInt32Array(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createUInt32Array"sv, count);
            }
        },
        { "createUInt64",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<uint64_t>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateUInt64(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createUInt64"sv, count);
            }
        },
        { "createUInt64Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const uint64_t>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateUInt64Array(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createUInt64Array"sv, count);
            }
        },
        { "createUInt8",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<uint8_t>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateUInt8(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createUInt8"sv, count);
            }
        },
        { "createUInt8Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const uint8_t>>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::PropertyValue::CreateUInt8Array(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation"sv, "PropertyValue"sv, "createUInt8Array"sv, count);
            }
        },
    };

    constexpr const static_class_data data{ "PropertyValue"sv, {}, {}, function_data };
}

namespace rnwinrt::interfaces::Windows::Foundation::IAsyncInfo
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "errorCode",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Foundation::IAsyncInfo>().ErrorCode());
            },
            nullptr
        },
        { "id",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Foundation::IAsyncInfo>().Id());
            },
            nullptr
        },
        { "status",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Foundation::IAsyncInfo>().Status());
            },
            nullptr
        },
    };
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "cancel",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Foundation::IAsyncInfo>().Cancel();
                    return jsi::Value::undefined();
                },
                0, false },
        { "close",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Foundation::IAsyncInfo>().Close();
                    return jsi::Value::undefined();
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Foundation::IAsyncInfo>(), property_data, {}, function_data };
}

namespace rnwinrt::interfaces::Windows::Foundation::IClosable
{
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "close",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Foundation::IClosable>().Close();
                    return jsi::Value::undefined();
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Foundation::IClosable>(), {}, {}, function_data };
}

namespace rnwinrt::interfaces::Windows::Foundation::IDeferral
{
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "complete",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    thisValue.as<winrt::Windows::Foundation::IDeferral>().Complete();
                    return jsi::Value::undefined();
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Foundation::IDeferral>(), {}, {}, function_data };
}

namespace rnwinrt::interfaces::Windows::Foundation::IPropertyValue
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "isNumericScalar",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Foundation::IPropertyValue>().IsNumericScalar());
            },
            nullptr
        },
        { "type",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Foundation::IPropertyValue>().Type());
            },
            nullptr
        },
    };
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "getBoolean",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetBoolean();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getBooleanArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<bool> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetBooleanArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getChar16",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetChar16();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getChar16Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<char16_t> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetChar16Array(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getDateTime",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetDateTime();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getDateTimeArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<winrt::Windows::Foundation::DateTime> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetDateTimeArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getDouble",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetDouble();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getDoubleArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<double> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetDoubleArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getGuid",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetGuid();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getGuidArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<winrt::guid> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetGuidArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getInspectableArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<winrt::Windows::Foundation::IInspectable> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetInspectableArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getInt16",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetInt16();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getInt16Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<int16_t> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetInt16Array(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getInt32",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetInt32();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getInt32Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<int32_t> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetInt32Array(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getInt64",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetInt64();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getInt64Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<int64_t> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetInt64Array(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getPoint",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetPoint();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getPointArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<winrt::Windows::Foundation::Point> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetPointArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getRect",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetRect();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getRectArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<winrt::Windows::Foundation::Rect> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetRectArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getSingle",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetSingle();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getSingleArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<float> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetSingleArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getSize",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetSize();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getSizeArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<winrt::Windows::Foundation::Size> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetSizeArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getString",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetString();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getStringArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<winrt::hstring> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetStringArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getTimeSpan",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetTimeSpan();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getTimeSpanArray",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<winrt::Windows::Foundation::TimeSpan> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetTimeSpanArray(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getUInt16",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetUInt16();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getUInt16Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<uint16_t> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetUInt16Array(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getUInt32",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetUInt32();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getUInt32Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<uint32_t> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetUInt32Array(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getUInt64",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetUInt64();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getUInt64Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<uint64_t> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetUInt64Array(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
        { "getUInt8",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetUInt8();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "getUInt8Array",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    winrt::com_array<uint8_t> arg0;
                    thisValue.as<winrt::Windows::Foundation::IPropertyValue>().GetUInt8Array(arg0);
                    return make_void_return_struct(runtime, "value", arg0);
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Foundation::IPropertyValue>(), property_data, {}, function_data };
}

namespace rnwinrt::interfaces::Windows::Foundation::IStringable
{
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "toString",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::Windows::Foundation::IStringable>().ToString();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Foundation::IStringable>(), {}, {}, function_data };
}

namespace rnwinrt
{
    jsi::Value projected_value_traits<winrt::Windows::Foundation::Point>::as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::Point& value)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "x", convert_native_to_value(runtime, value.X));
        result.setProperty(runtime, "y", convert_native_to_value(runtime, value.Y));
        return result;
    }

    winrt::Windows::Foundation::Point projected_value_traits<winrt::Windows::Foundation::Point>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::Windows::Foundation::Point result{};
        auto obj = value.asObject(runtime);
        if (auto field = obj.getProperty(runtime, "x"); !field.isUndefined())
            result.X = convert_value_to_native<float>(runtime, field);
        if (auto field = obj.getProperty(runtime, "y"); !field.isUndefined())
            result.Y = convert_value_to_native<float>(runtime, field);
        return result;
    }

    jsi::Value projected_value_traits<winrt::Windows::Foundation::Rect>::as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::Rect& value)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "x", convert_native_to_value(runtime, value.X));
        result.setProperty(runtime, "y", convert_native_to_value(runtime, value.Y));
        result.setProperty(runtime, "width", convert_native_to_value(runtime, value.Width));
        result.setProperty(runtime, "height", convert_native_to_value(runtime, value.Height));
        return result;
    }

    winrt::Windows::Foundation::Rect projected_value_traits<winrt::Windows::Foundation::Rect>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::Windows::Foundation::Rect result{};
        auto obj = value.asObject(runtime);
        if (auto field = obj.getProperty(runtime, "x"); !field.isUndefined())
            result.X = convert_value_to_native<float>(runtime, field);
        if (auto field = obj.getProperty(runtime, "y"); !field.isUndefined())
            result.Y = convert_value_to_native<float>(runtime, field);
        if (auto field = obj.getProperty(runtime, "width"); !field.isUndefined())
            result.Width = convert_value_to_native<float>(runtime, field);
        if (auto field = obj.getProperty(runtime, "height"); !field.isUndefined())
            result.Height = convert_value_to_native<float>(runtime, field);
        return result;
    }

    jsi::Value projected_value_traits<winrt::Windows::Foundation::Size>::as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::Size& value)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "width", convert_native_to_value(runtime, value.Width));
        result.setProperty(runtime, "height", convert_native_to_value(runtime, value.Height));
        return result;
    }

    winrt::Windows::Foundation::Size projected_value_traits<winrt::Windows::Foundation::Size>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::Windows::Foundation::Size result{};
        auto obj = value.asObject(runtime);
        if (auto field = obj.getProperty(runtime, "width"); !field.isUndefined())
            result.Width = convert_value_to_native<float>(runtime, field);
        if (auto field = obj.getProperty(runtime, "height"); !field.isUndefined())
            result.Height = convert_value_to_native<float>(runtime, field);
        return result;
    }

    jsi::Value projected_value_traits<winrt::Windows::Foundation::AsyncActionCompletedHandler>::as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::AsyncActionCompletedHandler& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "AsyncActionCompletedHandler"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "Windows.Foundation"sv, "AsyncActionCompletedHandler"sv);
                }

                auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IAsyncAction>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::Windows::Foundation::AsyncStatus>(runtime, args[1]);
                value(arg0, arg1);
                return jsi::Value::undefined();
            });
    }

    winrt::Windows::Foundation::AsyncActionCompletedHandler projected_value_traits<winrt::Windows::Foundation::AsyncActionCompletedHandler>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::Windows::Foundation::IAsyncAction const& param0, winrt::Windows::Foundation::AsyncStatus param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                fn.call(runtime, arg0, arg1);
            });
        };
    }

    jsi::Value projected_value_traits<winrt::Windows::Foundation::DeferralCompletedHandler>::as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::DeferralCompletedHandler& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "DeferralCompletedHandler"), 0,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 0)
                {
                    throw_invalid_delegate_arg_count(runtime, "Windows.Foundation"sv, "DeferralCompletedHandler"sv);
                }

                value();
                return jsi::Value::undefined();
            });
    }

    winrt::Windows::Foundation::DeferralCompletedHandler projected_value_traits<winrt::Windows::Foundation::DeferralCompletedHandler>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)]() {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                fn.call(runtime);
            });
        };
    }
}
