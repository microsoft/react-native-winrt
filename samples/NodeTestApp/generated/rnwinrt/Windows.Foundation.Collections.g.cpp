#include "pch.h"

#include "base.h"

#include "Windows.Foundation.Collections.g.h"

#include <winrt/Windows.Foundation.Collections.h>

namespace rnwinrt::namespaces::Windows::Foundation::Collections
{
    static constexpr const static_projection_data* const children[] = {
        &rnwinrt::classes::Windows::Foundation::Collections::PropertySet::data,
        &rnwinrt::classes::Windows::Foundation::Collections::StringMap::data,
        &rnwinrt::classes::Windows::Foundation::Collections::ValueSet::data,
        &rnwinrt::enums::Windows::Foundation::Collections::CollectionChange::data,
    };

    constexpr const static_namespace_data data{ "Collections"sv, children };
}

namespace rnwinrt::enums::Windows::Foundation::Collections::CollectionChange
{
    static constexpr const static_enum_data::value_mapping mappings[] = {
        { "reset"sv, 0, "0"sv },
        { "itemInserted"sv, 1, "1"sv },
        { "itemRemoved"sv, 2, "2"sv },
        { "itemChanged"sv, 3, "3"sv },
    };

    constexpr const static_enum_data data{ "CollectionChange"sv, mappings };
}

namespace rnwinrt::classes::Windows::Foundation::Collections::PropertySet
{
    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 0)
        {
            return convert_native_to_value(runtime, winrt::Windows::Foundation::Collections::PropertySet());
        }
        throw_no_constructor(runtime, "Windows.Foundation.Collections"sv, "PropertySet"sv, count);
    }

    constexpr const static_activatable_class_data data{ "Windows.Foundation.Collections"sv, "PropertySet"sv, constructor_function, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Foundation::Collections::StringMap
{
    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 0)
        {
            return convert_native_to_value(runtime, winrt::Windows::Foundation::Collections::StringMap());
        }
        throw_no_constructor(runtime, "Windows.Foundation.Collections"sv, "StringMap"sv, count);
    }

    constexpr const static_activatable_class_data data{ "Windows.Foundation.Collections"sv, "StringMap"sv, constructor_function, {}, {}, {} };
}

namespace rnwinrt::classes::Windows::Foundation::Collections::ValueSet
{
    static jsi::Value constructor_function(jsi::Runtime& runtime, const jsi::Value&, [[maybe_unused]] const jsi::Value* args, size_t count)
    {
        if (count == 0)
        {
            return convert_native_to_value(runtime, winrt::Windows::Foundation::Collections::ValueSet());
        }
        throw_no_constructor(runtime, "Windows.Foundation.Collections"sv, "ValueSet"sv, count);
    }

    constexpr const static_activatable_class_data data{ "Windows.Foundation.Collections"sv, "ValueSet"sv, constructor_function, {}, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Foundation::Collections::IPropertySet
{
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Foundation::Collections::IPropertySet>(), {}, {}, {} };
}

namespace rnwinrt::interfaces::Windows::Foundation::Collections::IVectorChangedEventArgs
{
    static constexpr const static_interface_data::property_mapping property_data[] = {
        { "collectionChange",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>().CollectionChange());
            },
            nullptr
        },
        { "index",
            [](jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& thisValue) {
                return convert_native_to_value(runtime, thisValue.as<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>().Index());
            },
            nullptr
        },
    };
    constexpr const static_interface_data data{ winrt::guid_of<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>(), property_data, {}, {} };
}
