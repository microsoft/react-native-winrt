# Developer Usage

This document describes how to use React Native WinRT in your React Native for Windows (RNW) app to access Windows Runtime APIs provided by the Windows SDK.

*NOTE: The following instructions are specific to using the [react-native-winrt npm package](https://www.npmjs.com/package/react-native-winrt). For instructions on using the Nuget package, see the [Nuget usage docs](USAGE-NUGET.md) for more info.*

## Requirements

Make sure you have the following requirements installed:

- See [System Requirements](https://microsoft.github.io/react-native-windows/docs/rnw-dependencies) for React Native for Windows development

## Usage Steps 

1. Open your existing RNW app, or follow [these docs](https://microsoft.github.io/react-native-windows/docs/getting-started) to create a new RNW app.

1. From the command line in the root directory of your project, execute the command `yarn add react-native-winrt@~X.Y` where `X.Y` is the React Native major/minor version used by your app. For example, if your app is a React Native 0.67 app, you would run the command `yarn add react-native-winrt@~0.67`.

1. *OPTIONAL:* Execute the command `npx react-native autolink-windows`. If you normally run your application with the command `npx react-native run-windows`, then autolinking is performed automatically and this step is not necessary. However, if you normally run your application via other means (e.g. by building/running through Visual Studio), then manually autolinking may be necessary.

1. Specify the WinRT namespaces that you want to consume using the `RnWinRTParameters` property in the `ExperimentalFeatures.props` file. Here is an example of how to set the `RnWinRTParameters` property:

    ```xml
    <RnWinRTParameters>
        -include Windows.UI.Notifications
        -include Windows.Data.Xml.Dom
    </RnWinRTParameters>
    ```

1. At the top of your application's `index.js` file, add the line `import 'react-native-winrt'`. This will make all WinRT namespaces and types specified in `ExperimentalFeatures.props` accessible to JavaScript/TypeScript code from there on out.

1. *OPTIONAL:* If you are building the application as 'Debug' (the default for `npx react-native run-windows`), you will need to disable web debugging. This is because turbo modules are incompatible with running the JavaScript engine in the browser. This can be done by changing the following line in your `App.cpp` from this:

    ```cpp
    InstanceSettings().UseWebDebugger(true);
    ```
    to this:
    ```cpp
    InstanceSettings().UseWebDebugger(false);
    ```

1. Build and run your RNW app. If running Debug from Visual Studio, first run `yarn start` in the command prompt from the root directory of your app.

## Troubleshooting and Known Issues

|Issue|Cause/Resolution|
|-----|----------------|
|*"Connection with server could not be established"*|If you are running the application as Debug and have made the change above to not use the web debugger, then it's possible that you need to run `yarn start` in the root of your project before running the app.|
|*"ReferenceError: Windows is not defined"*|This error can be caused by a number of things. In general, this means that the turbo module is not being loaded/initialized properly. If you see this error, verify the following:<ul><li>If you are building as debug, make sure that you have disabled use of the web debugger and that you have re-built the application</li><li>Make sure that the module is loaded before any code depending on it is run. In general, you'll want to verify that you have the line `import 'react-native-winrt'` at the top of your `index.js` file</li><li>Make sure that your `ExperimentalFeatures.props` lists the namespaces/types that you depend on</li></ul>If none of these solve the issue, see the [debugging tips](DEBUGGING.md).|
|Other similar "undefined" errors|If the use of WinRT types is giving you errors that indicate a child namespace, a type, a function, etc. are missing (e.g. by a property accessor returning `undefined`), the first thing you should check is to verify that the name is spelled correctly. Remember to keep in mind that function/property names, enum values, and struct members are projected to "camelCase" - so `DoSomething` becomes `doSomething`, etc. If the name is spelled correctly as expected, then your project might be mis-configured and the desired type is not included in the projection. Verify your `RnWinRTParameters` and try again. If the issue still persists, see the [debugging tips](DEBUGGING.md).|
|Linker warning *"warning C4702: unreachable code"*|These warnings can be safely ignored and are a result of projecting a subset of the Windows SDK.|
|IntelliSense errors and warnings in the Visual Studio Error List Window|These can be safely ignored so long as the project still builds successfully.|
|*"Invariant Violation: Module AppRegistry is not a registered callable module (calling runApplication)..."*|This typically indicates that something in your `App.js` threw an error during initialization. If React Native WinRT is to blame, the cause is likely that you are trying to use a WinRT namespace before the turbo module has been loaded/initialized. If this is the case, ensure that the `import 'react-native-winrt'` comes before `import App from './App'` in your `index.js`.|

If the above does not help resolve your issue, take a look at the [debugging tips](DEBUGGING.md) for instructions on how to debug various issues.
