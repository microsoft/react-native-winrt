@echo off
setlocal
setlocal EnableDelayedExpansion

set ROOT_DIR=%~dp0\..

set DIRS=rnwinrt/module rnwinrt/strings rnwinrt/rnwinrt rnwinrt/rnwinrt/strings rnwinrt/strings
set DIRS=%DIRS% samples/windows/RNWinRTTestApp samples/windows/WinRTTurboModule
set DIRS=%DIRS% tests/WinRTTests/windows/TestComponent tests/WinRTTests/windows/WinRTTests tests/WinRTTests/windows/WinRTTurboModule
set EXTS=.cpp .h

for %%d in (%DIRS%) do (
    for %%e in (%EXTS%) do (
        for %%f in (%ROOT_DIR%\%%d\*%%e) do clang-format -style=file -i %%f
    )
)
