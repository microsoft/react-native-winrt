# Verify deprecated and removed API support in react-native-winrt
# This script validates the TypeScript generation behavior using both the
# DeprecationTest WinMD and source-code verification.
# It tests two modes:
#   1. Without -deprecatedincluded: deprecated and removed items excluded
#   2. With -deprecatedincluded: all items present with @deprecated JSDoc annotations

param(
    [string]$rnwinrtPath,
    [string]$WinMDPath = "G:\WinRT_DeprecationTesting\WinMD\DeprecationTest.winmd"
)

$ErrorActionPreference = "Stop"
$script:passed = 0
$script:failed = 0

function Assert-True {
    param([bool]$Condition, [string]$Message)
    if ($Condition) {
        Write-Host "  PASS: $Message" -ForegroundColor Green
        $script:passed++
    } else {
        Write-Host "  FAIL: $Message" -ForegroundColor Red
        $script:failed++
    }
}

# === Source code verification ===
Write-Host "`n=== Source code: helper functions ===" -ForegroundColor Cyan

$helpersFile = Join-Path "rnwinrt\rnwinrt" "MetadataHelpers.h"
if (Test-Path $helpersFile) {
    $helpersContent = Get-Content $helpersFile -Raw
    Assert-True ($helpersContent -match "is_removed") "is_removed() helper exists"
    Assert-True ($helpersContent -match "is_deprecated") "is_deprecated() helper exists"
    Assert-True ($helpersContent -match "get_deprecated_message") "get_deprecated_message() helper exists"
}

$writerFile = Join-Path "rnwinrt\rnwinrt" "TypescriptWriter.h"
if (Test-Path $writerFile) {
    $writerContent = Get-Content $writerFile -Raw
    Assert-True ($writerContent -match "is_removed") "TypescriptWriter has is_removed() checks"
    Assert-True ($writerContent -match "WriteDeprecatedJsdoc") "TypescriptWriter has WriteDeprecatedJsdoc()"
}

# === WinMD-based generation verification ===
if (!(Test-Path $WinMDPath)) {
    Write-Host "SKIP: WinMD not found at $WinMDPath (WinMD tests skipped)" -ForegroundColor Yellow
    Write-Host "`n=== Summary ===" -ForegroundColor Cyan
    Write-Host "Passed: $($script:passed)" -ForegroundColor Green
    if ($script:failed -gt 0) { Write-Host "Failed: $($script:failed)" -ForegroundColor Red; exit 1 }
    else { Write-Host "All checks passed!" -ForegroundColor Green }
    exit 0
}

# Resolve rnwinrt path
if (-not $rnwinrtPath) {
    $candidates = @(
        (Join-Path $PSScriptRoot "..\rnwinrt\x64\Release\rnwinrt.exe"),
        (Join-Path (Get-Location) "rnwinrt\x64\Release\rnwinrt.exe")
    )
    foreach ($c in $candidates) {
        if (Test-Path $c) { $rnwinrtPath = $c; break }
    }
    if (-not $rnwinrtPath) {
        Write-Host "SKIP: rnwinrt.exe not found (WinMD tests skipped)" -ForegroundColor Yellow
        Write-Host "`n=== Summary ===" -ForegroundColor Cyan
        Write-Host "Passed: $($script:passed)" -ForegroundColor Green
        if ($script:failed -gt 0) { Write-Host "Failed: $($script:failed)" -ForegroundColor Red; exit 1 }
        else { Write-Host "All checks passed!" -ForegroundColor Green }
        exit 0
    }
}
$rnwinrtPath = (Resolve-Path $rnwinrtPath).Path
Write-Host "`nUsing rnwinrt: $rnwinrtPath"

# --- Mode 1: Without -deprecatedincluded (removed items excluded) ---
$outDir1 = Join-Path $env:TEMP "rnwinrt_verify_noincl_$(Get-Random)"
New-Item -ItemType Directory -Path $outDir1 -Force | Out-Null

Write-Host "`nGenerating TypeScript WITHOUT -deprecatedincluded..."
& $rnwinrtPath -input $WinMDPath -input local -include DeprecationTest -include Windows.Foundation -tsoutput $outDir1 2>&1 | Out-Null
$content1 = Get-Content (Join-Path $outDir1 "DeprecationTest.d.ts") -Raw

Write-Host "`n=== Mode 1 (no deprecated): Removed types excluded ===" -ForegroundColor Cyan
Assert-True ($content1 -notmatch 'class RemovedClass') "RemovedClass excluded"
Assert-True ($content1 -notmatch 'enum RemovedEnum') "RemovedEnum excluded"
Assert-True ($content1 -notmatch 'interface RemovedStruct') "RemovedStruct excluded"
Assert-True ($content1 -notmatch 'RemovedDelegate') "RemovedDelegate excluded"
Assert-True ($content1 -notmatch 'IRemovedInterface') "IRemovedInterface excluded"

Write-Host "`n=== Mode 1 (no deprecated): Normal types present ===" -ForegroundColor Cyan
Assert-True ($content1 -match 'class TestComponent') "TestComponent present"
Assert-True ($content1 -match 'enum NormalEnum') "NormalEnum present"
Assert-True ($content1 -match 'interface NormalStruct') "NormalStruct present"
Assert-True ($content1 -match 'INormalInterface') "INormalInterface present"

Write-Host "`n=== Mode 1 (no deprecated): Removed methods on TestComponent excluded ===" -ForegroundColor Cyan
Assert-True ($content1 -notmatch 'getEpsilon') "getEpsilon (removed) excluded"
Assert-True ($content1 -notmatch 'getZeta') "getZeta (removed) excluded"
Assert-True ($content1 -match 'getAlpha') "getAlpha (normal) present"
Assert-True ($content1 -match 'getBeta') "getBeta (normal) present"

Write-Host "`n=== Mode 1 (no deprecated): Removed static methods excluded ===" -ForegroundColor Cyan
Assert-True ($content1 -match 'staticMethod') "staticMethod (normal) present"
Assert-True ($content1 -notmatch 'staticRemovedMethod') "staticRemovedMethod excluded"

Write-Host "`n=== Mode 1 (no deprecated): Removed properties excluded ===" -ForegroundColor Cyan
Assert-True ($content1 -match 'normalProp') "normalProp (normal) present"
Assert-True ($content1 -notmatch 'removedProp') "removedProp excluded"
Assert-True ($content1 -match 'writableProp') "writableProp (normal) present"
Assert-True ($content1 -notmatch 'writableRemovedProp') "writableRemovedProp excluded"

Write-Host "`n=== Mode 1 (no deprecated): Removed static properties excluded ===" -ForegroundColor Cyan
Assert-True ($content1 -match 'staticProp') "staticProp (normal) present"
Assert-True ($content1 -notmatch 'staticRemovedProp') "staticRemovedProp excluded"

Write-Host "`n=== Mode 1 (no deprecated): Removed events excluded ===" -ForegroundColor Cyan
Assert-True ($content1 -match 'normalevent') "normalEvent present"
Assert-True ($content1 -notmatch 'removedevent') "removedEvent excluded"

Write-Host "`n=== Mode 1 (no deprecated): Removed constructors excluded ===" -ForegroundColor Cyan
Assert-True ($content1 -match 'constructor\(\)') "Default constructor present"
Assert-True ($content1 -notmatch 'constructor\(name: string, config: number\)') "Removed constructor excluded"

# --- Mode 2: With -deprecatedincluded (all items present with annotations) ---
$outDir2 = Join-Path $env:TEMP "rnwinrt_verify_incl_$(Get-Random)"
New-Item -ItemType Directory -Path $outDir2 -Force | Out-Null

Write-Host "`nGenerating TypeScript WITH -deprecatedincluded..."
& $rnwinrtPath -input $WinMDPath -input local -include DeprecationTest -include Windows.Foundation -tsoutput $outDir2 -deprecatedincluded 2>&1 | Out-Null
$content2 = Get-Content (Join-Path $outDir2 "DeprecationTest.d.ts") -Raw

Write-Host "`n=== Mode 2 (deprecated included): Removed types have @deprecated ===" -ForegroundColor Cyan
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+RemovedClass.*?\*/\s*class RemovedClass') `
    "RemovedClass has @deprecated annotation"
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+RemovedEnum.*?\*/\s*enum RemovedEnum') `
    "RemovedEnum has @deprecated annotation"

Write-Host "`n=== Mode 2 (deprecated included): Deprecated types have @deprecated ===" -ForegroundColor Cyan
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+DeprecatedClass.*?\*/\s*class DeprecatedClass') `
    "DeprecatedClass has @deprecated annotation"
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+DeprecatedEnum.*?\*/\s*enum DeprecatedEnum') `
    "DeprecatedEnum has @deprecated annotation"

Write-Host "`n=== Mode 2 (deprecated included): Deprecated methods on TestComponent ===" -ForegroundColor Cyan
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+GetGamma.*?\*/\s*public getGamma') `
    "getGamma has @deprecated annotation"
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+GetEpsilon.*?\*/\s*public getEpsilon') `
    "getEpsilon (removed) has @deprecated annotation"

Write-Host "`n=== Mode 2 (deprecated included): Static methods annotated ===" -ForegroundColor Cyan
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+StaticDeprecatedMethod.*?\*/\s*public static staticDeprecatedMethod') `
    "staticDeprecatedMethod has @deprecated"
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+StaticRemovedMethod.*?\*/\s*public static staticRemovedMethod') `
    "staticRemovedMethod has @deprecated"

Write-Host "`n=== Mode 2 (deprecated included): Events annotated ===" -ForegroundColor Cyan
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+DeprecatedEvent.*?\*/\s*public addEventListener\(type: "deprecatedevent"') `
    "DeprecatedEvent has @deprecated"
Assert-True ($content2 -match '(?s)/\*\*\s*@deprecated\s+RemovedEvent.*?\*/\s*public addEventListener\(type: "removedevent"') `
    "RemovedEvent has @deprecated"

# Cleanup
Remove-Item -Recurse -Force $outDir1 -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force $outDir2 -ErrorAction SilentlyContinue

Write-Host "`n=== Summary ===" -ForegroundColor Cyan
Write-Host "Passed: $($script:passed)" -ForegroundColor Green
if ($script:failed -gt 0) {
    Write-Host "Failed: $($script:failed)" -ForegroundColor Red
    exit 1
} else {
    Write-Host "All checks passed!" -ForegroundColor Green
}
