@echo off
SetLocal
SetLocal EnableDelayedExpansion

set ROOT_DIR=%~dp0..
pushd %ROOT_DIR%\package
    set PACKAGE_ROOT_DIR=%CD%
popd

:: Perform any build tasks that may be needed
call %ROOT_DIR%\scripts\build-npm-package.cmd
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to build the npm package [error %ERRORLEVEL%]
    exit /B %ERRORLEVEL%
)

:: Do a dry run to verify that we're not publishing a ton of unnecessary files
pushd %PACKAGE_ROOT_DIR%
    call npx --yes npm-packlist > packfiles.new
    sort packfiles.new > packfiles.new.temp
    move /Y packfiles.new.temp packfiles.new > NUL
    fc /c packfiles.old packfiles.new > NUL 2>&1
    set NEW_FILES_CHECK=%ERRORLEVEL%
popd

if %NEW_FILES_CHECK% NEQ 0 (
    echo ERROR: The files that would get published to npm.org does not match the set of expected files. This might be
    echo        expected if new files have been added/renamed/etc. This may also be a consequence of the output format
    echo        of 'npx npm-packlist' changing. You can view the differences by running:
    echo.
    echo            fc /c %PACKAGE_ROOT_DIR%\packfiles.old %PACKAGE_ROOT_DIR%\packfiles.new
    echo.
    echo        If the differences look correct/okay, you can copy the files from 'packfiles.new' to 'packfiles.old' and
    echo        re-run this script.
    exit /B %NEW_FILES_CHECK%
) else (
    del %PACKAGE_ROOT_DIR%\packfiles.new
)

pushd %PACKAGE_ROOT_DIR%
    echo Running 'npm publish'...
    call npm publish
    set EXIT_CODE=%ERRORLEVEL%
popd

exit /B %EXIT_CODE%
