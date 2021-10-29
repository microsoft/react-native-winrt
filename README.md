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
* Add `import './WinRTTurboModule';` to your project's index.js.
* Add `PackageProviders().Append(winrt::WinRTTurboModule::ReactPackageProvider());` to your project's App.cpp
* Add `#include <winrt\WinRTTurboModule.h>` to your project's pch.h
* Create `WinRTTurboModule.js` under your project (next to index.js) and copy the contents of [jswinrt/js/WinRTTurboModule.js](./jswinrt/js/WinRTTurboModule.js) to it.  This should be something like the following:
```
    import * as TurboModuleRegistry from 'react-native/Libraries/TurboModule/TurboModuleRegistry';
    const module = TurboModuleRegistry.get('WinRTTurboModule');
    if (module) {
        module.initialize();
    }
    export default module;
```
* Add `-include [winrt namespaces]` inside <JsWinRTParameters> in your WinRTTurboModule.vcxproj. Ex:
```
    <JsWinRTParameters>-include Windows.Globalization</JsWinRTParameters>
```
# Contributing

This project welcomes contributions and suggestions of all types. See our [Contributor's Guide](/CONTRIBUTING.md) for more information.
    
# Trademarks

This project may contain trademarks or logos for projects, products, or services. Authorized use of Microsoft trademarks or logos is subject to and must follow [Microsoft's Trademark & Brand Guidelines](https://www.microsoft.com/en-us/legal/intellectualproperty/trademarks). Use of Microsoft trademarks or logos in modified versions of this project must not cause confusion or imply Microsoft sponsorship. Any use of third-party trademarks or logos are subject to those third-party's policies.
