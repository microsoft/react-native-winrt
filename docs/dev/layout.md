
# React Native WinRT Repo Layout

The following is a list of important directories and files, along with a short description of each one's purpose.
More information on each can be found below.

* [`package/`](../../package) - The directory whose contents are used for publishing the [NPM package](./releases.md#publishing-on-npm).
  * [`packfiles.old`](../../package/packfiles.old) - List of files that are expected to be included in the NPM package. Used for validation before publishing a new package.
* [`rnwinrt/`](../../rnwinrt)
  * [`js/`](../../rnwinrt/js) - The definition of [WinRTTurboModule.js](../../rnwinrt/js/WinRTTurboModule.js) that gets included in the NPM package as `index.js`. This file can also be used as a reference (or copied locally) when using the Nuget package.
  * [`module/`](../../rnwinrt/module) - Source code for building the Turbo Module DLL that gets included in the NPM package. This also serves as a reference when using the Nuget package.
  * [`nuget/`](../../rnwinrt/nuget) - Files specific to creating the Nuget package.
  * [`rnwinrt/`](../../rnwinrt/rnwinrt) - Contains the source code for compiling the [rnwinrt.exe](#rnwinrtexe-executable) executable.
    * [`react/`](../../rnwinrt/rnwinrt/react) - Contains the source code specific to generating React Native Windows projection code.
      * [`strings/`](../../rnwinrt/rnwinrt/react/strings) - The definitions of [base.h](../../rnwinrt/rnwinrt/react/strings/base.h)/[base.cpp](../../rnwinrt/rnwinrt/react/strings/base.cpp) that get compiled into the `rnwinrt.exe` executable.
  * [`strings/`](../../rnwinrt/strings) - Contains the source code for compiling the [strings.exe](#stringsexe-executable) executable.
* [`samples/`](../../samples)
  * [`RNWinRTTestApp/`](../../samples/RNWinRTTestApp) - A [sample RNW app](#sample-application) showcasing the consumption of several WinRT APIs.
* [`scripts/`](../../scripts)
  * [`build-npm-package.cmd`](../../scripts/build-npm-package.cmd) - Builds the `rnwinrt.exe` executable and copies it, along with other essential files, to their proper locations in the `package/` directory.
  * [`mdExts.js`](../../scripts/mdExts.js) - WinDBG debug visualizer script for the [WinMD reader](https://github.com/microsoft/winmd) types. Very helpful when debugging `rnwinrt.exe`.
  * [`publish-npm-package.cmd`](../../scripts/publish-npm-package.cmd) - Runs `build-npm-package.cmd`, followed by `npm publish` if a quick validation succeeds.
  * [`run-clang-format.cmd`](../../scripts/run-clang-format.cmd) - Runs `clang-format` on all source files. Run this before issuing a PR.
* [`sdk/`](../../sdk)
  * [`react/`](../../sdk/react) - Project that builds a [test Turbo Module for the entire Windows SDK](#test-sdk-turbo-module).
* [`tests/`](../../tests)
  * [`RnWinRTTests/`](../../tests/RnWinRTTests) - [Test application](#test-application) for verifying changes and CI validation.
    * [`windows/`](../../tests/RnWinRTTests/windows)
      * [`RnWinRTTests/`](../../tests/RnWinRTTests/windows/RnWinRTTests) - Standard application code needed for all React Native Windows apps.
      * [`TestComponent/`](../../tests/RnWinRTTests/windows/TestComponent) - Project to build a [WinRT DLL](#test-winrt-dll) used for the tests.
      * [`WinRTTurboModule/`](../../tests/RnWinRTTests/windows/WinRTTurboModule) - Project to build the Turbo Module DLL used by the tests. The tests _do not_ consume rnwinrt "as if" it were an NPM package.
  * [`TestArtifacts/`](../../tests/TestArtifacts) - The TypeScript files that are expected to be generated as part of compiling the `RnWinRTTests` test application. This is to make PR validation of TypeScript changes easier.

# rnwinrt.exe Executable
The `rnwinrt.exe` executable is responsible for converting WinMD input into C++ source files that can be compiled into a React Native Turbo Module, exposing the desired WinRT types in JavaScript.
This project makes use of the [WinMD reader](https://github.com/microsoft/winmd) project for parsing the WinMD.
The code it generates uses JSI for interoperating with JavaScript and has a dependency on [C++/WinRT](https://github.com/microsoft/winmd) for interfacing with WinRT types.
For more specifics on the layout of the projection, see [here](projection.md).

# strings.exe Executable
`rnwinrt.exe` is intended to be distributed as a standalone executable, however there are two outputs - `base.h` and `base.cpp` - that have static content.
The contents of these two files must therefore get compiled into the `rnwinrt.exe` binary.
The `strings.exe` binary is used to convert these two files into string literals that can then be linked into `rnwinrt.exe`.
In practice, these two files are actually converted into two arrays of string literals since the compiler has a limit on the maximum length of string literals.
The generated file ends up looking something like:
```c++
namespace rnwinrt {
struct file_data
{
    const char* file_name;
    std::initializer_list<std::string_view> file_contents;
};

inline const file_data file_strings[] =
{
    {
        "base.cpp",
        {
            R"xyz(file contents...)xyz",
            R"xyz(...more file contents...)xyz",
            ...
        }
    },
    {
        "base.h",
        {
            R"xyz(file contents...)xyz",
            R"xyz(...more file contents...)xyz",
            ...
        }
    }
};
```

# Test Application
The React Native WinRT project is tested using a React Native Windows application that consumes a custom WinRT DLL specifically formulated to test interesting scenarios related to projecting types into JavaScript.
The application itself displays status as well as results regarding test executation, with options to re-run tests for easier debugging.
The application writes any tests that fail to the file `FailureLog.txt`, which is used by the CI pipeline to determine if the tests pass or fail.

## Test WinRT DLL
In order to easily test different features of the projection, we author our own WinRT DLL that gets projected into JavaScript.
This (1) allows us to test scenarios that may not occur in the Windows SDK, and (2) removes our dependency on the behavior of APIs whose implementation we don't control.
The definition of the types and functions that the test uses can be seen in [`TestComponent.idl`](../../tests/RnWinRTTests/windows/TestComponent/TestComponent.idl).
Some examples of interesting scenarios that this tests include: static & non-static functions/properties/events, calling constructors, function & constructor overloads, arrays and array-like types (`IVector`, etc.), async types and callbacks (`IAsync*`), structs, `out` params, among many others.

# Sample Application
The sample application is intended to be an exaple of (1) how to consume the `react-native-winrt` NPM package (albeit through a directory symlink), and (2) how projected APIs can be used in a more "realistic" context.
Since the sample app simulates using the NPM package, the local package must first be built by running the [`scripts/build-npm-package.cmd`](../../scripts/build-npm-package.cmd) script.

# Test SDK Turbo Module
The SDK Turbo Module serves as a sanity check to ensure that we can project the entire Windows SDK and compile it without any errors.
Currently, this is something that is only run locally; it is not built as a part of CI validation.
This is also not something that is intended to be consumed by any RNW application; the binary size is significantly larger than any application should realistically need.
