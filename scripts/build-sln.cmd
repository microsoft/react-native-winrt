:: Helper to invoke msbuild on a solution, e.g. for easier consumption from something like VS Code
:: E.g. possible task config:
:: {
::     "type": "shell",
::     "label": "WinRTTests x64 Debug",
::     "command": "scripts/build-sln.cmd tests/WinRTTests/windows/WinRTTests.sln"
:: }
@echo off
setlocal
setlocal EnableDelayedExpansion

:: Argument init
set SLN_PATH=
set TGT_PLATFORM=
set TGT_CONFIG=
set TGT_TARGET=

goto :parseargs

:usage
    echo USAGE:
    echo     build-sln.cmd [arg...] <path>
    echo.
    echo ARGUMENTS:
    echo     -p <value>
    echo         Specifies the platform passed to msbuild. E.g. as '-p:Platform=<value>'. The default is 'x64'
    echo     -c <value>
    echo         Specifies the configuration passed to msbuild. E.g. as '-p:Configuration=<value>'. The default is 'DebugBundle'
    echo     -t <value>
    echo         Specifies the target passed to msbuild. E.g. as '-t:<value>'. The default is 'Build'
    goto :eof

:parseargs
    if "%1"=="" goto :build

    if "%1"=="-p" (
        if "%2"=="" echo ERROR: Missing platform value & call :usage & exit /B 1

        if "%TGT_PLATFORM%" NEQ "" (
            echo ERROR: Platform specified twice
            echo ERROR: First was '%TGT_PLATFORM%'
            echo ERROR: Second was '%2'
            call :usage & exit /B 1
        )

        set TGT_PLATFORM=%2
        shift
        shift
        goto :parseargs
    )

    if "%1"=="-c" (
        if "%2"=="" echo ERROR: Missing configuration value & call :usage & exit /B 1

        if "%TGT_CONFIG%" NEQ "" (
            echo ERROR: Configuration specified twice
            echo ERROR: First was '%TGT_CONFIG%'
            echo ERROR: Second was '%2'
            call :usage & exit /B 1
        )

        set TGT_CONFIG=%2
        shift
        shift
        goto :parseargs
    )

    if "%1"=="-t" (
        if "%2"=="" echo ERROR: Missing target value & call :usage & exit /B 1

        if "%TGT_TARGET%" NEQ "" (
            echo ERROR: Target specified twice
            echo ERROR: First was '%TGT_TARGET%'
            echo ERROR: Second was '%2'
            call :usage & exit /B 1
        )

        set TGT_TARGET=%2
        shift
        shift
        goto :parseargs
    )

    if "%SLN_PATH%"=="" (
        set SLN_PATH=%1
        shift
        goto :parseargs
    )

    echo ERROR: Two arguments were interpreted as paths to solution files. Only one expected
    echo ERROR: First was '%SLN_PATH%'
    echo ERROR: Second was '%1'
    echo.
    call :usage & exit /B 1

:build
    :: Defaults
    if "%TGT_PLATFORM%"=="" set TGT_PLATFORM=x64
    if "%TGT_CONFIG%"=="" set TGT_CONFIG=DebugBundle
    if "%TGT_TARGET%"=="" set TGT_TARGET=Build

    call msbuild -p:Platform=%TGT_PLATFORM%,Configuration=%TGT_CONFIG% -t:%TGT_TARGET% %SLN_PATH%
