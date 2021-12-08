// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

#pragma once
#pragma warning(disable : 4100) // unreferenced local variable
#pragma warning(disable : 4505) // unreferenced local function has been removed

#define NOMINMAX

#include <Windows.h>
#include <XmlLite.h>
#include <shlwapi.h>

#include <array>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "wil\com.h"
#include "wil\resource.h"
#include "wil\result.h"
#include "wil\win32_helpers.h"

#include <winmd_reader.h>

using namespace std::literals;

template <typename... Fns>
struct overloaded : Fns...
{
    using Fns::operator()...;
};

template <typename... Fns>
overloaded(Fns...) -> overloaded<Fns...>;

inline bool starts_with(std::string_view str, std::string_view prefix)
{
    return str.substr(0, prefix.size()) == prefix;
}

// NOTE: The compare function is supposed to return a value less than zero if the passed in value sorts before the value
// you are testing against
template <typename ListT, typename Comp>
inline auto reverse_sorted_find(ListT&& list, Comp&& compareFn)
{
    auto itr = list.rbegin();
    for (; itr != list.rend(); ++itr)
    {
        auto cmp = compareFn(*itr);
        if (cmp == 0)
        {
            return std::pair{ itr.base() - 1, true };
        }
        else if (cmp < 0)
        {
            return std::pair{ itr.base(), false };
        }
    }

    return std::pair{ itr.base(), false };
}

inline int camel_case_compare(std::string_view lhs, std::string_view rhs)
{
    if (lhs.empty() || rhs.empty())
        return lhs.compare(rhs);

    auto lhsFirst = std::tolower(lhs[0]);
    auto rhsFirst = std::tolower(rhs[0]);
    if (lhsFirst != rhsFirst)
        return lhsFirst - rhsFirst;

    return lhs.substr(1).compare(rhs.substr(1));
}

inline int lower_case_compare(std::string_view lhs, std::string_view rhs)
{
    while (!lhs.empty() && !rhs.empty())
    {
        auto lhsFirst = std::tolower(lhs[0]);
        auto rhsFirst = std::tolower(rhs[0]);
        if (lhsFirst != rhsFirst)
            return lhsFirst - rhsFirst;

        lhs = lhs.substr(1);
        rhs = rhs.substr(1);
    }

    return lhs.compare(rhs);
}

inline void ThrowInvalidArg(std::string const& message)
{
    throw std::invalid_argument(message);
}

template <typename... T>
inline void ThrowInvalidArg(std::string message, T const&... args)
{
    (message.append(args), ...);
    throw std::invalid_argument(message);
}
