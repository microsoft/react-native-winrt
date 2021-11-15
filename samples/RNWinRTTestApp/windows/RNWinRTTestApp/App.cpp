// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

#include "pch.h"

#include "App.h"
#include "ReactPackageProvider.h"

using namespace winrt::RNWinRTTestApp;
using namespace winrt::RNWinRTTestApp::implementation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of
/// authored code executed, and as such is the logical equivalent of main() or
/// WinMain().
/// </summary>
App::App() noexcept
{    
    const auto settings = InstanceSettings();

    JavaScriptMainModuleName(L"index");
    settings.UseWebDebugger(false);
    settings.UseDirectDebugger(true);
    settings.UseFastRefresh(true);

#if _DEBUG
    settings.UseDeveloperSupport(true);
#else
    settings.UseDeveloperSupport(false);
#endif

    RegisterAutolinkedNativeModulePackages(PackageProviders()); // Includes any autolinked modules

    PackageProviders().Append(make<ReactPackageProvider>()); // Includes all modules in this project
    PackageProviders().Append(winrt::WinRTTurboModule::ReactPackageProvider());
    InitializeComponent();
}


void App::OnLaunched(const winrt::Windows::ApplicationModel::Activation::LaunchActivatedEventArgs& args)
{
    super::OnLaunched(args);
    winrt::Windows::UI::Xaml::Window::Current().Content().as<winrt::Windows::UI::Xaml::Controls::Frame>().Navigate(winrt::xaml_typename<winrt::RNWinRTTestApp::MainPage>(), winrt::box_value(args.Arguments()));
}