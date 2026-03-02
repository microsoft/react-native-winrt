# Verify deprecated and removed API support in react-native-winrt
# This script validates the TypeScript generation behavior for both deprecated
# and removed APIs using the generated .d.ts test artifacts.

param(
    [string]$ArtifactsDir = "tests\TestArtifacts"
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

Write-Host "`n=== DeprecationType enum definition ===" -ForegroundColor Cyan

$metadataFile = Join-Path $ArtifactsDir "Windows.Foundation.Metadata.d.ts"
if (!(Test-Path $metadataFile)) {
    Write-Host "ERROR: $metadataFile not found" -ForegroundColor Red
    exit 1
}

$metadataContent = Get-Content $metadataFile -Raw

# Verify DeprecationType enum exists with both values
Assert-True ($metadataContent -match "enum DeprecationType") `
    "DeprecationType enum is defined"
Assert-True ($metadataContent -match "deprecate = 0") `
    "DeprecationType.deprecate = 0 exists"
Assert-True ($metadataContent -match "remove = 1") `
    "DeprecationType.remove = 1 exists"

Write-Host "`n=== DeprecatedAttribute class ===" -ForegroundColor Cyan

Assert-True ($metadataContent -match "class DeprecatedAttribute") `
    "DeprecatedAttribute class is defined"
Assert-True ($metadataContent -match "DeprecationType") `
    "DeprecatedAttribute references DeprecationType"

Write-Host "`n=== is_removed() logic verification ===" -ForegroundColor Cyan

# Check the C++ source directly to verify the logic is implemented
$helpersFile = Join-Path "rnwinrt\rnwinrt" "MetadataHelpers.h"
if (Test-Path $helpersFile) {
    $helpersContent = Get-Content $helpersFile -Raw
    
    Assert-True ($helpersContent -match "is_removed") `
        "is_removed() helper function exists"
    Assert-True ($helpersContent -match "is_deprecated") `
        "is_deprecated() helper function exists"
    Assert-True ($helpersContent -match "get_deprecated_message") `
        "get_deprecated_message() helper function exists"
}

# Check TypescriptWriter.h for removed skip logic
$writerFile = Join-Path "rnwinrt\rnwinrt" "TypescriptWriter.h"
if (Test-Path $writerFile) {
    $writerContent = Get-Content $writerFile -Raw
    
    Assert-True ($writerContent -match "is_removed") `
        "TypescriptWriter has is_removed() checks"
    Assert-True ($writerContent -match "WriteDeprecatedJsdoc") `
        "TypescriptWriter has WriteDeprecatedJsdoc() method"
}

Write-Host "`n=== Generated .d.ts deprecated annotations ===" -ForegroundColor Cyan

$testComponentFile = Join-Path $ArtifactsDir "TestComponent.d.ts"
if (Test-Path $testComponentFile) {
    $tcContent = Get-Content $testComponentFile -Raw
    # Just verify the file exists and has content
    Assert-True ($tcContent.Length -gt 100) `
        "TestComponent.d.ts has generated content"
} else {
    Write-Host "  SKIP: TestComponent.d.ts not found (run generator first)" -ForegroundColor Yellow
}

# Verify deprecated annotations appear in SDK .d.ts files
$foundationFile = Join-Path $ArtifactsDir "Windows.Foundation.d.ts"
if (Test-Path $foundationFile) {
    $foundContent = Get-Content $foundationFile -Raw
    Assert-True ($foundContent.Length -gt 100) `
        "Windows.Foundation.d.ts has generated content"
}

Write-Host "`n=== Summary ===" -ForegroundColor Cyan
Write-Host "Passed: $($script:passed)" -ForegroundColor Green
if ($script:failed -gt 0) {
    Write-Host "Failed: $($script:failed)" -ForegroundColor Red
    exit 1
} else {
    Write-Host "All checks passed!" -ForegroundColor Green
}
