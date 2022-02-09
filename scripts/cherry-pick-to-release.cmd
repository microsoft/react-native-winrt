@echo off
setlocal
setlocal EnableDelayedExpansion

if "%1"=="" (
    echo ERROR: Missing commit argument
    exit /B 1
)
set COMMIT=%1

set SUPPORTED_RELEASES=0.64 0.65 0.66 0.67

for %%v in (%SUPPORTED_RELEASES%) do (
    call :cherrypick_to_release %%v
    if !ERRORLEVEL! NEQ 0 (
        exit /B !ERRORLEVEL!
    )
    echo.
)

goto :eof

:cherrypick_to_release
    set BRANCHNAME=cherrypick_%COMMIT%_to_%1
    git checkout -b %BRANCHNAME% origin/release/%1
    git push -u origin %BRANCHNAME%
    git cherry-pick %COMMIT%
    if !ERRORLEVEL! NEQ 0 (
        echo ERROR: cherry-pick failed. Please check the commit id and try again or perform this process manually
        exit /B !ERRORLEVEL!
    )

    git push
    :: TODO: How to delete these branches locally? Do it at the end after checking out master?
    :: git branch -d %BRANCHNAME%
