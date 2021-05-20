# The JS/WinRT language projection

JavaScript WinRT (JS/WinRT) is a module for React Native Windows which provides access to WinRT APIs from JS. It uses components from [xlang](https://github.com/Microsoft/xlang) to [parse metadata](https://github.com/microsoft/winmd) for public or private WinRT COM APIs and generate JSI (JavaScript Interface) wrappers for [C++/WinRT](https://github.com/microsoft/cppwinrt) types at compile-time and the code generator was itself derived from cppwinrt.exe. 

# Building JS/WinRT Sample

* Open a dev command prompt pointing at the root of the repo.
* Run `npm install` under ["\samples\RNWinRTTestApp\"](./samples/RNWinRTTestApp)
* Open the [`RNWinRTTestApp.sln`](./samples/RNWinRTTestApp/windows/RNWinRTTestApp.sln) solution.
* Build the x64 Release or Debug configuration.
* If running Debug: Run 'yarn start' under ["\samples\RNWinRTTestApp\"](./samples/RNWinRTTestApp)
* F5 deploy the sample app.

# Adding the projection to your own solution
> **DISCLAIMER**: These steps are temporary and subject to change as the projection evolves.

* Copy the [`WinRTTurboModule`](./samples/RNWinRTTestApp/windows/WinRTTurboModule) (from the sample) source directory to your project.
* Add [`WinRTTurboModule.vcxproj`](./samples/RNWinRTTestApp/windows/WinRTTurboModule/WinRTTurboModule.vcxproj) to your solution.
* Make sure your project depends on the WinRTTurboModule project.
* Turn off deploy for the WinRTTurboModule project.
* Add `/Zc:externConstexpr` to the additional C++ compilation options for the WinRTTurboModule project
  * OPTIONAL: You can also disable permissive mode (i.e. `/permissive-`) for the project
* Add `import './WinRTTurboModule';` to your project's index.js.
* Add `PackageProviders().Append(winrt::WinRTTurboModule::ReactPackageProvider());` to your project's App.cpp
* Add `#include <winrt\WinRTTurboModule.h>` to your project's pch.h
* Create `WinRTTurboModule.js` under your project (next to index.js) and copy the contents of [rnwinrt/js/WinRTTurboModule.js](./rnwinrt/js/WinRTTurboModule.js) to it.  This should be something like the following:
```
    import * as TurboModuleRegistry from 'react-native/Libraries/TurboModule/TurboModuleRegistry';
    const module = TurboModuleRegistry.get('WinRTTurboModule');
    if (module) {
        module.initialize();
    }
    export default module;
```
* Add `-include [winrt namespaces]` inside <RnWinRTParamters> in your WinRTTurboModule.vcxproj. Ex:
```
    <RnWinRTParameters>-include Windows.Globalization</RnWinRTParameters>
```
# Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.opensource.microsoft.com.

When you submit a pull request, a CLA bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., status check, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.
