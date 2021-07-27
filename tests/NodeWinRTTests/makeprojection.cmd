@echo off
setlocal
setlocal EnableDelayedExpansion

set CURRENT_DIR=%~dp0
set OUTPUT_DIR=%CURRENT_DIR%\Generated Files
set NUGET_DIR=%CURRENT_DIR%\packages

set JSWINRT_BASE_DIR=%CURRENT_DIR%\..\..\jswinrt\x64
set JSWINRT_ARGS=-nodejs -webhosthiddenincluded -input sdk -output "%OUTPUT_DIR%"
set JSWINRT_ARGS=%JSWINRT_ARGS% -include Windows.Foundation
set JSWINRT_ARGS=%JSWINRT_ARGS% -include Windows.ApplicationModel
set JSWINRT_ARGS=%JSWINRT_ARGS% -include Windows.Management

set CPPWINRT_VERSION=2.0.201113.7
set CPPWINRT_NUGET_DIR=%NUGET_DIR%\Microsoft.Windows.CppWinRT.%CPPWINRT_VERSION%
set CPPWINRT_EXE_DIR=%CPPWINRT_NUGET_DIR%\bin
set CPPWINRT_ARGS=-input sdk -output "%OUTPUT_DIR%"

:: Options
set BUILD_TYPE=Release

goto :parseargs

:usage
    echo USAGE:
    echo     buildprojection.cmd [debug]
    echo.
    echo ARGUMENTS:
    echo     debug
    echo         Specifies that the debug build of the jswinrt tool should be used. By default the Release version is
    echo         used. Regardless, the x64 version is always used.
    goto :eof

:parseargs
    if "%1"=="" goto :execute

    if "%1"=="debug" (
        set BUILD_TYPE=Debug
        shift
        goto :parseargs
    )

    echo ERROR: Unrecognized argument '%1'
    echo.
    call :usage & exit /B 1

:execute
    :: Install the nuget package if it doesn't already exist
    if NOT exist "%CPPWINRT_NUGET_DIR%" (
        nuget install -OutputDirectory "%NUGET_DIR%"
    )

    "%CPPWINRT_EXE_DIR%\cppwinrt.exe" %CPPWINRT_ARGS%

    set JSWINRT_DIR=%JSWINRT_BASE_DIR%\%BUILD_TYPE%
    "%JSWINRT_DIR%\jswinrt.exe" %JSWINRT_ARGS%

    goto :eof
