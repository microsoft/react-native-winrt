#include "pch.h"

#include "base.h"

#include "Windows.Foundation.Metadata.g.h"

#include <winrt/Windows.Foundation.Metadata.h>

namespace rnwinrt::namespaces::Windows::Foundation::Metadata
{
    static constexpr const static_projection_data* const children[] = {
        &rnwinrt::classes::Windows::Foundation::Metadata::ApiInformation::data,
        &rnwinrt::enums::Windows::Foundation::Metadata::AttributeTargets::data,
        &rnwinrt::enums::Windows::Foundation::Metadata::CompositionType::data,
        &rnwinrt::enums::Windows::Foundation::Metadata::DeprecationType::data,
        &rnwinrt::enums::Windows::Foundation::Metadata::FeatureStage::data,
        &rnwinrt::enums::Windows::Foundation::Metadata::GCPressureAmount::data,
        &rnwinrt::enums::Windows::Foundation::Metadata::MarshalingType::data,
        &rnwinrt::enums::Windows::Foundation::Metadata::Platform::data,
        &rnwinrt::enums::Windows::Foundation::Metadata::ThreadingModel::data,
    };

    constexpr const static_namespace_data data{ "Metadata"sv, children };
}

namespace rnwinrt::enums::Windows::Foundation::Metadata::AttributeTargets
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "all"sv, 4294967295, "4294967295"sv },
        { "delegate"sv, 1, "1"sv },
        { "enum"sv, 2, "2"sv },
        { "event"sv, 4, "4"sv },
        { "field"sv, 8, "8"sv },
        { "interface"sv, 16, "16"sv },
        { "method"sv, 64, "64"sv },
        { "parameter"sv, 128, "128"sv },
        { "property"sv, 256, "256"sv },
        { "runtimeClass"sv, 512, "512"sv },
        { "struct"sv, 1024, "1024"sv },
        { "interfaceImpl"sv, 2048, "2048"sv },
        { "apiContract"sv, 8192, "8192"sv },
    };

    constexpr const static_enum_data data{ "AttributeTargets"sv, mappings };
}

namespace rnwinrt::enums::Windows::Foundation::Metadata::CompositionType
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "protected"sv, 1, "1"sv },
        { "public"sv, 2, "2"sv },
    };

    constexpr const static_enum_data data{ "CompositionType"sv, mappings };
}

namespace rnwinrt::enums::Windows::Foundation::Metadata::DeprecationType
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "deprecate"sv, 0, "0"sv },
        { "remove"sv, 1, "1"sv },
    };

    constexpr const static_enum_data data{ "DeprecationType"sv, mappings };
}

namespace rnwinrt::enums::Windows::Foundation::Metadata::FeatureStage
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "alwaysDisabled"sv, 0, "0"sv },
        { "disabledByDefault"sv, 1, "1"sv },
        { "enabledByDefault"sv, 2, "2"sv },
        { "alwaysEnabled"sv, 3, "3"sv },
    };

    constexpr const static_enum_data data{ "FeatureStage"sv, mappings };
}

namespace rnwinrt::enums::Windows::Foundation::Metadata::GCPressureAmount
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "low"sv, 0, "0"sv },
        { "medium"sv, 1, "1"sv },
        { "high"sv, 2, "2"sv },
    };

    constexpr const static_enum_data data{ "GCPressureAmount"sv, mappings };
}

namespace rnwinrt::enums::Windows::Foundation::Metadata::MarshalingType
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "none"sv, 1, "1"sv },
        { "agile"sv, 2, "2"sv },
        { "standard"sv, 3, "3"sv },
        { "invalidMarshaling"sv, 0, "0"sv },
    };

    constexpr const static_enum_data data{ "MarshalingType"sv, mappings };
}

namespace rnwinrt::enums::Windows::Foundation::Metadata::Platform
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "windows"sv, 0, "0"sv },
        { "windowsPhone"sv, 1, "1"sv },
    };

    constexpr const static_enum_data data{ "Platform"sv, mappings };
}

namespace rnwinrt::enums::Windows::Foundation::Metadata::ThreadingModel
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "sTA"sv, 1, "1"sv },
        { "mTA"sv, 2, "2"sv },
        { "both"sv, 3, "3"sv },
        { "invalidThreading"sv, 0, "0"sv },
    };

    constexpr const static_enum_data data{ "ThreadingModel"sv, mappings };
}

namespace rnwinrt::classes::Windows::Foundation::Metadata::ApiInformation
{
    static constexpr const static_class_data::function_mapping function_data[] = {
        { "isApiContractPresent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<uint16_t>(runtime, args[1]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                if (count == 3)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<uint16_t>(runtime, args[1]);
                    auto arg2 = convert_value_to_native<uint16_t>(runtime, args[2]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(arg0, arg1, arg2);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation.Metadata"sv, "ApiInformation"sv, "isApiContractPresent"sv, count);
            }
        },
        { "isEnumNamedValuePresent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsEnumNamedValuePresent(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation.Metadata"sv, "ApiInformation"sv, "isEnumNamedValuePresent"sv, count);
            }
        },
        { "isEventPresent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsEventPresent(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation.Metadata"sv, "ApiInformation"sv, "isEventPresent"sv, count);
            }
        },
        { "isMethodPresent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                if (count == 3)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto arg2 = convert_value_to_native<uint32_t>(runtime, args[2]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsMethodPresent(arg0, arg1, arg2);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation.Metadata"sv, "ApiInformation"sv, "isMethodPresent"sv, count);
            }
        },
        { "isPropertyPresent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsPropertyPresent(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation.Metadata"sv, "ApiInformation"sv, "isPropertyPresent"sv, count);
            }
        },
        { "isReadOnlyPropertyPresent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsReadOnlyPropertyPresent(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation.Metadata"sv, "ApiInformation"sv, "isReadOnlyPropertyPresent"sv, count);
            }
        },
        { "isTypePresent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsTypePresent(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation.Metadata"sv, "ApiInformation"sv, "isTypePresent"sv, count);
            }
        },
        { "isWriteablePropertyPresent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::Windows::Foundation::Metadata::ApiInformation::IsWriteablePropertyPresent(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "Windows.Foundation.Metadata"sv, "ApiInformation"sv, "isWriteablePropertyPresent"sv, count);
            }
        },
    };

    constexpr const static_class_data data{ "ApiInformation"sv, {}, {}, function_data };
}
