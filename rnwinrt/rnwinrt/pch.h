#pragma once
#pragma warning(disable: 4100) // unreferenced local variable
#pragma warning(disable: 4505) // unreferenced local function has been removed

#define NOMINMAX

#include <Windows.h>
#include <shlwapi.h>
#include <XmlLite.h>

#include <array>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <regex>
#include <set>
#include <vector>

#include "wil\com.h"
#include "wil\resource.h"
#include "wil\result.h"
#include "wil\win32_helpers.h"

#include <winmd_reader.h>

using namespace std::literals;

inline void ThrowInvalidArg(std::string const& message)
{
    throw std::invalid_argument(message);
}

template <typename...T>
inline void ThrowInvalidArg(std::string message, T const&... args)
{
    (message.append(args), ...);
    throw std::invalid_argument(message);
}