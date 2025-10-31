#include "pch.h"

#include "base.h"

#include "Windows.g.h"
#include "Windows.Foundation.g.h"

namespace rnwinrt::namespaces::Windows
{
    static constexpr const static_projection_data* const children[] = {
        &rnwinrt::namespaces::Windows::Foundation::data,
    };

    constexpr const static_namespace_data data{ "Windows"sv, children };
}
