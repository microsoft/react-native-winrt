#include "pch.h"

#include "base.h"

#include "Windows.Media.g.h"
#include "Windows.Media.PlayTo.g.h"

namespace rnwinrt::namespaces::Windows::Media
{
    static constexpr const static_projection_data* const children[] = {
        &rnwinrt::namespaces::Windows::Media::PlayTo::data,
    };

    constexpr const static_namespace_data data{ "Media"sv, children };
}
