# React Native WinRT Sample

This sample demonstrates how to use the **Microsoft.ReactNative.WinRT** package to consume projected WinRT APIs with Javascript in a React Native for Windows app.

The WinRT APIs demonstrated in this sample include:
- `Windows.UI.StartScreen`
- `Windows.Foundation.Uri`
- `Windows.Storage`
- `Windows.Foundation.Diagnostics`
- `Windows.UI.Notifications`

## Prerequisites

- Visual Studio 2019 or later
- See [System Requirements](https://microsoft.github.io/react-native-windows/docs/rnw-dependencies) for React Native for Windows development

## Building and running the sample

1. Clone or download the react-native-winrt repository.
2. Open a developer command prompt pointing at this directory.
3. Run `npm install` from this directory.
4. Open the solution file [`\windows\RNWinRTTestApp.sln`](./windows/RNWinRTTestApp.sln) in Visual Studio.
5. Build the solution.
6. If running Debug: run `yarn start` in the command prompt from this directory.
7. F5 deploy the sample app (*RNWinRTTestApp*).

You will see something similar to below after successfully building and running the sample:

<img src="images/sample-app.png" alt="Sample app" width="400">