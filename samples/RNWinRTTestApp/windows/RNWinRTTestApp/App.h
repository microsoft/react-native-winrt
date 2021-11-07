// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

#pragma once

#include "App.xaml.g.h"

namespace winrt::RNWinRTTestApp::implementation
{
    struct App : AppT<App>
    {
        App() noexcept;
        void OnLaunched(const winrt::Windows::ApplicationModel::Activation::LaunchActivatedEventArgs& args);

    private:
        using super = AppT<App>;
    };
} // namespace winrt::RNWinRTTestApp::implementation


