# Nuget Developer Usage 

This document describes how to use React Native WinRT in your React Native for Windows (RNW) app to access Windows Runtime APIs provided by the Windows SDK.

*NOTE: The following instructions are specific to using the [Microsoft.ReactNative.WinRT nuget package](https://www.nuget.org/packages/Microsoft.ReactNative.WinRT/). For most consumers, we recommend using the react-native-winrt npm package due to its simpler consumption through auto-linking. For instructions on using the npm package, see the [npm usage docs](USAGE.md) for more info.*

## Requirements

Make sure you have the following requirements installed:

- See [System Requirements](https://microsoft.github.io/react-native-windows/docs/rnw-dependencies) for React Native for Windows development
- [Visual Studio](https://visualstudio.microsoft.com/downloads/) is *recommended* for the steps to add the native module project (`WinRTTurboModule`)

## Usage Steps 

1. Open your existing RNW app, or follow [these docs](https://microsoft.github.io/react-native-windows/docs/getting-started) to create a new RNW app.

1. Download and extract the contents of the `WinRTTurboModule` zip file from the [Github releases page](https://github.com/microsoft/react-native-winrt/releases) for the corresponding version of RNW being used in your app.

1. Follow these steps to add and configure the `WinRTTurboModule` project to your RNW project. Note that the `WinRTTurboModule` project already includes a reference to the appropriate version of the [Microsoft.ReactNative.WinRT NuGet package](https://www.nuget.org/packages/Microsoft.ReactNative.WinRT), and is pre-configured to generate a WinRT projection.

    1. Copy the `WinRTTurboModule` directory from the zip file into the `windows` directory of your app. 

        Your app directory should look something like this:

        <img src="images/winrtturbomodule-file-explorer.png" alt="File Explorer folder structure" width="200">

    1. Add `WinRTTurboModule.vcxproj` under the `WinRTTurboModule` directory to your RNW app solution. In Visual Studio: right click on the solution, and click **Add** -> **Existing Project**. Navigate to `WinRTTurboModule.vcxproj` in the file picker dialog and open the file.

    1. Specify the WinRT namespaces that you want to consume using the `RnWinRTParameters` property in the `ExperimentalFeatures.props` file. `RnWinRTParameters` is a property used by the Microsoft.ReactNative.WinRT package. Here is an example of how to set the `RnWinRTParameters` property:

        ```xml
        <PropertyGroup Label="React Native WinRT properties"> 
            <RnWinRTParameters> 
                -include Windows.UI.Notifications  
                -include Windows.Data.Xml.Dom 
            </RnWinRTParameters> 
        </PropertyGroup> 
        ```

1. The `WinRTTurboModule` project can now be built to generate the native module with the specified WinRT namespaces.

1. Follow these steps to include the projected WinRT namespaces and consume them in your RNW app.

    1. Add a project reference from your RNW app to the `WinRTTurboModule` project. If you are using Visual Studio, right click on your RNW app project, select **Add Project Reference** and select **WinRTTurboModule**.

    1. Add the following line to your project's `pch.h` file.

        ```cpp
        #include <winrt/WinRTTurboModule.h>
        ```

    1. Add the following line to your project's `App.cpp` file, underneath the line `PackageProviders().Append(make<ReactPackageProvider>());`:

        ```cpp
        PackageProviders().Append(winrt::WinRTTurboModule::ReactPackageProvider());
        ```

    1. Import the WinRT module in your RNW app. Copy the `WinRTTurboModule.js` file from the downloaded zip file to the main directory for your application (next to `index.js`).
       
        Then, add the following line to your project's `index.js` file, **before** the App import statement:

        ```js
        import './WinRTTurboModule';
        ...
        import App from './App';
        ```

    1. You can now call any of the projected WinRT APIs in your RNW app (in Javascript or Typescript). See [App.tsx](../samples/RNWinRTTestApp/App.tsx) in the sample for an example.  

1. Build and run your RNW app. If running Debug, first run `yarn start` in the command prompt from the root directory of your app.

## Troubleshooting and Known Issues
The troubleshooting instructions for using the nuget package will be mostly identical to the [troubleshooting instructions for the npm package](USAGE.md#Troubleshooting-and-Known-Issues), with the key difference being that the module import for the npm package will be `import 'react-native-winrt'` as opposed to `import './WinRTTurboModule'`. 
