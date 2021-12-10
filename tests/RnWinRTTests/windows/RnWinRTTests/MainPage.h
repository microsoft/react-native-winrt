// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
#pragma once

#include "MainPage.g.h"
#include <winrt/Microsoft.ReactNative.h>

namespace winrt::RnWinRTTests::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();
    };
}

namespace winrt::RnWinRTTests::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
