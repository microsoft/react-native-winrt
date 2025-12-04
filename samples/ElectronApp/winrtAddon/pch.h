// Precompiled header for nodewinrt addon (pch.h)
// Keep this list minimal and stable to maximize build reuse.

#pragma once

// Windows core headers
#include <windows.h>

// Standard library (commonly used pieces)
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

// WinRT base (before projecting specific namespaces)
#include <winrt/base.h>

// Node-API C++ wrapper
#include <napi.h>

// Frequently used Windows App SDK headers (add selectively if build perf justifies)
#include <winrt/Microsoft.Windows.ApplicationModel.WindowsAppRuntime.h>
#include <winrt/Microsoft.Windows.ApplicationModel.DynamicDependency.h>

// Bootstrap header (used in addon.cpp)
#include <MddBootstrap.h>