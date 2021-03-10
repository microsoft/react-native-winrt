#include "pch.h"
#include "ReactPackageProvider.h"
#include <ReactPackageProvider.g.cpp>
#include <TurboModuleProvider.h>

namespace local
{
    using namespace winrt::WinRTTurboModule;
    namespace impl
    {
        using namespace winrt::WinRTTurboModule::implementation;
    }
}

namespace winrt
{
    using namespace winrt::Microsoft::ReactNative;
    using namespace winrt::Windows::Foundation;
}

// TODO: WinRTTurboMmodule.cpp implements the function but there is no header for it since the declaration was previously injected in TurboModuleManager.cpp
std::shared_ptr<facebook::react::TurboModule> CreateWinRTTurboModule(const std::shared_ptr<facebook::react::CallInvoker>& jsInvoker);

void local::impl::ReactPackageProvider::CreatePackage(const winrt::IReactPackageBuilder& packageBuilder) noexcept
{
    // TODO: The AddTurboModuleProvider<T> cannot be used since the TurboModule is not foward declared so for the timebeing, copy it inline.
    packageBuilder.as<winrt::IReactPackageBuilderExperimental>().AddTurboModule(L"WinRTTurboModule"sv,
        [](const winrt::IReactModuleBuilder& moduleBuilder) noexcept -> winrt::IInspectable
        {
            // We expect the initializer to be called immediately for TurboModules.
            winrt::IJsiHostObject turboModule{ nullptr };
            moduleBuilder.AddInitializer(
                [&turboModule](const winrt::IReactContext& context) mutable
                {
                   // Ensure the JSI runtime is created.
                    GetOrCreateContextRuntime(winrt::ReactContext{context});

                    // Instantiate the TurboModule directly.
                    turboModule = winrt::make<JsiHostObjectWrapper>(CreateWinRTTurboModule(winrt::MakeAbiCallInvoker(context.JSDispatcher())));
                }
            );
            return turboModule;
        }
    );
}
