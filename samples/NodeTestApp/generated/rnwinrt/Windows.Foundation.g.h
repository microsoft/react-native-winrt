#pragma once

#include "base.h"

namespace rnwinrt::namespaces::Windows::Foundation
{
    extern const static_namespace_data data;
}

namespace rnwinrt::enums::Windows::Foundation
{
    namespace AsyncStatus
    {
        extern const static_enum_data data;
    }

    namespace PropertyType
    {
        extern const static_enum_data data;
    }
}

namespace rnwinrt::classes::Windows::Foundation
{
    namespace Deferral
    {
        extern const static_activatable_class_data data;
    }

    namespace PropertyValue
    {
        extern const static_class_data data;
    }
}

namespace rnwinrt::interfaces::Windows::Foundation
{
    namespace IAsyncInfo
    {
        extern const static_interface_data data;
    }

    namespace IClosable
    {
        extern const static_interface_data data;
    }

    namespace IDeferral
    {
        extern const static_interface_data data;
    }

    namespace IPropertyValue
    {
        extern const static_interface_data data;
    }

    namespace IStringable
    {
        extern const static_interface_data data;
    }
}
