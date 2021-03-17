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

#if 1
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
#endif
