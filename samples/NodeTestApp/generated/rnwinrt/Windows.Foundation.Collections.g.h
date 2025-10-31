#pragma once

#include "base.h"

namespace rnwinrt::namespaces::Windows::Foundation::Collections
{
    extern const static_namespace_data data;
}

namespace rnwinrt::enums::Windows::Foundation::Collections
{
    namespace CollectionChange
    {
        extern const static_enum_data data;
    }
}

namespace rnwinrt::classes::Windows::Foundation::Collections
{
    namespace PropertySet
    {
        extern const static_activatable_class_data data;
    }

    namespace StringMap
    {
        extern const static_activatable_class_data data;
    }

    namespace ValueSet
    {
        extern const static_activatable_class_data data;
    }
}

namespace rnwinrt::interfaces::Windows::Foundation::Collections
{
    namespace IPropertySet
    {
        extern const static_interface_data data;
    }

    namespace IVectorChangedEventArgs
    {
        extern const static_interface_data data;
    }
}
