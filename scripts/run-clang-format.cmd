@echo off
setlocal
setlocal EnableDelayedExpansion

set ROOT_DIR=%~dp0\..

set DIRS=rnwinrt/module rnwinrt/strings rnwinrt/rnwinrt rnwinrt/rnwinrt/react rnwinrt/rnwinrt/react/strings
set DIRS=%DIRS% samples/windows/RNWinRTTestApp samples/windows/WinRTTurboModule
set DIRS=%DIRS% tests/RnWinRTTests/windows/TestComponent tests/RnWinRTTests/windows/RnWinRTTests tests/RnWinRTTests/windows/WinRTTurboModule
set EXTS=.cpp .h

for %%d in (%DIRS%) do (
    for %%e in (%EXTS%) do (
        for %%f in (%ROOT_DIR%\%%d\*%%e) do clang-format -style=file -i %%f
    )
)
