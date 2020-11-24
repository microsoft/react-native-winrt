#pragma once
#pragma warning(disable : 4100) // unreferenced formal parameter

#define NOMINMAX
#include <windows.h>

#include <combaseapi.h>

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

#include <wil\com.h>
#include <wil\cppwinrt.h>
#include <wil\win32_helpers.h>
#include <wil\winrt.h>

#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

#include <inspectable.h>
#include <unknwn.h>
#include <winrt\Windows.Foundation.Collections.h>
#include <winrt\Windows.Foundation.h>

#include <TurboModule\Core\TurboModule.h>
#include <jsi\jsi.h>

namespace jsi
{
    using namespace facebook::jsi;
}

namespace react
{
    using namespace facebook::react;
}

namespace std
{
    template <>
    struct less<winrt::guid>
    {
        bool operator()(const winrt::guid& left, const winrt::guid& right) const
        {
            return memcmp(&left, &right, sizeof(winrt::guid)) < 0;
        }
    };

#if 0
    template<> struct hash<winrt::guid>
    {
        size_t operator()(const winrt::guid& guid) const
        {
            const uint64_t* const guidAsUint64 = reinterpret_cast<uint64_t const*>(&guid);
            hash<uint64_t> uint64Hash;
            return static_cast<size_t>(uint64Hash(guidAsUint64[0]) ^ uint64Hash(guidAsUint64[1]));
        }
    };
#endif
}

using namespace std::literals;