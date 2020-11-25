#include "pch.h"

#include "NativeModules.h"
#include "ReactPackageProvider.h"

using namespace winrt::Microsoft::ReactNative;

namespace winrt::WinRTTests::implementation
{
    void ReactPackageProvider::CreatePackage(IReactPackageBuilder const& packageBuilder) noexcept
    {
        AddAttributedModules(packageBuilder);
    }

} // namespace winrt::WinRTTests::implementation
