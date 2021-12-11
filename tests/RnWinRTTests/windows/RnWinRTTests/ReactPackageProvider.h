// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
#pragma once

#include "winrt/Microsoft.ReactNative.h"

namespace winrt::RnWinRTTests::implementation
{
    struct ReactPackageProvider :
        winrt::implements<ReactPackageProvider, winrt::Microsoft::ReactNative::IReactPackageProvider>
    {
    public: // IReactPackageProvider
        void CreatePackage(winrt::Microsoft::ReactNative::IReactPackageBuilder const& packageBuilder) noexcept;
    };
} // namespace winrt::RnWinRTTests::implementation
