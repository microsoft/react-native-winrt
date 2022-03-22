# React Native WinRT

React Native WinRT is a module for [React Native for Windows](https://microsoft.github.io/react-native-windows/) developers who want to call native (non-XAML) WinRT APIs in their apps.
It uses components from [xlang](https://github.com/Microsoft/xlang) to [parse metadata](https://github.com/microsoft/winmd) for public or private WinRT COM APIs and generate JSI (JavaScript Interface) wrappers for [C++/WinRT](https://github.com/microsoft/cppwinrt) types at compile-time.
The code generator was itself derived from `cppwinrt.exe`.

## Getting started

Refer to the [Github usage docs](https://github.com/microsoft/react-native-winrt/blob/main/docs/USAGE.md) for instructions on how to use React Native WinRT in your React Native Windows app to call projected WinRT APIs in Javascript.

## Versioning

The versioning scheme for React Native WinRT aligns with versions of React Native for Windows.
A new major version of React Native for Windows is not guaranteed to be supported by a mismatched version of React Native WinRT.
For example, a React Native for Windows 0.67 app may not be compatible with **react-native-winrt** 0.66.* package versions due to potential breaking changes.

Please see the versions tab for a list of currently supported versions.

## Contributing

_We want to hear from you!_ File a [new issue](https://github.com/microsoft/react-native-winrt/issues/new) to report a bug, ask a question, or make a suggestion. See our [Contributor's Guide](https://github.com/microsoft/react-native-winrt/blob/main/CONTRIBUTING.md) for more information.
