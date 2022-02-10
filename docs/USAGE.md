# Developer Usage 

This document describes how to use React Native WinRT in your React Native for Windows app to access Windows Runtime APIs provided by the Windows SDK.

## Requirements

Make sure you have the following requirements installed:

- See [System Requirements](https://microsoft.github.io/react-native-windows/docs/rnw-dependencies) for React Native for Windows development
- [Visual Studio](https://visualstudio.microsoft.com/downloads/) is *recommended* for the steps to add the native module project (`WinRTTurboModule`)

## Usage Steps 

1. Open your existing RNW app, or follow [these docs](https://microsoft.github.io/react-native-windows/docs/getting-started) to create a new RNW app.

1. Follow these steps to add and configure the `WinRTTurboModule` project to your RNW project. Note that the `WinRTTurboModule` project already includes a reference to the [Microsoft.ReactNative.WinRT NuGet package](https://www.nuget.org/packages/Microsoft.ReactNative.WinRT), and is pre-configured to generate a WinRT projection.

    1. Copy the [`WinRTTurboModule`](../samples/RNWinRTTestApp/windows/WinRTTurboModule) directory for the appropriate release branch to the `windows` directory in your React Native app. You will first need to clone this repo to download the source files to your development machine, and checkout the release branch that corresponds with the version of RNW being used in your app. For example, if your app uses RNW 0.67, you would run the following command line instructions:
    
        ```cmd
        C:\repos>git clone https://github.com/microsoft/react-native-winrt.git
        C:\repos>cd react-native-winrt
        C:\repos\react-native-winrt>git checkout release/0.67
        ```
    
        After copying the `WinRTTurboModule` project to your app, the app directory will look something like this:

        <img src="images/winrtturbomodule-file-explorer.png" alt="File Explorer folder structure" width="200">

    1. Add [`WinRTTurboModule.vcxproj`](../samples/RNWinRTTestApp/windows/WinRTTurboModule/WinRTTurboModule.vcxproj) to your React Native for Windows app solution. In Visual Studio: right click on the solution, and click **Add** -> **Existing Project**. Navigate to `WinRTTurboModule.vcxproj` in the file picker dialog and open the file.

    1. Turn off deploy for the `WinRTTurboModule` project. If you are using Visual Studio, go to **Build** -> **Configuration Manager**, and uncheck the **Deploy** option for the Build/Configuration being used.

        <img src="images/winrtturbomodule-uncheck-deploy.png" alt="Uncheck deploy" width="350">

    1. Specify the WinRT namespaces that you want to consume using the `RnWinRTParameters` property in `WinRTTurboModule.vcxproj`. `RnWinRTParameters` is a property defined in the Microsoft.ReactNative.WinRT package. If you are using Visual Studio, right click on the `WinRTTurboModule` project and select **Unload Project**. You can then directly edit `WinRTTurboModule.vcxproj`. Here is an example of how to set the `RnWinRTParameters` property:

        ```xml
        <RnWinRTParameters>-include Windows.Globalization -include Windows.Storage</RnWinRTParameters>
        ```

1. Reload and build the `WinRTTurboModule` project.

1. Follow these steps to include the projected WinRT namespaces and consume them in your RNW app.

    1. Add a project reference from your React Native Windows app to the `WinRTTurboModule` project. If you are using Visual Studio, right click on your React Native Windows app project, select **Add Project Reference** and select **WinRTTurboModule**.

    1. Add the following line to to your project's `pch.h` file.

        ```cpp
        #include <winrt/WinRTTurboModule.h>
        ```

    1. Add the following line to your project's `App.cpp` file, underneath the line `PackageProviders().Append(make<ReactPackageProvider>());`:

        ```cpp
        PackageProviders().Append(winrt::WinRTTurboModule::ReactPackageProvider());
        ```

    1. Import the WinRT module in your React Native app. Create a file named `WinRTTurboModule.js` under your root project directory (next to `index.js`), and copy the contents of [rnwinrt/js/WinRTTurboModule.js](../rnwinrt/js/WinRTTurboModule.js) to it.  This should be something like the following:

        ```js
        import * as TurboModuleRegistry from 'react-native/Libraries/TurboModule/TurboModuleRegistry';
        const module = TurboModuleRegistry.get('WinRTTurboModule');
        if (module) {
            module.initialize();
        }
        export default module;
        ```
        Then, add the following line to your project's `index.js` file, **before** the App import statement:

        ```js
        import './WinRTTurboModule';
        ...
        import App from './App';
        ```

    1. You can now call any of the projected WinRT APIs in your RNW app (in Javascript or Typescript). See [App.tsx](../samples/RNWinRTTestApp/App.tsx) in the sample for an example.  

1. Build and deploy/run your RNW app. If running Debug, first run `yarn start` in the command prompt from the root directory of your app.

## Troubleshooting and Known Issues

- If running Debug, make sure to run `yarn start` in the command prompt before running the app. Otherwise, you may see an error with *"connection with server could not be established"*.

- When running Debug, you may see the error: *"ReferenceError: Windows is not defined"*. The browser debugger is not supported with React Native Windows, so you may need to change this line in `App.cpp`:

    ```cpp
    `InstanceSettings().UseWebDebugger(true);`
    ```
    to this:
    ```cpp
    `InstanceSettings().UseWebDebugger(false);`
    ```
  
 - When building the `WinRTTurboModule` project, you may see compiler build warnings like *"warning C4702: unreachable code"*. These warnings can be safely ignored and are a result of projecting a subset of the Windows SDK.

- When building the `WinRTTurboModule` project, you may see several IntelliSense build errors and warnings in the Visual Studio Error List Window. These can be safely ignored, and the project should still build successfully.

- When running your React Native app, you may see the following error: *'Invariant Violation: Module AppRegistry is not a registered callable module (calling runApplication). A frequent cause of the error is that the application entry file path is incorrect.
This can also happen when the JS bundle is corrupt or there is an early initialization error when loading React Native.'* 

    To address this error, make sure to import the WinRTTurboModule **before** the App import statement in `index.js`.
