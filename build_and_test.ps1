
$ErrorActionPreference = "Stop"

# change dir to script root
pushd $PSScriptRoot

function Get-MSBuildPath {
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $path = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe
        if ($path) { return $path }
    }
    $cmd = Get-Command msbuild.exe -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    return $null
}

$msbuildPath = Get-MSBuildPath
Write-Host "MSBuild Path: $msbuildPath"
    

nuget restore rnwinrt

Write-Host "Building rnwinrt" -ForegroundColor Cyan

# build x64
& $msbuildPath /p:Platform=x64 rnwinrt/rnwinrt.sln /v:q

Write-Host "Building rnwinrt tests" -ForegroundColor Cyan

& $msbuildPath /p:Platform=x64 tests\RnWinRTTests\windows\TestComponent\TestComponent.vcxproj /v:q


Write-Host "Building NodeTestApp" -ForegroundColor Cyan
pushd samples\NodeTestApp
npm i
npm run rebuild
popd


popd
