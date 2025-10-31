#include "pch.h"

#include "base.h"

#include "TestComponent.g.h"

#include <winrt/TestComponent.h>

namespace rnwinrt::namespaces::TestComponent
{
    static constexpr const static_projection_data* const children[] = {
        &rnwinrt::classes::TestComponent::HierarchyBase::data,
        &rnwinrt::classes::TestComponent::HierarchyDerived::data,
        &rnwinrt::classes::TestComponent::StaticOnlyTest::data,
        &rnwinrt::classes::TestComponent::Test::data,
        &rnwinrt::classes::TestComponent::TestObject::data,
        &rnwinrt::enums::TestComponent::TestEnum::data,
    };

    constexpr const static_namespace_data data{ "TestComponent"sv, children };
}

namespace rnwinrt::enums::TestComponent::TestEnum
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "first"sv, 1, "1"sv },
        { "second"sv, 2, "2"sv },
        { "third"sv, 3, "3"sv },
        { "fourth"sv, 4, "4"sv },
    };

    constexpr const static_enum_data data{ "TestEnum"sv, mappings };
}

namespace rnwinrt::classes::TestComponent::HierarchyBase
{
    static constexpr const static_class_data::function_mapping function_data[] = {
        { "staticHierarchyBaseMethod",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::TestComponent::HierarchyBase::StaticHierarchyBaseMethod();
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "HierarchyBase"sv, "staticHierarchyBaseMethod"sv, count);
            }
        },
    };

    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 0)
        {
            return convert_native_to_value(runtime, winrt::TestComponent::HierarchyBase());
        }
        throw_no_constructor(runtime, "TestComponent"sv, "HierarchyBase"sv, count);
    }

    constexpr const static_activatable_class_data data{ "TestComponent"sv, "HierarchyBase"sv, constructor_function, {}, {}, function_data };
}

namespace rnwinrt::classes::TestComponent::HierarchyDerived
{
    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 1)
        {
            auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
            return convert_native_to_value(runtime, winrt::TestComponent::HierarchyDerived(arg0));
        }
        throw_no_constructor(runtime, "TestComponent"sv, "HierarchyDerived"sv, count);
    }

    constexpr const static_activatable_class_data data{ "TestComponent"sv, "HierarchyDerived"sv, constructor_function, {}, {}, {} };
}

namespace rnwinrt::classes::TestComponent::StaticOnlyTest
{
    static constexpr const static_class_data::property_mapping property_data[] = {
        { "boolProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::BoolProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::BoolProperty(convert_value_to_native<bool>(runtime, value));
            },
        },
        { "booleansStructProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::BooleansStructProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::BooleansStructProperty(convert_value_to_native<winrt::TestComponent::BooleanTypes>(runtime, value));
            },
        },
        { "charProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::CharProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::CharProperty(convert_value_to_native<char16_t>(runtime, value));
            },
        },
        { "compositeStructProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::CompositeStructProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::CompositeStructProperty(convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, value));
            },
        },
        { "enumProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::EnumProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::EnumProperty(convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, value));
            },
        },
        { "f32Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::F32Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::F32Property(convert_value_to_native<float>(runtime, value));
            },
        },
        { "f64Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::F64Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::F64Property(convert_value_to_native<double>(runtime, value));
            },
        },
        { "guidProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::GuidProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::GuidProperty(convert_value_to_native<winrt::guid>(runtime, value));
            },
        },
        { "hResultProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::HResultProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::HResultProperty(convert_value_to_native<winrt::hresult>(runtime, value));
            },
        },
        { "numericsStructProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::NumericsStructProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::NumericsStructProperty(convert_value_to_native<winrt::TestComponent::NumericTypes>(runtime, value));
            },
        },
        { "objectProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::ObjectProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::ObjectProperty(convert_value_to_native<winrt::TestComponent::TestObject>(runtime, value));
            },
        },
        { "refBooleanProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::RefBooleanProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::RefBooleanProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<bool>>(runtime, value));
            },
        },
        { "refCharProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::RefCharProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::RefCharProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<char16_t>>(runtime, value));
            },
        },
        { "refEnumProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::RefEnumProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::RefEnumProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<winrt::TestComponent::TestEnum>>(runtime, value));
            },
        },
        { "refNumericProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::RefNumericProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::RefNumericProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, value));
            },
        },
        { "s16Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::S16Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::S16Property(convert_value_to_native<int16_t>(runtime, value));
            },
        },
        { "s32Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::S32Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::S32Property(convert_value_to_native<int32_t>(runtime, value));
            },
        },
        { "s64Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::S64Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::S64Property(convert_value_to_native<int64_t>(runtime, value));
            },
        },
        { "stringProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::StringProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::StringProperty(convert_value_to_native<winrt::hstring>(runtime, value));
            },
        },
        { "stringsStructProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::StringsStructProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::StringsStructProperty(convert_value_to_native<winrt::TestComponent::StringTypes>(runtime, value));
            },
        },
        { "u16Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::U16Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::U16Property(convert_value_to_native<uint16_t>(runtime, value));
            },
        },
        { "u32Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::U32Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::U32Property(convert_value_to_native<uint32_t>(runtime, value));
            },
        },
        { "u64Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::U64Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::U64Property(convert_value_to_native<uint64_t>(runtime, value));
            },
        },
        { "u8Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::StaticOnlyTest::U8Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::StaticOnlyTest::U8Property(convert_value_to_native<uint8_t>(runtime, value));
            },
        },
    };

    static constexpr const static_class_data::event_mapping event_data[] = {
        { "objecteventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::StaticOnlyTest::ObjectEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<winrt::TestComponent::TestObject>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::StaticOnlyTest::ObjectEventHandler(token);
            }
        },
    };

    static constexpr const static_class_data::function_mapping function_data[] = {
        { "copyString",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto result = winrt::TestComponent::StaticOnlyTest::CopyString(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "StaticOnlyTest"sv, "copyString"sv, count);
            }
        },
        { "raiseObjectEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, args[0]);
                    winrt::TestComponent::StaticOnlyTest::RaiseObjectEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "StaticOnlyTest"sv, "raiseObjectEvent"sv, count);
            }
        },
    };

    constexpr const static_class_data data{ "StaticOnlyTest"sv, property_data, event_data, function_data };
}

namespace rnwinrt::classes::TestComponent::Test
{
    static constexpr const static_class_data::property_mapping property_data[] = {
        { "staticBoolProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticBoolProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticBoolProperty(convert_value_to_native<bool>(runtime, value));
            },
        },
        { "staticBooleanArrayProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticBooleanArrayProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticBooleanArrayProperty(convert_value_to_native<winrt::array_view<const bool>>(runtime, value));
            },
        },
        { "staticBooleansStructProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticBooleansStructProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticBooleansStructProperty(convert_value_to_native<winrt::TestComponent::BooleanTypes>(runtime, value));
            },
        },
        { "staticCharArrayProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticCharArrayProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticCharArrayProperty(convert_value_to_native<winrt::array_view<const char16_t>>(runtime, value));
            },
        },
        { "staticCharProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticCharProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticCharProperty(convert_value_to_native<char16_t>(runtime, value));
            },
        },
        { "staticCompositeStructArrayProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticCompositeStructArrayProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticCompositeStructArrayProperty(convert_value_to_native<winrt::array_view<const winrt::TestComponent::CompositeType>>(runtime, value));
            },
        },
        { "staticCompositeStructProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticCompositeStructProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticCompositeStructProperty(convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, value));
            },
        },
        { "staticEnumArrayProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticEnumArrayProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticEnumArrayProperty(convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestEnum>>(runtime, value));
            },
        },
        { "staticEnumProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticEnumProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticEnumProperty(convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, value));
            },
        },
        { "staticF32Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticF32Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticF32Property(convert_value_to_native<float>(runtime, value));
            },
        },
        { "staticF64Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticF64Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticF64Property(convert_value_to_native<double>(runtime, value));
            },
        },
        { "staticGuidArrayProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticGuidArrayProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticGuidArrayProperty(convert_value_to_native<winrt::array_view<const winrt::guid>>(runtime, value));
            },
        },
        { "staticGuidProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticGuidProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticGuidProperty(convert_value_to_native<winrt::guid>(runtime, value));
            },
        },
        { "staticNumericArrayProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticNumericArrayProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticNumericArrayProperty(convert_value_to_native<winrt::array_view<const int32_t>>(runtime, value));
            },
        },
        { "staticNumericsStructProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticNumericsStructProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticNumericsStructProperty(convert_value_to_native<winrt::TestComponent::NumericTypes>(runtime, value));
            },
        },
        { "staticObjectArrayProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticObjectArrayProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticObjectArrayProperty(convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestObject>>(runtime, value));
            },
        },
        { "staticObjectProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticObjectProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticObjectProperty(convert_value_to_native<winrt::TestComponent::TestObject>(runtime, value));
            },
        },
        { "staticRefArrayProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticRefArrayProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticRefArrayProperty(convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IReference<int32_t>>>(runtime, value));
            },
        },
        { "staticRefBooleanProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticRefBooleanProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticRefBooleanProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<bool>>(runtime, value));
            },
        },
        { "staticRefCharProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticRefCharProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticRefCharProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<char16_t>>(runtime, value));
            },
        },
        { "staticRefEnumProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticRefEnumProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticRefEnumProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<winrt::TestComponent::TestEnum>>(runtime, value));
            },
        },
        { "staticRefNumericProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticRefNumericProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticRefNumericProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, value));
            },
        },
        { "staticS16Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticS16Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticS16Property(convert_value_to_native<int16_t>(runtime, value));
            },
        },
        { "staticS32Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticS32Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticS32Property(convert_value_to_native<int32_t>(runtime, value));
            },
        },
        { "staticS64Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticS64Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticS64Property(convert_value_to_native<int64_t>(runtime, value));
            },
        },
        { "staticStringArrayProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticStringArrayProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticStringArrayProperty(convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, value));
            },
        },
        { "staticStringProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticStringProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticStringProperty(convert_value_to_native<winrt::hstring>(runtime, value));
            },
        },
        { "staticStringsStructProperty",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticStringsStructProperty());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticStringsStructProperty(convert_value_to_native<winrt::TestComponent::StringTypes>(runtime, value));
            },
        },
        { "staticU16Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticU16Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticU16Property(convert_value_to_native<uint16_t>(runtime, value));
            },
        },
        { "staticU32Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticU32Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticU32Property(convert_value_to_native<uint32_t>(runtime, value));
            },
        },
        { "staticU64Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticU64Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticU64Property(convert_value_to_native<uint64_t>(runtime, value));
            },
        },
        { "staticU8Property",
            [](jsi::Runtime& runtime) {
                return convert_native_to_value(runtime, winrt::TestComponent::Test::StaticU8Property());
            },
            [](jsi::Runtime& runtime, const jsi::Value& value) {
                winrt::TestComponent::Test::StaticU8Property(convert_value_to_native<uint8_t>(runtime, value));
            },
        },
    };

    static constexpr const static_class_data::event_mapping event_data[] = {
        { "staticbooleventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticBoolEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<bool>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticBoolEventHandler(token);
            }
        },
        { "staticchareventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticCharEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<char16_t>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticCharEventHandler(token);
            }
        },
        { "staticcompositestructeventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticCompositeStructEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<winrt::TestComponent::CompositeType>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticCompositeStructEventHandler(token);
            }
        },
        { "staticenumeventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticEnumEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<winrt::TestComponent::TestEnum>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticEnumEventHandler(token);
            }
        },
        { "staticguideventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticGuidEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<winrt::guid>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticGuidEventHandler(token);
            }
        },
        { "staticnumericeventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticNumericEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<int32_t>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticNumericEventHandler(token);
            }
        },
        { "staticobjecteventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticObjectEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<winrt::TestComponent::TestObject>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticObjectEventHandler(token);
            }
        },
        { "staticrefeventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticRefEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IReference<int32_t>>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticRefEventHandler(token);
            }
        },
        { "staticstringeventhandler",
            [](jsi::Runtime& runtime, const jsi::Value& callback) {
                return winrt::TestComponent::Test::StaticStringEventHandler(convert_value_to_native<winrt::Windows::Foundation::EventHandler<winrt::hstring>>(runtime, callback));
            },
            [](winrt::event_token token) {
                winrt::TestComponent::Test::StaticStringEventHandler(token);
            }
        },
    };

    static constexpr const static_class_data::function_mapping function_data[] = {
        { "addAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::AddAsync(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "addAsync"sv, count);
            }
        },
        { "copyBoolsToVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyBoolsToVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyBoolsToVector"sv, count);
            }
        },
        { "copyBoolsToVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyBoolsToVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyBoolsToVectorView"sv, count);
            }
        },
        { "copyCharsToVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const char16_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyCharsToVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyCharsToVector"sv, count);
            }
        },
        { "copyCharsToVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const char16_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyCharsToVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyCharsToVectorView"sv, count);
            }
        },
        { "copyCompositeStructsToVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyCompositeStructsToVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyCompositeStructsToVector"sv, count);
            }
        },
        { "copyCompositeStructsToVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyCompositeStructsToVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyCompositeStructsToVectorView"sv, count);
            }
        },
        { "copyEnumValuesToVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyEnumValuesToVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyEnumValuesToVector"sv, count);
            }
        },
        { "copyEnumValuesToVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyEnumValuesToVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyEnumValuesToVectorView"sv, count);
            }
        },
        { "copyGuidsToVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::guid>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyGuidsToVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyGuidsToVector"sv, count);
            }
        },
        { "copyGuidsToVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::guid>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyGuidsToVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyGuidsToVectorView"sv, count);
            }
        },
        { "copyNumericsToVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyNumericsToVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyNumericsToVector"sv, count);
            }
        },
        { "copyNumericsToVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyNumericsToVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyNumericsToVectorView"sv, count);
            }
        },
        { "copyObjectsToVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestObject>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyObjectsToVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyObjectsToVector"sv, count);
            }
        },
        { "copyObjectsToVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IInspectable>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyObjectsToVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyObjectsToVectorView"sv, count);
            }
        },
        { "copyRefsToVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyRefsToVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyRefsToVector"sv, count);
            }
        },
        { "copyRefsToVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyRefsToVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyRefsToVectorView"sv, count);
            }
        },
        { "copyStringsToVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyStringsToVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyStringsToVector"sv, count);
            }
        },
        { "copyStringsToVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyStringsToVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyStringsToVectorView"sv, count);
            }
        },
        { "copyToMapView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IMap<winrt::hstring, int32_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CopyToMapView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "copyToMapView"sv, count);
            }
        },
        { "countDoubleAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CountDoubleAsync(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "countDoubleAsync"sv, count);
            }
        },
        { "countToNumberAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::CountToNumberAsync(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "countToNumberAsync"sv, count);
            }
        },
        { "createStringToNumberMap",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::TestComponent::Test::CreateStringToNumberMap();
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "createStringToNumberMap"sv, count);
            }
        },
        { "getObjectsAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::TestComponent::Test::GetObjectsAsync();
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "getObjectsAsync"sv, count);
            }
        },
        { "immediateReturnAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ImmediateReturnAsync(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "immediateReturnAsync"sv, count);
            }
        },
        { "logFailures",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::TestComponent::Test::LogFailures(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "logFailures"sv, count);
            }
        },
        { "makeObservableMap",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::TestComponent::Test::MakeObservableMap();
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "makeObservableMap"sv, count);
            }
        },
        { "makeObservableVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::TestComponent::Test::MakeObservableVector();
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "makeObservableVector"sv, count);
            }
        },
        { "pauseAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::PauseAsync(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "pauseAsync"sv, count);
            }
        },
        { "raiseStaticBoolEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    winrt::TestComponent::Test::RaiseStaticBoolEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticBoolEvent"sv, count);
            }
        },
        { "raiseStaticCharEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                    winrt::TestComponent::Test::RaiseStaticCharEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticCharEvent"sv, count);
            }
        },
        { "raiseStaticCompositeStructEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, args[0]);
                    winrt::TestComponent::Test::RaiseStaticCompositeStructEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticCompositeStructEvent"sv, count);
            }
        },
        { "raiseStaticEnumEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, args[0]);
                    winrt::TestComponent::Test::RaiseStaticEnumEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticEnumEvent"sv, count);
            }
        },
        { "raiseStaticGuidEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::guid>(runtime, args[0]);
                    winrt::TestComponent::Test::RaiseStaticGuidEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticGuidEvent"sv, count);
            }
        },
        { "raiseStaticNumericEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    winrt::TestComponent::Test::RaiseStaticNumericEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticNumericEvent"sv, count);
            }
        },
        { "raiseStaticNumericEventAsync",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::RaiseStaticNumericEventAsync(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticNumericEventAsync"sv, count);
            }
        },
        { "raiseStaticObjectEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, args[0]);
                    winrt::TestComponent::Test::RaiseStaticObjectEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticObjectEvent"sv, count);
            }
        },
        { "raiseStaticRefEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, args[0]);
                    winrt::TestComponent::Test::RaiseStaticRefEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticRefEvent"sv, count);
            }
        },
        { "raiseStaticStringEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::TestComponent::Test::RaiseStaticStringEvent(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "raiseStaticStringEvent"sv, count);
            }
        },
        { "returnSameBoolIterable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<bool>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameBoolIterable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameBoolIterable"sv, count);
            }
        },
        { "returnSameBoolVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVector<bool>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameBoolVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameBoolVector"sv, count);
            }
        },
        { "returnSameBoolVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorView<bool>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameBoolVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameBoolVectorView"sv, count);
            }
        },
        { "returnSameCharIterable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<char16_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameCharIterable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameCharIterable"sv, count);
            }
        },
        { "returnSameCharVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVector<char16_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameCharVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameCharVector"sv, count);
            }
        },
        { "returnSameCharVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorView<char16_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameCharVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameCharVectorView"sv, count);
            }
        },
        { "returnSameCompositeStructIterable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameCompositeStructIterable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameCompositeStructIterable"sv, count);
            }
        },
        { "returnSameCompositeStructVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVector<winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameCompositeStructVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameCompositeStructVector"sv, count);
            }
        },
        { "returnSameCompositeStructVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorView<winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameCompositeStructVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameCompositeStructVectorView"sv, count);
            }
        },
        { "returnSameEnumIterable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameEnumIterable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameEnumIterable"sv, count);
            }
        },
        { "returnSameEnumVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVector<winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameEnumVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameEnumVector"sv, count);
            }
        },
        { "returnSameEnumVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorView<winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameEnumVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameEnumVectorView"sv, count);
            }
        },
        { "returnSameGuidIterable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<winrt::guid>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameGuidIterable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameGuidIterable"sv, count);
            }
        },
        { "returnSameGuidVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVector<winrt::guid>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameGuidVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameGuidVector"sv, count);
            }
        },
        { "returnSameGuidVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorView<winrt::guid>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameGuidVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameGuidVectorView"sv, count);
            }
        },
        { "returnSameNumericIterable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<int32_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameNumericIterable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameNumericIterable"sv, count);
            }
        },
        { "returnSameNumericVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVector<int32_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameNumericVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameNumericVector"sv, count);
            }
        },
        { "returnSameNumericVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorView<int32_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameNumericVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameNumericVectorView"sv, count);
            }
        },
        { "returnSameObjectIterable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<winrt::TestComponent::TestObject>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameObjectIterable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameObjectIterable"sv, count);
            }
        },
        { "returnSameObjectVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVector<winrt::TestComponent::TestObject>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameObjectVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameObjectVector"sv, count);
            }
        },
        { "returnSameObjectVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorView<winrt::TestComponent::TestObject>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameObjectVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameObjectVectorView"sv, count);
            }
        },
        { "returnSameRefIterable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameRefIterable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameRefIterable"sv, count);
            }
        },
        { "returnSameRefVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameRefVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameRefVector"sv, count);
            }
        },
        { "returnSameRefVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameRefVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameRefVectorView"sv, count);
            }
        },
        { "returnSameStringIterable",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IIterable<winrt::hstring>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameStringIterable(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameStringIterable"sv, count);
            }
        },
        { "returnSameStringVector",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVector<winrt::hstring>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameStringVector(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameStringVector"sv, count);
            }
        },
        { "returnSameStringVectorView",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorView<winrt::hstring>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::ReturnSameStringVectorView(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "returnSameStringVectorView"sv, count);
            }
        },
        { "staticAdd",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticAdd(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticAdd"sv, count);
            }
        },
        { "staticAddAll",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::StaticAddAll(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticAddAll"sv, count);
            }
        },
        { "staticAppend",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 3)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<char16_t>(runtime, args[1]);
                    auto arg2 = convert_value_to_native<winrt::hstring>(runtime, args[2]);
                    auto result = winrt::TestComponent::Test::StaticAppend(arg0, arg1, arg2);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticAppend"sv, count);
            }
        },
        { "staticAppendAll",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::StaticAppendAll(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticAppendAll"sv, count);
            }
        },
        { "staticArityOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::TestComponent::Test::StaticArityOverload();
                    return convert_native_to_value(runtime, result);
                }
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::StaticArityOverload(arg0);
                    return convert_native_to_value(runtime, result);
                }
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticArityOverload(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticArityOverload"sv, count);
            }
        },
        { "staticBoolArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                    winrt::com_array<bool> arg1;
                    winrt::com_array<bool> arg2;
                    auto result = winrt::TestComponent::Test::StaticBoolArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticBoolArrayOutParam"sv, count);
            }
        },
        { "staticBoolFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<bool>>(runtime, args[0]);
                    winrt::TestComponent::Test::StaticBoolFillParam(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticBoolFillParam"sv, count);
            }
        },
        { "staticBoolOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<bool>(runtime, args[1]);
                    bool arg2;
                    bool arg3;
                    auto result = winrt::TestComponent::Test::StaticBoolOutParam(arg0, arg1, arg2, arg3);
                    return make_return_struct(runtime, result, "andResult", arg2, "orResult", arg3);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticBoolOutParam"sv, count);
            }
        },
        { "staticCharArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const char16_t>>(runtime, args[0]);
                    winrt::com_array<char16_t> arg1;
                    winrt::com_array<char16_t> arg2;
                    auto result = winrt::TestComponent::Test::StaticCharArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticCharArrayOutParam"sv, count);
            }
        },
        { "staticCharFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<char16_t>>(runtime, args[0]);
                    winrt::TestComponent::Test::StaticCharFillParam(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticCharFillParam"sv, count);
            }
        },
        { "staticCharOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                    char16_t arg1;
                    char16_t arg2;
                    auto result = winrt::TestComponent::Test::StaticCharOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "next", arg1, "prev", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticCharOutParam"sv, count);
            }
        },
        { "staticCompositeStructArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    winrt::com_array<winrt::TestComponent::CompositeType> arg1;
                    winrt::com_array<winrt::TestComponent::CompositeType> arg2;
                    auto result = winrt::TestComponent::Test::StaticCompositeStructArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticCompositeStructArrayOutParam"sv, count);
            }
        },
        { "staticCompositeStructFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    winrt::TestComponent::Test::StaticCompositeStructFillParam(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticCompositeStructFillParam"sv, count);
            }
        },
        { "staticCompositeStructOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, args[0]);
                    winrt::TestComponent::CompositeType arg1;
                    winrt::TestComponent::CompositeType arg2;
                    auto result = winrt::TestComponent::Test::StaticCompositeStructOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "first", arg1, "second", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticCompositeStructOutParam"sv, count);
            }
        },
        { "staticContractArityOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::StaticContractArityOverload(arg0);
                    return convert_native_to_value(runtime, result);
                }
                if (count == 0)
                {
                    auto result = winrt::TestComponent::Test::StaticContractArityOverload();
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticContractArityOverload"sv, count);
            }
        },
        { "staticContractDefaultOverloadV1",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticContractDefaultOverloadV1(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticContractDefaultOverloadV1"sv, count);
            }
        },
        { "staticContractDefaultOverloadV2",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticContractDefaultOverloadV2(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticContractDefaultOverloadV2"sv, count);
            }
        },
        { "staticContractOutParamOverloadV1",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::hstring arg1;
                    auto result = winrt::TestComponent::Test::StaticContractOutParamOverloadV1(arg0, arg1);
                    return make_return_struct(runtime, result, "outParam", arg1);
                }
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticContractOutParamOverloadV1(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticContractOutParamOverloadV1"sv, count);
            }
        },
        { "staticContractOutParamOverloadV2",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::hstring arg1;
                    auto result = winrt::TestComponent::Test::StaticContractOutParamOverloadV2(arg0, arg1);
                    return make_return_struct(runtime, result, "outParam", arg1);
                }
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticContractOutParamOverloadV2(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticContractOutParamOverloadV2"sv, count);
            }
        },
        { "staticDefaultOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticDefaultOverload(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticDefaultOverload"sv, count);
            }
        },
        { "staticEnumArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    winrt::com_array<winrt::TestComponent::TestEnum> arg1;
                    winrt::com_array<winrt::TestComponent::TestEnum> arg2;
                    auto result = winrt::TestComponent::Test::StaticEnumArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticEnumArrayOutParam"sv, count);
            }
        },
        { "staticEnumFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    winrt::TestComponent::Test::StaticEnumFillParam(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticEnumFillParam"sv, count);
            }
        },
        { "staticEnumOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, args[0]);
                    winrt::TestComponent::TestEnum arg1;
                    winrt::TestComponent::TestEnum arg2;
                    auto result = winrt::TestComponent::Test::StaticEnumOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "plusOne", arg1, "plusTwo", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticEnumOutParam"sv, count);
            }
        },
        { "staticGuidArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::guid>>(runtime, args[0]);
                    winrt::com_array<winrt::guid> arg1;
                    winrt::com_array<winrt::guid> arg2;
                    auto result = winrt::TestComponent::Test::StaticGuidArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticGuidArrayOutParam"sv, count);
            }
        },
        { "staticGuidFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::guid>>(runtime, args[0]);
                    winrt::TestComponent::Test::StaticGuidFillParam(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticGuidFillParam"sv, count);
            }
        },
        { "staticGuidOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::guid>(runtime, args[0]);
                    winrt::guid arg1;
                    winrt::guid arg2;
                    auto result = winrt::TestComponent::Test::StaticGuidOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "zero", arg1, "allSet", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticGuidOutParam"sv, count);
            }
        },
        { "staticInterwovenParams",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 4)
                {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    bool arg1;
                    auto arg2 = convert_value_to_native<int32_t>(runtime, args[1]);
                    int32_t arg3;
                    auto arg4 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[2]);
                    winrt::com_array<int32_t> arg5;
                    auto arg6 = convert_value_to_native<winrt::array_view<int32_t>>(runtime, args[3]);
                    auto result = winrt::TestComponent::Test::StaticInterwovenParams(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
                    return make_return_struct(runtime, result, "outBool", arg1, "outNumeric", arg3, "outArray", arg5);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInterwovenParams"sv, count);
            }
        },
        { "staticInvokeBoolArrayDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::BoolArrayDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeBoolArrayDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeBoolArrayDelegate"sv, count);
            }
        },
        { "staticInvokeBoolDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::BoolDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeBoolDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeBoolDelegate"sv, count);
            }
        },
        { "staticInvokeBoolDelegateWithOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::BoolDelegateWithOutParam>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeBoolDelegateWithOutParam(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeBoolDelegateWithOutParam"sv, count);
            }
        },
        { "staticInvokeCharArrayDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const char16_t>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::CharArrayDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeCharArrayDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeCharArrayDelegate"sv, count);
            }
        },
        { "staticInvokeCharDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::CharDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeCharDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeCharDelegate"sv, count);
            }
        },
        { "staticInvokeCharDelegateWithOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::CharDelegateWithOutParam>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeCharDelegateWithOutParam(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeCharDelegateWithOutParam"sv, count);
            }
        },
        { "staticInvokeCompositeStructArrayDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::CompositeStructArrayDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeCompositeStructArrayDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeCompositeStructArrayDelegate"sv, count);
            }
        },
        { "staticInvokeCompositeStructDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::CompositeStructDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeCompositeStructDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeCompositeStructDelegate"sv, count);
            }
        },
        { "staticInvokeCompositeStructDelegateWithOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::CompositeStructDelegateWithOutParam>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeCompositeStructDelegateWithOutParam(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeCompositeStructDelegateWithOutParam"sv, count);
            }
        },
        { "staticInvokeEnumArrayDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::EnumArrayDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeEnumArrayDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeEnumArrayDelegate"sv, count);
            }
        },
        { "staticInvokeEnumDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::EnumDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeEnumDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeEnumDelegate"sv, count);
            }
        },
        { "staticInvokeEnumDelegateWithOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::EnumDelegateWithOutParam>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeEnumDelegateWithOutParam(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeEnumDelegateWithOutParam"sv, count);
            }
        },
        { "staticInvokeGuidArrayDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::guid>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::GuidArrayDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeGuidArrayDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeGuidArrayDelegate"sv, count);
            }
        },
        { "staticInvokeGuidDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::guid>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::GuidDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeGuidDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeGuidDelegate"sv, count);
            }
        },
        { "staticInvokeGuidDelegateWithOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::guid>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::GuidDelegateWithOutParam>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeGuidDelegateWithOutParam(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeGuidDelegateWithOutParam"sv, count);
            }
        },
        { "staticInvokeInterwovenDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 4)
                {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto arg2 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[2]);
                    auto arg3 = convert_value_to_native<winrt::TestComponent::InterwovenDelegate>(runtime, args[3]);
                    auto result = winrt::TestComponent::Test::StaticInvokeInterwovenDelegate(arg0, arg1, arg2, arg3);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeInterwovenDelegate"sv, count);
            }
        },
        { "staticInvokeNumericArrayDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::NumericArrayDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeNumericArrayDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeNumericArrayDelegate"sv, count);
            }
        },
        { "staticInvokeNumericDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::NumericDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeNumericDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeNumericDelegate"sv, count);
            }
        },
        { "staticInvokeNumericDelegateWithOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::NumericDelegateWithOutParam>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeNumericDelegateWithOutParam(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeNumericDelegateWithOutParam"sv, count);
            }
        },
        { "staticInvokeObjectArrayDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestObject>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::ObjectArrayDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeObjectArrayDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeObjectArrayDelegate"sv, count);
            }
        },
        { "staticInvokeObjectDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::ObjectDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeObjectDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeObjectDelegate"sv, count);
            }
        },
        { "staticInvokeObjectDelegateWithOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::ObjectDelegateWithOutParam>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeObjectDelegateWithOutParam(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeObjectDelegateWithOutParam"sv, count);
            }
        },
        { "staticInvokeRefArrayDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::RefArrayDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeRefArrayDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeRefArrayDelegate"sv, count);
            }
        },
        { "staticInvokeRefDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::RefDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeRefDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeRefDelegate"sv, count);
            }
        },
        { "staticInvokeRefDelegateWithOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::RefDelegateWithOutParam>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeRefDelegateWithOutParam(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeRefDelegateWithOutParam"sv, count);
            }
        },
        { "staticInvokeStringArrayDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::StringArrayDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeStringArrayDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeStringArrayDelegate"sv, count);
            }
        },
        { "staticInvokeStringDelegate",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::StringDelegate>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeStringDelegate(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeStringDelegate"sv, count);
            }
        },
        { "staticInvokeStringDelegateWithOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::TestComponent::StringDelegateWithOutParam>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticInvokeStringDelegateWithOutParam(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticInvokeStringDelegateWithOutParam"sv, count);
            }
        },
        { "staticNumericArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[0]);
                    winrt::com_array<int32_t> arg1;
                    winrt::com_array<int32_t> arg2;
                    auto result = winrt::TestComponent::Test::StaticNumericArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticNumericArrayOutParam"sv, count);
            }
        },
        { "staticNumericFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<int32_t>>(runtime, args[0]);
                    winrt::TestComponent::Test::StaticNumericFillParam(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticNumericFillParam"sv, count);
            }
        },
        { "staticNumericOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    int32_t arg1;
                    int32_t arg2;
                    auto result = winrt::TestComponent::Test::StaticNumericOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "doubledValue", arg1, "tripledValue", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticNumericOutParam"sv, count);
            }
        },
        { "staticObjectArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestObject>>(runtime, args[0]);
                    winrt::com_array<winrt::TestComponent::TestObject> arg1;
                    winrt::com_array<winrt::TestComponent::TestObject> arg2;
                    auto result = winrt::TestComponent::Test::StaticObjectArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticObjectArrayOutParam"sv, count);
            }
        },
        { "staticObjectFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::TestComponent::TestObject>>(runtime, args[0]);
                    winrt::TestComponent::Test::StaticObjectFillParam(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticObjectFillParam"sv, count);
            }
        },
        { "staticObjectOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, args[0]);
                    winrt::TestComponent::TestObject arg1{ nullptr };
                    winrt::TestComponent::TestObject arg2{ nullptr };
                    auto result = winrt::TestComponent::Test::StaticObjectOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "doubledValue", arg1, "tripledValue", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticObjectOutParam"sv, count);
            }
        },
        { "staticOr",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<bool>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticOr(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticOr"sv, count);
            }
        },
        { "staticOrAll",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                    auto result = winrt::TestComponent::Test::StaticOrAll(arg0);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticOrAll"sv, count);
            }
        },
        { "staticOutParamOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::hstring arg1;
                    auto result = winrt::TestComponent::Test::StaticOutParamOverload(arg0, arg1);
                    return make_return_struct(runtime, result, "outParam", arg1);
                }
                if (count == 2)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = winrt::TestComponent::Test::StaticOutParamOverload(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticOutParamOverload"sv, count);
            }
        },
        { "staticRefArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    winrt::com_array<winrt::Windows::Foundation::IReference<int32_t>> arg1;
                    winrt::com_array<winrt::Windows::Foundation::IReference<int32_t>> arg2;
                    auto result = winrt::TestComponent::Test::StaticRefArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticRefArrayOutParam"sv, count);
            }
        },
        { "staticRefFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    winrt::TestComponent::Test::StaticRefFillParam(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticRefFillParam"sv, count);
            }
        },
        { "staticRefOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, args[0]);
                    winrt::Windows::Foundation::IReference<int32_t> arg1;
                    winrt::Windows::Foundation::IReference<int32_t> arg2;
                    auto result = winrt::TestComponent::Test::StaticRefOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "doubledValue", arg1, "tripledValue", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticRefOutParam"sv, count);
            }
        },
        { "staticStringArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, args[0]);
                    winrt::com_array<winrt::hstring> arg1;
                    winrt::com_array<winrt::hstring> arg2;
                    auto result = winrt::TestComponent::Test::StaticStringArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticStringArrayOutParam"sv, count);
            }
        },
        { "staticStringFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::hstring>>(runtime, args[0]);
                    winrt::TestComponent::Test::StaticStringFillParam(arg0);
                    return jsi::Value::undefined();
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticStringFillParam"sv, count);
            }
        },
        { "staticStringOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 1)
                {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::hstring arg1;
                    winrt::hstring arg2;
                    auto result = winrt::TestComponent::Test::StaticStringOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "lower", arg1, "upper", arg2);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "staticStringOutParam"sv, count);
            }
        },
        { "throwAsyncException",
            []([[maybe_unused]] jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count == 0)
                {
                    auto result = winrt::TestComponent::Test::ThrowAsyncException();
                    return convert_native_to_value(runtime, result);
                }
                throw_no_function_overload(runtime, "TestComponent"sv, "Test"sv, "throwAsyncException"sv, count);
            }
        },
    };

    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 1)
        {
            auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
            return convert_native_to_value(runtime, winrt::TestComponent::Test(arg0));
        }
        if (count == 2)
        {
            auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
            auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
            return convert_native_to_value(runtime, winrt::TestComponent::Test(arg0, arg1));
        }
        if (count == 0)
        {
            return convert_native_to_value(runtime, winrt::TestComponent::Test());
        }
        throw_no_constructor(runtime, "TestComponent"sv, "Test"sv, count);
    }

    constexpr const static_activatable_class_data data{ "TestComponent"sv, "Test"sv, constructor_function, property_data, event_data, function_data };
}

namespace rnwinrt::classes::TestComponent::TestObject
{
    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 1)
        {
            auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
            return convert_native_to_value(runtime, winrt::TestComponent::TestObject(arg0));
        }
        throw_no_constructor(runtime, "TestComponent"sv, "TestObject"sv, count);
    }

    constexpr const static_activatable_class_data data{ "TestComponent"sv, "TestObject"sv, constructor_function, {}, {}, {} };
}

namespace rnwinrt::interfaces::TestComponent::IHierarchyBase
{
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "nonOverriddenHierarchyBaseMethod",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::TestComponent::IHierarchyBase>().NonOverriddenHierarchyBaseMethod();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "overloadedHierarchyBaseMethod",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto result = thisValue.as<winrt::TestComponent::IHierarchyBase>().OverloadedHierarchyBaseMethod(arg0);
                    return convert_native_to_value(runtime, result);
                },
                1, false },
        { "overriddenHierarchyBaseMethod",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::TestComponent::IHierarchyBase>().OverriddenHierarchyBaseMethod();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::IHierarchyBase>(), {}, {}, function_data };
}

namespace rnwinrt::interfaces::TestComponent::IHierarchyBaseFactory
{
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "createInstance",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IInspectable>(runtime, args[0]);
                    winrt::Windows::Foundation::IInspectable arg1;
                    auto result = thisValue.as<winrt::TestComponent::IHierarchyBaseFactory>().CreateInstance(arg0, arg1);
                    return make_return_struct(runtime, result, "innerInterface", arg1);
                },
                1, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::IHierarchyBaseFactory>(), {}, {}, function_data };
}

namespace rnwinrt::interfaces::TestComponent::IHierarchyDerived
{
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "hierarchyDerivedMethod",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::TestComponent::IHierarchyDerived>().HierarchyDerivedMethod();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "overloadedHierarchyBaseMethod",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::IHierarchyDerived>().OverloadedHierarchyBaseMethod(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::IHierarchyDerived>(), {}, {}, function_data };
}

namespace rnwinrt::interfaces::TestComponent::IHierarchyDerivedFactory
{
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "createInstance",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::Windows::Foundation::IInspectable>(runtime, args[1]);
                    winrt::Windows::Foundation::IInspectable arg2;
                    auto result = thisValue.as<winrt::TestComponent::IHierarchyDerivedFactory>().CreateInstance(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "innerInterface", arg2);
                },
                2, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::IHierarchyDerivedFactory>(), {}, {}, function_data };
}

namespace rnwinrt::interfaces::TestComponent::IHierarchyV2Contract
{
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "iHierarchyV2ContractMethod",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::TestComponent::IHierarchyV2Contract>().IHierarchyV2ContractMethod();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::IHierarchyV2Contract>(), {}, {}, function_data };
}

namespace rnwinrt::interfaces::TestComponent::ITest
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "boolProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().BoolProperty());
            },
            nullptr
        },
        { "booleanArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().BooleanArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().BooleanArrayProperty(convert_value_to_native<winrt::array_view<const bool>>(runtime, value));
            },
        },
        { "booleansStructProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().BooleansStructProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().BooleansStructProperty(convert_value_to_native<winrt::TestComponent::BooleanTypes>(runtime, value));
            },
        },
        { "charArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().CharArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().CharArrayProperty(convert_value_to_native<winrt::array_view<const char16_t>>(runtime, value));
            },
        },
        { "charProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().CharProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().CharProperty(convert_value_to_native<char16_t>(runtime, value));
            },
        },
        { "compositeStructArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().CompositeStructArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().CompositeStructArrayProperty(convert_value_to_native<winrt::array_view<const winrt::TestComponent::CompositeType>>(runtime, value));
            },
        },
        { "compositeStructProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().CompositeStructProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().CompositeStructProperty(convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, value));
            },
        },
        { "constructorParamCount",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().ConstructorParamCount());
            },
            nullptr
        },
        { "dateTimeProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().DateTimeProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().DateTimeProperty(convert_value_to_native<winrt::Windows::Foundation::DateTime>(runtime, value));
            },
        },
        { "enumArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().EnumArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().EnumArrayProperty(convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestEnum>>(runtime, value));
            },
        },
        { "enumProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().EnumProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().EnumProperty(convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, value));
            },
        },
        { "f32Property",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().F32Property());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().F32Property(convert_value_to_native<float>(runtime, value));
            },
        },
        { "f64Property",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().F64Property());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().F64Property(convert_value_to_native<double>(runtime, value));
            },
        },
        { "guidArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().GuidArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().GuidArrayProperty(convert_value_to_native<winrt::array_view<const winrt::guid>>(runtime, value));
            },
        },
        { "guidProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().GuidProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().GuidProperty(convert_value_to_native<winrt::guid>(runtime, value));
            },
        },
        { "hResultProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().HResultProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().HResultProperty(convert_value_to_native<winrt::hresult>(runtime, value));
            },
        },
        { "numericArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().NumericArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().NumericArrayProperty(convert_value_to_native<winrt::array_view<const int32_t>>(runtime, value));
            },
        },
        { "numericsStructProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().NumericsStructProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().NumericsStructProperty(convert_value_to_native<winrt::TestComponent::NumericTypes>(runtime, value));
            },
        },
        { "objectArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().ObjectArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().ObjectArrayProperty(convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestObject>>(runtime, value));
            },
        },
        { "objectProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().ObjectProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().ObjectProperty(convert_value_to_native<winrt::TestComponent::TestObject>(runtime, value));
            },
        },
        { "propertyValue",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().PropertyValue());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().PropertyValue(convert_value_to_native<winrt::Windows::Foundation::IPropertyValue>(runtime, value));
            },
        },
        { "propertyValueCppType",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().PropertyValueCppType());
            },
            nullptr
        },
        { "refArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().RefArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().RefArrayProperty(convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IReference<int32_t>>>(runtime, value));
            },
        },
        { "refBooleanProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().RefBooleanProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().RefBooleanProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<bool>>(runtime, value));
            },
        },
        { "refCharProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().RefCharProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().RefCharProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<char16_t>>(runtime, value));
            },
        },
        { "refEnumProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().RefEnumProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().RefEnumProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<winrt::TestComponent::TestEnum>>(runtime, value));
            },
        },
        { "refNumericProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().RefNumericProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().RefNumericProperty(convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, value));
            },
        },
        { "s16Property",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().S16Property());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().S16Property(convert_value_to_native<int16_t>(runtime, value));
            },
        },
        { "s32Property",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().S32Property());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().S32Property(convert_value_to_native<int32_t>(runtime, value));
            },
        },
        { "s64Property",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().S64Property());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().S64Property(convert_value_to_native<int64_t>(runtime, value));
            },
        },
        { "stringArrayProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().StringArrayProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().StringArrayProperty(convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, value));
            },
        },
        { "stringProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().StringProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().StringProperty(convert_value_to_native<winrt::hstring>(runtime, value));
            },
        },
        { "stringsStructProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().StringsStructProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().StringsStructProperty(convert_value_to_native<winrt::TestComponent::StringTypes>(runtime, value));
            },
        },
        { "timeSpanProperty",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().TimeSpanProperty());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().TimeSpanProperty(convert_value_to_native<winrt::Windows::Foundation::TimeSpan>(runtime, value));
            },
        },
        { "u16Property",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().U16Property());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().U16Property(convert_value_to_native<uint16_t>(runtime, value));
            },
        },
        { "u32Property",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().U32Property());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().U32Property(convert_value_to_native<uint32_t>(runtime, value));
            },
        },
        { "u64Property",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().U64Property());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().U64Property(convert_value_to_native<uint64_t>(runtime, value));
            },
        },
        { "u8Property",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITest>().U8Property());
            },
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest>().U8Property(convert_value_to_native<uint8_t>(runtime, value));
            },
        },
    };
    static constexpr const static_interface_data::event_mapping event_data[] = {
        { "booleventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().BoolEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, bool>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().BoolEventHandler(token);
            }
        },
        { "chareventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().CharEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, char16_t>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().CharEventHandler(token);
            }
        },
        { "compositestructeventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().CompositeStructEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, winrt::TestComponent::CompositeType>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().CompositeStructEventHandler(token);
            }
        },
        { "enumeventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().EnumEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, winrt::TestComponent::TestEnum>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().EnumEventHandler(token);
            }
        },
        { "guideventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().GuidEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, winrt::guid>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().GuidEventHandler(token);
            }
        },
        { "numericeventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().NumericEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, int32_t>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().NumericEventHandler(token);
            }
        },
        { "objecteventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().ObjectEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, winrt::TestComponent::TestObject>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().ObjectEventHandler(token);
            }
        },
        { "refeventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().RefEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, winrt::Windows::Foundation::IReference<int32_t>>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().RefEventHandler(token);
            }
        },
        { "stringeventhandler",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& callback) {
                return thisValue.as<winrt::TestComponent::ITest>().StringEventHandler(convert_value_to_native<winrt::Windows::Foundation::TypedEventHandler<winrt::TestComponent::Test, winrt::hstring>>(runtime, callback));
            },
            [](const winrt::Windows::Foundation::IInspectable& thisValue, winrt::event_token token) {
                thisValue.as<winrt::TestComponent::ITest>().StringEventHandler(token);
            }
        },
    };

    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "add",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().Add(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, false },
        { "addAll",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[0]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().AddAll(arg0);
                    return convert_native_to_value(runtime, result);
                },
                1, false },
        { "append",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<char16_t>(runtime, args[1]);
                    auto arg2 = convert_value_to_native<winrt::hstring>(runtime, args[2]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().Append(arg0, arg1, arg2);
                    return convert_native_to_value(runtime, result);
                },
                3, false },
        { "appendAll",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, args[0]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().AppendAll(arg0);
                    return convert_native_to_value(runtime, result);
                },
                1, false },
        { "arityOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ArityOverload();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "arityOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ArityOverload(arg0);
                    return convert_native_to_value(runtime, result);
                },
                1, false },
        { "arityOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ArityOverload(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, false },
        { "assignPropertyValueAsType",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IPropertyValue>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    thisValue.as<winrt::TestComponent::ITest>().AssignPropertyValueAsType(arg0, arg1);
                    return jsi::Value::undefined();
                },
                2, false },
        { "boolArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                    winrt::com_array<bool> arg1;
                    winrt::com_array<bool> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().BoolArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                },
                1, false },
        { "boolFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<bool>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().BoolFillParam(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "boolOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<bool>(runtime, args[1]);
                    bool arg2;
                    bool arg3;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().BoolOutParam(arg0, arg1, arg2, arg3);
                    return make_return_struct(runtime, result, "andResult", arg2, "orResult", arg3);
                },
                2, false },
        { "charArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const char16_t>>(runtime, args[0]);
                    winrt::com_array<char16_t> arg1;
                    winrt::com_array<char16_t> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().CharArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                },
                1, false },
        { "charFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<char16_t>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().CharFillParam(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "charOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                    char16_t arg1;
                    char16_t arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().CharOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "next", arg1, "prev", arg2);
                },
                1, false },
        { "compositeStructArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    winrt::com_array<winrt::TestComponent::CompositeType> arg1;
                    winrt::com_array<winrt::TestComponent::CompositeType> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().CompositeStructArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                },
                1, false },
        { "compositeStructFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::TestComponent::CompositeType>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().CompositeStructFillParam(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "compositeStructOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, args[0]);
                    winrt::TestComponent::CompositeType arg1;
                    winrt::TestComponent::CompositeType arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().CompositeStructOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "first", arg1, "second", arg2);
                },
                1, false },
        { "contractArityOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ContractArityOverload();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "contractDefaultOverloadV1",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ContractDefaultOverloadV1(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, true },
        { "contractDefaultOverloadV2",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<double>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ContractDefaultOverloadV2(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, false },
        { "contractOutParamOverloadV1",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::hstring arg1;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ContractOutParamOverloadV1(arg0, arg1);
                    return make_return_struct(runtime, result, "outParam", arg1);
                },
                1, true },
        { "contractOutParamOverloadV1",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ContractOutParamOverloadV1(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, true },
        { "contractOutParamOverloadV2",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ContractOutParamOverloadV2(arg0);
                    return convert_native_to_value(runtime, result);
                },
                1, false },
        { "dateTimePropertyCppValue",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::TestComponent::ITest>().DateTimePropertyCppValue();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
        { "defaultOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().DefaultOverload(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, true },
        { "enumArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    winrt::com_array<winrt::TestComponent::TestEnum> arg1;
                    winrt::com_array<winrt::TestComponent::TestEnum> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().EnumArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                },
                1, false },
        { "enumFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::TestComponent::TestEnum>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().EnumFillParam(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "enumOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, args[0]);
                    winrt::TestComponent::TestEnum arg1;
                    winrt::TestComponent::TestEnum arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().EnumOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "plusOne", arg1, "plusTwo", arg2);
                },
                1, false },
        { "guidArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::guid>>(runtime, args[0]);
                    winrt::com_array<winrt::guid> arg1;
                    winrt::com_array<winrt::guid> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().GuidArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                },
                1, false },
        { "guidFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::guid>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().GuidFillParam(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "guidOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::guid>(runtime, args[0]);
                    winrt::guid arg1;
                    winrt::guid arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().GuidOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "zero", arg1, "allSet", arg2);
                },
                1, false },
        { "interwovenParams",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    bool arg1;
                    auto arg2 = convert_value_to_native<int32_t>(runtime, args[1]);
                    int32_t arg3;
                    auto arg4 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[2]);
                    winrt::com_array<int32_t> arg5;
                    auto arg6 = convert_value_to_native<winrt::array_view<int32_t>>(runtime, args[3]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().InterwovenParams(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
                    return make_return_struct(runtime, result, "outBool", arg1, "outNumeric", arg3, "outArray", arg5);
                },
                4, false },
        { "numericArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[0]);
                    winrt::com_array<int32_t> arg1;
                    winrt::com_array<int32_t> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().NumericArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                },
                1, false },
        { "numericFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<int32_t>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().NumericFillParam(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "numericOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    int32_t arg1;
                    int32_t arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().NumericOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "doubledValue", arg1, "tripledValue", arg2);
                },
                1, false },
        { "objectArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestObject>>(runtime, args[0]);
                    winrt::com_array<winrt::TestComponent::TestObject> arg1;
                    winrt::com_array<winrt::TestComponent::TestObject> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ObjectArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                },
                1, false },
        { "objectFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::TestComponent::TestObject>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().ObjectFillParam(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "objectOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, args[0]);
                    winrt::TestComponent::TestObject arg1{ nullptr };
                    winrt::TestComponent::TestObject arg2{ nullptr };
                    auto result = thisValue.as<winrt::TestComponent::ITest>().ObjectOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "doubledValue", arg1, "tripledValue", arg2);
                },
                1, false },
        { "or",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<bool>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().Or(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, false },
        { "orAll",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().OrAll(arg0);
                    return convert_native_to_value(runtime, result);
                },
                1, false },
        { "outParamOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::hstring arg1;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().OutParamOverload(arg0, arg1);
                    return make_return_struct(runtime, result, "outParam", arg1);
                },
                1, true },
        { "outParamOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest>().OutParamOverload(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, true },
        { "raiseBoolEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RaiseBoolEvent(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "raiseCharEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RaiseCharEvent(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "raiseCompositeStructEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RaiseCompositeStructEvent(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "raiseEnumEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RaiseEnumEvent(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "raiseGuidEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::guid>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RaiseGuidEvent(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "raiseNumericEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RaiseNumericEvent(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "raiseObjectEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RaiseObjectEvent(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "raiseRefEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RaiseRefEvent(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "raiseStringEvent",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RaiseStringEvent(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "refArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    winrt::com_array<winrt::Windows::Foundation::IReference<int32_t>> arg1;
                    winrt::com_array<winrt::Windows::Foundation::IReference<int32_t>> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().RefArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                },
                1, false },
        { "refFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().RefFillParam(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "refOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, args[0]);
                    winrt::Windows::Foundation::IReference<int32_t> arg1;
                    winrt::Windows::Foundation::IReference<int32_t> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().RefOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "doubledValue", arg1, "tripledValue", arg2);
                },
                1, false },
        { "stringArrayOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, args[0]);
                    winrt::com_array<winrt::hstring> arg1;
                    winrt::com_array<winrt::hstring> arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().StringArrayOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "rot1", arg1, "rot2", arg2);
                },
                1, false },
        { "stringFillParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::array_view<winrt::hstring>>(runtime, args[0]);
                    thisValue.as<winrt::TestComponent::ITest>().StringFillParam(arg0);
                    return jsi::Value::undefined();
                },
                1, false },
        { "stringOutParam",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::hstring arg1;
                    winrt::hstring arg2;
                    auto result = thisValue.as<winrt::TestComponent::ITest>().StringOutParam(arg0, arg1, arg2);
                    return make_return_struct(runtime, result, "lower", arg1, "upper", arg2);
                },
                1, false },
        { "timeSpanPropertyCppValue",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto result = thisValue.as<winrt::TestComponent::ITest>().TimeSpanPropertyCppValue();
                    return convert_native_to_value(runtime, result);
                },
                0, false },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::ITest>(), property_data, event_data, function_data };
}

namespace rnwinrt::interfaces::TestComponent::ITest2
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "boolProperty",
            nullptr,
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, const jsi::Value& value) {
                thisValue.as<winrt::TestComponent::ITest2>().BoolProperty(convert_value_to_native<bool>(runtime, value));
            },
        },
    };
    static constexpr const static_interface_data::function_mapping function_data[] = {
        { "contractArityOverload",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto result = thisValue.as<winrt::TestComponent::ITest2>().ContractArityOverload(arg0);
                    return convert_native_to_value(runtime, result);
                },
                1, false },
        { "contractDefaultOverloadV1",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<double>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest2>().ContractDefaultOverloadV1(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, false },
        { "contractDefaultOverloadV2",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<int32_t>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest2>().ContractDefaultOverloadV2(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, true },
        { "contractOutParamOverloadV1",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                    auto result = thisValue.as<winrt::TestComponent::ITest2>().ContractOutParamOverloadV1(arg0);
                    return convert_native_to_value(runtime, result);
                },
                1, false },
        { "contractOutParamOverloadV2",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    winrt::hstring arg1;
                    auto result = thisValue.as<winrt::TestComponent::ITest2>().ContractOutParamOverloadV2(arg0, arg1);
                    return make_return_struct(runtime, result, "outParam", arg1);
                },
                1, true },
        { "contractOutParamOverloadV2",
            []([[maybe_unused]] jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue, [[maybe_unused]] const jsi::Value* args) {
                    auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<winrt::hstring>(runtime, args[1]);
                    auto result = thisValue.as<winrt::TestComponent::ITest2>().ContractOutParamOverloadV2(arg0, arg1);
                    return convert_native_to_value(runtime, result);
                },
                2, true },
    };

    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::ITest2>(), property_data, {}, function_data };
}

namespace rnwinrt::interfaces::TestComponent::ITestInterface
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "magicValue",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITestInterface>().MagicValue());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::ITestInterface>(), property_data, {}, {} };
}

namespace rnwinrt::interfaces::TestComponent::ITestObject
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "value",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::TestComponent::ITestObject>().Value());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::TestComponent::ITestObject>(), property_data, {}, {} };
}

namespace rnwinrt
{
    jsi::Value projected_value_traits<winrt::TestComponent::BooleanTypes>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::BooleanTypes& value)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "value", convert_native_to_value(runtime, value.Value));
        return result;
    }

    winrt::TestComponent::BooleanTypes projected_value_traits<winrt::TestComponent::BooleanTypes>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::TestComponent::BooleanTypes result{};
        auto obj = value.asObject(runtime);
        if (auto field = obj.getProperty(runtime, "value"); !field.isUndefined())
            result.Value = convert_value_to_native<bool>(runtime, field);
        return result;
    }

    jsi::Value projected_value_traits<winrt::TestComponent::CompositeType>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::CompositeType& value)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "numerics", convert_native_to_value(runtime, value.Numerics));
        result.setProperty(runtime, "strings", convert_native_to_value(runtime, value.Strings));
        result.setProperty(runtime, "bools", convert_native_to_value(runtime, value.Bools));
        return result;
    }

    winrt::TestComponent::CompositeType projected_value_traits<winrt::TestComponent::CompositeType>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::TestComponent::CompositeType result{};
        auto obj = value.asObject(runtime);
        if (auto field = obj.getProperty(runtime, "numerics"); !field.isUndefined())
            result.Numerics = convert_value_to_native<winrt::TestComponent::NumericTypes>(runtime, field);
        if (auto field = obj.getProperty(runtime, "strings"); !field.isUndefined())
            result.Strings = convert_value_to_native<winrt::TestComponent::StringTypes>(runtime, field);
        if (auto field = obj.getProperty(runtime, "bools"); !field.isUndefined())
            result.Bools = convert_value_to_native<winrt::TestComponent::BooleanTypes>(runtime, field);
        return result;
    }

    jsi::Value projected_value_traits<winrt::TestComponent::NumericTypes>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::NumericTypes& value)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "u8", convert_native_to_value(runtime, value.U8));
        result.setProperty(runtime, "u16", convert_native_to_value(runtime, value.U16));
        result.setProperty(runtime, "u32", convert_native_to_value(runtime, value.U32));
        result.setProperty(runtime, "u64", convert_native_to_value(runtime, value.U64));
        result.setProperty(runtime, "s16", convert_native_to_value(runtime, value.S16));
        result.setProperty(runtime, "s32", convert_native_to_value(runtime, value.S32));
        result.setProperty(runtime, "s64", convert_native_to_value(runtime, value.S64));
        result.setProperty(runtime, "f32", convert_native_to_value(runtime, value.F32));
        result.setProperty(runtime, "f64", convert_native_to_value(runtime, value.F64));
        result.setProperty(runtime, "enum", convert_native_to_value(runtime, value.Enum));
        return result;
    }

    winrt::TestComponent::NumericTypes projected_value_traits<winrt::TestComponent::NumericTypes>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::TestComponent::NumericTypes result{};
        auto obj = value.asObject(runtime);
        if (auto field = obj.getProperty(runtime, "u8"); !field.isUndefined())
            result.U8 = convert_value_to_native<uint8_t>(runtime, field);
        if (auto field = obj.getProperty(runtime, "u16"); !field.isUndefined())
            result.U16 = convert_value_to_native<uint16_t>(runtime, field);
        if (auto field = obj.getProperty(runtime, "u32"); !field.isUndefined())
            result.U32 = convert_value_to_native<uint32_t>(runtime, field);
        if (auto field = obj.getProperty(runtime, "u64"); !field.isUndefined())
            result.U64 = convert_value_to_native<uint64_t>(runtime, field);
        if (auto field = obj.getProperty(runtime, "s16"); !field.isUndefined())
            result.S16 = convert_value_to_native<int16_t>(runtime, field);
        if (auto field = obj.getProperty(runtime, "s32"); !field.isUndefined())
            result.S32 = convert_value_to_native<int32_t>(runtime, field);
        if (auto field = obj.getProperty(runtime, "s64"); !field.isUndefined())
            result.S64 = convert_value_to_native<int64_t>(runtime, field);
        if (auto field = obj.getProperty(runtime, "f32"); !field.isUndefined())
            result.F32 = convert_value_to_native<float>(runtime, field);
        if (auto field = obj.getProperty(runtime, "f64"); !field.isUndefined())
            result.F64 = convert_value_to_native<double>(runtime, field);
        if (auto field = obj.getProperty(runtime, "enum"); !field.isUndefined())
            result.Enum = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, field);
        return result;
    }

    jsi::Value projected_value_traits<winrt::TestComponent::StringTypes>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::StringTypes& value)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "char", convert_native_to_value(runtime, value.Char));
        result.setProperty(runtime, "string", convert_native_to_value(runtime, value.String));
        result.setProperty(runtime, "guid", convert_native_to_value(runtime, value.Guid));
        return result;
    }

    winrt::TestComponent::StringTypes projected_value_traits<winrt::TestComponent::StringTypes>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        winrt::TestComponent::StringTypes result{};
        auto obj = value.asObject(runtime);
        if (auto field = obj.getProperty(runtime, "char"); !field.isUndefined())
            result.Char = convert_value_to_native<char16_t>(runtime, field);
        if (auto field = obj.getProperty(runtime, "string"); !field.isUndefined())
            result.String = convert_value_to_native<winrt::hstring>(runtime, field);
        if (auto field = obj.getProperty(runtime, "guid"); !field.isUndefined())
            result.Guid = convert_value_to_native<winrt::guid>(runtime, field);
        return result;
    }

    jsi::Value projected_value_traits<winrt::TestComponent::BoolArrayDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::BoolArrayDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "BoolArrayDelegate"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "BoolArrayDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::array_view<const bool>>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::array_view<bool>>(runtime, args[1]);
                winrt::com_array<bool> arg2;
                auto result = value(arg0, arg1, arg2);
                return make_return_struct(runtime, result, "outValue", arg2);
            });
    }

    winrt::TestComponent::BoolArrayDelegate projected_value_traits<winrt::TestComponent::BoolArrayDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::array_view<const bool> param0, winrt::array_view<bool> param1, winrt::com_array<bool>& param2) {
            winrt::com_array<bool> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                auto result = fn.call(runtime, arg0, arg1.value());
                auto obj = result.asObject(runtime);
                param2 = convert_value_to_native<winrt::com_array<bool>>(runtime, obj.getProperty(runtime, "outValue"));
                returnValue = convert_value_to_native<winrt::com_array<bool>>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::BoolDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::BoolDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "BoolDelegate"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "BoolDelegate"sv);
                }

                auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                auto result = value(arg0);
                return convert_native_to_value(runtime, result);
            });
    }

    winrt::TestComponent::BoolDelegate projected_value_traits<winrt::TestComponent::BoolDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](bool param0) {
            bool returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                returnValue = convert_value_to_native<bool>(runtime, result);
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::BoolDelegateWithOutParam>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::BoolDelegateWithOutParam& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "BoolDelegateWithOutParam"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "BoolDelegateWithOutParam"sv);
                }

                auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                bool arg1;
                value(arg0, arg1);
                return make_void_return_struct(runtime, "outValue", arg1);
            });
    }

    winrt::TestComponent::BoolDelegateWithOutParam projected_value_traits<winrt::TestComponent::BoolDelegateWithOutParam>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](bool param0, bool& param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<bool>(runtime, obj.getProperty(runtime, "outValue"));
            });
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::CharArrayDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::CharArrayDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "CharArrayDelegate"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "CharArrayDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::array_view<const char16_t>>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::array_view<char16_t>>(runtime, args[1]);
                winrt::com_array<char16_t> arg2;
                auto result = value(arg0, arg1, arg2);
                return make_return_struct(runtime, result, "outValue", arg2);
            });
    }

    winrt::TestComponent::CharArrayDelegate projected_value_traits<winrt::TestComponent::CharArrayDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::array_view<const char16_t> param0, winrt::array_view<char16_t> param1, winrt::com_array<char16_t>& param2) {
            winrt::com_array<char16_t> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                auto result = fn.call(runtime, arg0, arg1.value());
                auto obj = result.asObject(runtime);
                param2 = convert_value_to_native<winrt::com_array<char16_t>>(runtime, obj.getProperty(runtime, "outValue"));
                returnValue = convert_value_to_native<winrt::com_array<char16_t>>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::CharDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::CharDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "CharDelegate"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "CharDelegate"sv);
                }

                auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                auto result = value(arg0);
                return convert_native_to_value(runtime, result);
            });
    }

    winrt::TestComponent::CharDelegate projected_value_traits<winrt::TestComponent::CharDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](char16_t param0) {
            char16_t returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                returnValue = convert_value_to_native<char16_t>(runtime, result);
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::CharDelegateWithOutParam>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::CharDelegateWithOutParam& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "CharDelegateWithOutParam"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "CharDelegateWithOutParam"sv);
                }

                auto arg0 = convert_value_to_native<char16_t>(runtime, args[0]);
                char16_t arg1;
                value(arg0, arg1);
                return make_void_return_struct(runtime, "outValue", arg1);
            });
    }

    winrt::TestComponent::CharDelegateWithOutParam projected_value_traits<winrt::TestComponent::CharDelegateWithOutParam>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](char16_t param0, char16_t& param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<char16_t>(runtime, obj.getProperty(runtime, "outValue"));
            });
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::CompositeStructArrayDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::CompositeStructArrayDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "CompositeStructArrayDelegate"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "CompositeStructArrayDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::CompositeType>>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::array_view<winrt::TestComponent::CompositeType>>(runtime, args[1]);
                winrt::com_array<winrt::TestComponent::CompositeType> arg2;
                auto result = value(arg0, arg1, arg2);
                return make_return_struct(runtime, result, "outValue", arg2);
            });
    }

    winrt::TestComponent::CompositeStructArrayDelegate projected_value_traits<winrt::TestComponent::CompositeStructArrayDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::array_view<const winrt::TestComponent::CompositeType> const& param0, winrt::array_view<winrt::TestComponent::CompositeType> const& param1, winrt::com_array<winrt::TestComponent::CompositeType>& param2) {
            winrt::com_array<winrt::TestComponent::CompositeType> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                auto result = fn.call(runtime, arg0, arg1.value());
                auto obj = result.asObject(runtime);
                param2 = convert_value_to_native<winrt::com_array<winrt::TestComponent::CompositeType>>(runtime, obj.getProperty(runtime, "outValue"));
                returnValue = convert_value_to_native<winrt::com_array<winrt::TestComponent::CompositeType>>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::CompositeStructDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::CompositeStructDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "CompositeStructDelegate"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "CompositeStructDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, args[0]);
                auto result = value(arg0);
                return convert_native_to_value(runtime, result);
            });
    }

    winrt::TestComponent::CompositeStructDelegate projected_value_traits<winrt::TestComponent::CompositeStructDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::TestComponent::CompositeType const& param0) {
            winrt::TestComponent::CompositeType returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                returnValue = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, result);
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::CompositeStructDelegateWithOutParam>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::CompositeStructDelegateWithOutParam& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "CompositeStructDelegateWithOutParam"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "CompositeStructDelegateWithOutParam"sv);
                }

                auto arg0 = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, args[0]);
                winrt::TestComponent::CompositeType arg1;
                value(arg0, arg1);
                return make_void_return_struct(runtime, "outValue", arg1);
            });
    }

    winrt::TestComponent::CompositeStructDelegateWithOutParam projected_value_traits<winrt::TestComponent::CompositeStructDelegateWithOutParam>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::TestComponent::CompositeType const& param0, winrt::TestComponent::CompositeType& param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<winrt::TestComponent::CompositeType>(runtime, obj.getProperty(runtime, "outValue"));
            });
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::EnumArrayDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::EnumArrayDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "EnumArrayDelegate"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "EnumArrayDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestEnum>>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::array_view<winrt::TestComponent::TestEnum>>(runtime, args[1]);
                winrt::com_array<winrt::TestComponent::TestEnum> arg2;
                auto result = value(arg0, arg1, arg2);
                return make_return_struct(runtime, result, "outValue", arg2);
            });
    }

    winrt::TestComponent::EnumArrayDelegate projected_value_traits<winrt::TestComponent::EnumArrayDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::array_view<const winrt::TestComponent::TestEnum> param0, winrt::array_view<winrt::TestComponent::TestEnum> param1, winrt::com_array<winrt::TestComponent::TestEnum>& param2) {
            winrt::com_array<winrt::TestComponent::TestEnum> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                auto result = fn.call(runtime, arg0, arg1.value());
                auto obj = result.asObject(runtime);
                param2 = convert_value_to_native<winrt::com_array<winrt::TestComponent::TestEnum>>(runtime, obj.getProperty(runtime, "outValue"));
                returnValue = convert_value_to_native<winrt::com_array<winrt::TestComponent::TestEnum>>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::EnumDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::EnumDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "EnumDelegate"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "EnumDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, args[0]);
                auto result = value(arg0);
                return convert_native_to_value(runtime, result);
            });
    }

    winrt::TestComponent::EnumDelegate projected_value_traits<winrt::TestComponent::EnumDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::TestComponent::TestEnum param0) {
            winrt::TestComponent::TestEnum returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                returnValue = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, result);
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::EnumDelegateWithOutParam>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::EnumDelegateWithOutParam& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "EnumDelegateWithOutParam"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "EnumDelegateWithOutParam"sv);
                }

                auto arg0 = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, args[0]);
                winrt::TestComponent::TestEnum arg1;
                value(arg0, arg1);
                return make_void_return_struct(runtime, "outValue", arg1);
            });
    }

    winrt::TestComponent::EnumDelegateWithOutParam projected_value_traits<winrt::TestComponent::EnumDelegateWithOutParam>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::TestComponent::TestEnum param0, winrt::TestComponent::TestEnum& param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<winrt::TestComponent::TestEnum>(runtime, obj.getProperty(runtime, "outValue"));
            });
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::GuidArrayDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::GuidArrayDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "GuidArrayDelegate"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "GuidArrayDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::array_view<const winrt::guid>>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::array_view<winrt::guid>>(runtime, args[1]);
                winrt::com_array<winrt::guid> arg2;
                auto result = value(arg0, arg1, arg2);
                return make_return_struct(runtime, result, "outValue", arg2);
            });
    }

    winrt::TestComponent::GuidArrayDelegate projected_value_traits<winrt::TestComponent::GuidArrayDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::array_view<const winrt::guid> param0, winrt::array_view<winrt::guid> param1, winrt::com_array<winrt::guid>& param2) {
            winrt::com_array<winrt::guid> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                auto result = fn.call(runtime, arg0, arg1.value());
                auto obj = result.asObject(runtime);
                param2 = convert_value_to_native<winrt::com_array<winrt::guid>>(runtime, obj.getProperty(runtime, "outValue"));
                returnValue = convert_value_to_native<winrt::com_array<winrt::guid>>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::GuidDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::GuidDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "GuidDelegate"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "GuidDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::guid>(runtime, args[0]);
                auto result = value(arg0);
                return convert_native_to_value(runtime, result);
            });
    }

    winrt::TestComponent::GuidDelegate projected_value_traits<winrt::TestComponent::GuidDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::guid param0) {
            winrt::guid returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                returnValue = convert_value_to_native<winrt::guid>(runtime, result);
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::GuidDelegateWithOutParam>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::GuidDelegateWithOutParam& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "GuidDelegateWithOutParam"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "GuidDelegateWithOutParam"sv);
                }

                auto arg0 = convert_value_to_native<winrt::guid>(runtime, args[0]);
                winrt::guid arg1;
                value(arg0, arg1);
                return make_void_return_struct(runtime, "outValue", arg1);
            });
    }

    winrt::TestComponent::GuidDelegateWithOutParam projected_value_traits<winrt::TestComponent::GuidDelegateWithOutParam>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::guid param0, winrt::guid& param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<winrt::guid>(runtime, obj.getProperty(runtime, "outValue"));
            });
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::InterwovenDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::InterwovenDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "InterwovenDelegate"), 4,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 4)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "InterwovenDelegate"sv);
                }

                auto arg0 = convert_value_to_native<bool>(runtime, args[0]);
                bool arg1;
                auto arg2 = convert_value_to_native<int32_t>(runtime, args[1]);
                int32_t arg3;
                auto arg4 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[2]);
                winrt::com_array<int32_t> arg5;
                auto arg6 = convert_value_to_native<winrt::array_view<int32_t>>(runtime, args[3]);
                auto result = value(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
                return make_return_struct(runtime, result, "outBool", arg1, "outNumeric", arg3, "outArray", arg5);
            });
    }

    winrt::TestComponent::InterwovenDelegate projected_value_traits<winrt::TestComponent::InterwovenDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](bool param0, bool& param1, int32_t param2, int32_t& param3, winrt::array_view<const int32_t> param4, winrt::com_array<int32_t>& param5, winrt::array_view<int32_t> param6) {
            uint32_t returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg2 = convert_native_to_value(runtime, param2);
                auto arg4 = convert_native_to_value(runtime, param4);
                auto arg6 = convert_native_to_value(runtime, param6);
                auto result = fn.call(runtime, arg0, arg2, arg4, arg6.value());
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<bool>(runtime, obj.getProperty(runtime, "outBool"));
                param3 = convert_value_to_native<int32_t>(runtime, obj.getProperty(runtime, "outNumeric"));
                param5 = convert_value_to_native<winrt::com_array<int32_t>>(runtime, obj.getProperty(runtime, "outArray"));
                returnValue = convert_value_to_native<uint32_t>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::NumericArrayDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::NumericArrayDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "NumericArrayDelegate"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "NumericArrayDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::array_view<const int32_t>>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::array_view<int32_t>>(runtime, args[1]);
                winrt::com_array<int32_t> arg2;
                auto result = value(arg0, arg1, arg2);
                return make_return_struct(runtime, result, "outValue", arg2);
            });
    }

    winrt::TestComponent::NumericArrayDelegate projected_value_traits<winrt::TestComponent::NumericArrayDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::array_view<const int32_t> param0, winrt::array_view<int32_t> param1, winrt::com_array<int32_t>& param2) {
            winrt::com_array<int32_t> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                auto result = fn.call(runtime, arg0, arg1.value());
                auto obj = result.asObject(runtime);
                param2 = convert_value_to_native<winrt::com_array<int32_t>>(runtime, obj.getProperty(runtime, "outValue"));
                returnValue = convert_value_to_native<winrt::com_array<int32_t>>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::NumericDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::NumericDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "NumericDelegate"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "NumericDelegate"sv);
                }

                auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                auto result = value(arg0);
                return convert_native_to_value(runtime, result);
            });
    }

    winrt::TestComponent::NumericDelegate projected_value_traits<winrt::TestComponent::NumericDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](int32_t param0) {
            int32_t returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                returnValue = convert_value_to_native<int32_t>(runtime, result);
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::NumericDelegateWithOutParam>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::NumericDelegateWithOutParam& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "NumericDelegateWithOutParam"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "NumericDelegateWithOutParam"sv);
                }

                auto arg0 = convert_value_to_native<int32_t>(runtime, args[0]);
                int32_t arg1;
                value(arg0, arg1);
                return make_void_return_struct(runtime, "outValue", arg1);
            });
    }

    winrt::TestComponent::NumericDelegateWithOutParam projected_value_traits<winrt::TestComponent::NumericDelegateWithOutParam>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](int32_t param0, int32_t& param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<int32_t>(runtime, obj.getProperty(runtime, "outValue"));
            });
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::ObjectArrayDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::ObjectArrayDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "ObjectArrayDelegate"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "ObjectArrayDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::array_view<const winrt::TestComponent::TestObject>>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::array_view<winrt::TestComponent::TestObject>>(runtime, args[1]);
                winrt::com_array<winrt::TestComponent::TestObject> arg2;
                auto result = value(arg0, arg1, arg2);
                return make_return_struct(runtime, result, "outValue", arg2);
            });
    }

    winrt::TestComponent::ObjectArrayDelegate projected_value_traits<winrt::TestComponent::ObjectArrayDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::array_view<const winrt::TestComponent::TestObject> const& param0, winrt::array_view<winrt::TestComponent::TestObject> const& param1, winrt::com_array<winrt::TestComponent::TestObject>& param2) {
            winrt::com_array<winrt::TestComponent::TestObject> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                auto result = fn.call(runtime, arg0, arg1.value());
                auto obj = result.asObject(runtime);
                param2 = convert_value_to_native<winrt::com_array<winrt::TestComponent::TestObject>>(runtime, obj.getProperty(runtime, "outValue"));
                returnValue = convert_value_to_native<winrt::com_array<winrt::TestComponent::TestObject>>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::ObjectDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::ObjectDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "ObjectDelegate"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "ObjectDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, args[0]);
                auto result = value(arg0);
                return convert_native_to_value(runtime, result);
            });
    }

    winrt::TestComponent::ObjectDelegate projected_value_traits<winrt::TestComponent::ObjectDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::TestComponent::TestObject const& param0) {
            winrt::TestComponent::TestObject returnValue{ nullptr };
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                returnValue = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, result);
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::ObjectDelegateWithOutParam>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::ObjectDelegateWithOutParam& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "ObjectDelegateWithOutParam"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "ObjectDelegateWithOutParam"sv);
                }

                auto arg0 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, args[0]);
                winrt::TestComponent::TestObject arg1{ nullptr };
                value(arg0, arg1);
                return make_void_return_struct(runtime, "outValue", arg1);
            });
    }

    winrt::TestComponent::ObjectDelegateWithOutParam projected_value_traits<winrt::TestComponent::ObjectDelegateWithOutParam>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::TestComponent::TestObject const& param0, winrt::TestComponent::TestObject& param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<winrt::TestComponent::TestObject>(runtime, obj.getProperty(runtime, "outValue"));
            });
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::RefArrayDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::RefArrayDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "RefArrayDelegate"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "RefArrayDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::array_view<const winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::array_view<winrt::Windows::Foundation::IReference<int32_t>>>(runtime, args[1]);
                winrt::com_array<winrt::Windows::Foundation::IReference<int32_t>> arg2;
                auto result = value(arg0, arg1, arg2);
                return make_return_struct(runtime, result, "outValue", arg2);
            });
    }

    winrt::TestComponent::RefArrayDelegate projected_value_traits<winrt::TestComponent::RefArrayDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::array_view<const winrt::Windows::Foundation::IReference<int32_t>> const& param0, winrt::array_view<winrt::Windows::Foundation::IReference<int32_t>> const& param1, winrt::com_array<winrt::Windows::Foundation::IReference<int32_t>>& param2) {
            winrt::com_array<winrt::Windows::Foundation::IReference<int32_t>> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                auto result = fn.call(runtime, arg0, arg1.value());
                auto obj = result.asObject(runtime);
                param2 = convert_value_to_native<winrt::com_array<winrt::Windows::Foundation::IReference<int32_t>>>(runtime, obj.getProperty(runtime, "outValue"));
                returnValue = convert_value_to_native<winrt::com_array<winrt::Windows::Foundation::IReference<int32_t>>>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::RefDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::RefDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "RefDelegate"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "RefDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, args[0]);
                auto result = value(arg0);
                return convert_native_to_value(runtime, result);
            });
    }

    winrt::TestComponent::RefDelegate projected_value_traits<winrt::TestComponent::RefDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::Windows::Foundation::IReference<int32_t> const& param0) {
            winrt::Windows::Foundation::IReference<int32_t> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                returnValue = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, result);
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::RefDelegateWithOutParam>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::RefDelegateWithOutParam& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "RefDelegateWithOutParam"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "RefDelegateWithOutParam"sv);
                }

                auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, args[0]);
                winrt::Windows::Foundation::IReference<int32_t> arg1;
                value(arg0, arg1);
                return make_void_return_struct(runtime, "outValue", arg1);
            });
    }

    winrt::TestComponent::RefDelegateWithOutParam projected_value_traits<winrt::TestComponent::RefDelegateWithOutParam>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::Windows::Foundation::IReference<int32_t> const& param0, winrt::Windows::Foundation::IReference<int32_t>& param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<winrt::Windows::Foundation::IReference<int32_t>>(runtime, obj.getProperty(runtime, "outValue"));
            });
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::StringArrayDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::StringArrayDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "StringArrayDelegate"), 2,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 2)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "StringArrayDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::array_view<const winrt::hstring>>(runtime, args[0]);
                auto arg1 = convert_value_to_native<winrt::array_view<winrt::hstring>>(runtime, args[1]);
                winrt::com_array<winrt::hstring> arg2;
                auto result = value(arg0, arg1, arg2);
                return make_return_struct(runtime, result, "outValue", arg2);
            });
    }

    winrt::TestComponent::StringArrayDelegate projected_value_traits<winrt::TestComponent::StringArrayDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::array_view<const winrt::hstring> const& param0, winrt::array_view<winrt::hstring> const& param1, winrt::com_array<winrt::hstring>& param2) {
            winrt::com_array<winrt::hstring> returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto arg1 = convert_native_to_value(runtime, param1);
                auto result = fn.call(runtime, arg0, arg1.value());
                auto obj = result.asObject(runtime);
                param2 = convert_value_to_native<winrt::com_array<winrt::hstring>>(runtime, obj.getProperty(runtime, "outValue"));
                returnValue = convert_value_to_native<winrt::com_array<winrt::hstring>>(runtime, obj.getProperty(runtime, "returnValue"));
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::StringDelegate>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::StringDelegate& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "StringDelegate"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "StringDelegate"sv);
                }

                auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                auto result = value(arg0);
                return convert_native_to_value(runtime, result);
            });
    }

    winrt::TestComponent::StringDelegate projected_value_traits<winrt::TestComponent::StringDelegate>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::hstring const& param0) {
            winrt::hstring returnValue;
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                returnValue = convert_value_to_native<winrt::hstring>(runtime, result);
            });
            return returnValue;
        };
    }

    jsi::Value projected_value_traits<winrt::TestComponent::StringDelegateWithOutParam>::as_value(jsi::Runtime& runtime, const winrt::TestComponent::StringDelegateWithOutParam& value)
    {
        return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "StringDelegateWithOutParam"), 1,
            [value](jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count) {
                if (count != 1)
                {
                    throw_invalid_delegate_arg_count(runtime, "TestComponent"sv, "StringDelegateWithOutParam"sv);
                }

                auto arg0 = convert_value_to_native<winrt::hstring>(runtime, args[0]);
                winrt::hstring arg1;
                value(arg0, arg1);
                return make_void_return_struct(runtime, "outValue", arg1);
            });
    }

    winrt::TestComponent::StringDelegateWithOutParam projected_value_traits<winrt::TestComponent::StringDelegateWithOutParam>::as_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return [ctxt = current_runtime_context()->add_reference(),
                   fn = value.asObject(runtime).asFunction(runtime)](winrt::hstring const& param0, winrt::hstring& param1) {
            ctxt->call_sync([&]() {
                auto& runtime = ctxt->runtime;
                auto arg0 = convert_native_to_value(runtime, param0);
                auto result = fn.call(runtime, arg0);
                auto obj = result.asObject(runtime);
                param1 = convert_value_to_native<winrt::hstring>(runtime, obj.getProperty(runtime, "outValue"));
            });
        };
    }
}
