#pragma once
#include <WinBase.h>

namespace facebook::react
{
    class JSCallInvoker;
    class TurboModule;
}

std::shared_ptr<facebook::react::TurboModule> CreateWinRTTurboModule(
    const std::shared_ptr<facebook::react::CallInvoker>& invoker);

inline std::shared_ptr<facebook::react::TurboModule> TryLoadAndCreateWinRTTurboModule(
    const std::shared_ptr<facebook::react::CallInvoker>& invoker)
{
    if (const auto module = LoadPackagedLibrary(L"WinRTTurboModule.dll", 0 /*reserved*/))
    {
        if (const auto fn =
                reinterpret_cast<decltype(&CreateWinRTTurboModule)>(GetProcAddress(module, "CreateWinRTTurboModule")))
        {
            return fn(invoker);
        }
    }
    return nullptr;
}