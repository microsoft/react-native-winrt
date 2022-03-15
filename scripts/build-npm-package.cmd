@echo off
SetLocal
SetLocal EnableDelayedExpansion

set ROOT_DIR=%~dp0..
set PACKAGE_DIR=%ROOT_DIR%\package

:: Build the rnwinrt executable so that we can copy it later
call nuget restore %ROOT_DIR%\rnwinrt\rnwinrt.sln > NUL
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to run nuget restore on rnwinrt.sln
    exit /B %ERRORLEVEL%
)
call msbuild %ROOT_DIR%\rnwinrt\rnwinrt.sln -p:Platform=x64;Configuration=Release > NUL
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to build rnwinrt.sln
    exit /B %ERRORLEVEL%
)

:: NOTE: All of the following are relative to 'ROOT_DIR' and 'PACKAGE_DIR'
set RESULT=0
call :copy_file LICENSE LICENSE
call :copy_file rnwinrt\js\WinRTTurboModule.js index.js

call :copy_file rnwinrt\x64\Release\rnwinrt.exe bin\rnwinrt.exe
call :copy_file rnwinrt\x64\Release\rnwinrt.pdb bin\rnwinrt.pdb

call :copy_file rnwinrt\module\dllmain.cpp module\dllmain.cpp
call :copy_file rnwinrt\module\WinRTTurboModule.h module\WinRTTurboModule.h
call :copy_file rnwinrt\module\WinRTTurboModule.cpp module\WinRTTurboModule.cpp
call :copy_file rnwinrt\module\WinRTTurboModule.def module\WinRTTurboModule.def
call :copy_file rnwinrt\module\WinRTTurboModule.props module\WinRTTurboModule.props
call :copy_file rnwinrt\module\WinRTTurboModule.targets module\WinRTTurboModule.targets
exit /B %RESULT%

:copy_file
    echo f | xcopy /y %ROOT_DIR%\%1 %PACKAGE_DIR%\%2 > NUL
    if %ERRORLEVEL% NEQ 0 set RESULT=%ERRORLEVEL%
    exit /B %ERRORLEVEL%
