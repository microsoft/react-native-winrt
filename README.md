# The RN/WinRT language projection

React Native WinRT (RN/WinRT) is a module for React Native Windows which provides access to WinRT APIs from Javascript. It uses components from [xlang](https://github.com/Microsoft/xlang) to [parse metadata](https://github.com/microsoft/winmd) for public or private WinRT COM APIs and generate JSI (JavaScript Interface) wrappers for [C++/WinRT](https://github.com/microsoft/cppwinrt) types at compile-time and the code generator was itself derived from `cppwinrt.exe`.

## Building RN/WinRT Sample

- Open a dev command prompt pointing at the root of the repo.
- Run `npm install` under ["\samples\RNWinRTTestApp\"](./samples/RNWinRTTestApp)
- Open the [`RNWinRTTestApp.sln`](./samples/RNWinRTTestApp/windows/RNWinRTTestApp.sln) solution.
- Build the x64 Release or Debug configuration.
- If running Debug: Run 'yarn start' under ["\samples\RNWinRTTestApp\"](./samples/RNWinRTTestApp)
- F5 deploy the sample app.

## Usage

Refer to the [usage docs](docs/USAGE.md) for instructions on how to use react-native-winrt in your React Native Windows app to call projected WinRT APIs in Javascript.

## Contributing

This project welcomes contributions and suggestions of all types. See our [Contributor's Guide](/CONTRIBUTING.md) for more information.

## Trademarks

This project may contain trademarks or logos for projects, products, or services. Authorized use of Microsoft trademarks or logos is subject to and must follow [Microsoft's Trademark & Brand Guidelines](https://www.microsoft.com/en-us/legal/intellectualproperty/trademarks). Use of Microsoft trademarks or logos in modified versions of this project must not cause confusion or imply Microsoft sponsorship. Any use of third-party trademarks or logos are subject to those third-party's policies.
