@echo off
setlocal
setlocal EnableDelayedExpansion

set ROOT_DIR=%~dp0\..

set DIRS=jswinrt/module jswinrt/strings jswinrt/jswinrt jswinrt/jswinrt/strings jswinrt/strings
set DIRS=%DIRS% samples/windows/RNWinRTTestApp samples/windows/WinRTTurboModule
set DIRS=%DIRS% tests/WinRTTests/windows/TestComponent tests/WinRTTests/windows/WinRTTests tests/WinRTTests/windows/WinRTTurboModule
set EXTS=.cpp .h

for %%d in (%DIRS%) do (
    for %%e in (%EXTS%) do (
        for %%f in (%ROOT_DIR%\%%d\*%%e) do clang-format -style=file -i %%f
    )
)
