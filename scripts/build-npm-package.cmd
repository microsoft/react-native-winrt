@echo off
SetLocal
SetLocal EnableDelayedExpansion

set ROOT_DIR=%~dp0..
set PACKAGE_DIR=%ROOT_DIR%\package
set RESULT=0

:: NOTE: All of the following are relative to 'ROOT_DIR' and 'PACKAGE_DIR'
call :copy_file LICENSE LICENSE
call :copy_file rnwinrt\js\WinRTTurboModule.js index.js
exit /B %RESULT%

:copy_file
    copy %ROOT_DIR%\%1 %PACKAGE_DIR%\%2 > NUL
    if %ERRORLEVEL% NEQ 0 set RESULT=%ERRORLEVEL%
    exit /B %ERRORLEVEL%
