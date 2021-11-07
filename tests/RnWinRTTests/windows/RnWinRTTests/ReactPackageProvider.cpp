// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

#include "pch.h"

#include "NativeModules.h"
#include "ReactPackageProvider.h"

using namespace winrt::Microsoft::ReactNative;

namespace winrt::RnWinRTTests::implementation
{
    void ReactPackageProvider::CreatePackage(IReactPackageBuilder const& packageBuilder) noexcept
    {
        AddAttributedModules(packageBuilder);
    }

} // namespace winrt::RnWinRTTests::implementation
