# React Native WinRT

React Native WinRT is a module for [React Native for Windows](https://microsoft.github.io/react-native-windows/) developers who want to call native (non-XAML) WinRT APIs in their apps. It uses components from [xlang](https://github.com/Microsoft/xlang) to [parse metadata](https://github.com/microsoft/winmd) for public or private WinRT COM APIs and generate JSI (JavaScript Interface) wrappers for [C++/WinRT](https://github.com/microsoft/cppwinrt) types at compile-time. The code generator was itself derived from `cppwinrt.exe`.

This repo generates the **Microsoft.ReactNative.WinRT** NuGet package for specific versions of React Native Windows. 

## Getting started

Refer to the [usage docs](docs/USAGE.md) for instructions on how to use React Native WinRT in your React Native Windows app to call projected WinRT APIs in Javascript.

### Sample code

See the [sample app](/samples/RNWinRTTestApp/) for an example of using the **Microsoft.ReactNative.WinRT** package to consume WinRT APIs.

## Contributing

This project welcomes contributions and suggestions of all types. See our [Contributor's Guide](/CONTRIBUTING.md) for more information.

## Trademarks

This project may contain trademarks or logos for projects, products, or services. Authorized use of Microsoft trademarks or logos is subject to and must follow [Microsoft's Trademark & Brand Guidelines](https://www.microsoft.com/en-us/legal/intellectualproperty/trademarks). Use of Microsoft trademarks or logos in modified versions of this project must not cause confusion or imply Microsoft sponsorship. Any use of third-party trademarks or logos are subject to those third-party's policies.
