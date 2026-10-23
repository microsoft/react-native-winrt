<#
.SYNOPSIS
    Verifies that @deprecated JSDoc annotations are correctly generated for deprecated WinRT types.

.DESCRIPTION
    Runs rnwinrt.exe against system WinMD with -deprecatedincluded flag and verifies that
    deprecated types and members have /** @deprecated ... */ JSDoc annotations in the
    generated TypeScript declaration files.

.PARAMETER rnwinrtPath
    Path to rnwinrt.exe. Defaults to the x64 Release build location.
#>
[CmdLetBinding()]
Param(
    [string] $rnwinrtPath
)

$ErrorActionPreference = "Stop"
$passed = 0
$failed = 0

function Test-Check {
    param([string]$Name, [bool]$Condition)
    if ($Condition) {
        Write-Host "  PASS: $Name" -ForegroundColor Green
        $script:passed++
    } else {
        Write-Host "  FAIL: $Name" -ForegroundColor Red
        $script:failed++
    }
}

# Resolve rnwinrt path
if (-not $rnwinrtPath) {
    # Try common locations relative to script
    $candidates = @(
        (Join-Path $PSScriptRoot "..\..\rnwinrt\x64\Release\rnwinrt.exe"),
        (Join-Path (Get-Location) "rnwinrt\x64\Release\rnwinrt.exe")
    )
    foreach ($c in $candidates) {
        if (Test-Path $c) { $rnwinrtPath = $c; break }
    }
    if (-not $rnwinrtPath) {
        throw "Cannot find rnwinrt.exe. Pass -rnwinrtPath explicitly."
    }
}
$rnwinrtPath = (Resolve-Path $rnwinrtPath).Path
Write-Host "Using rnwinrt: $rnwinrtPath"

# Create temp output directory
$outDir = Join-Path $env:TEMP "rnwinrt_deprecated_verify_$(Get-Random)"
New-Item -ItemType Directory -Path $outDir -Force | Out-Null

try {
    Write-Host "`nGenerating TypeScript for Windows.Media.PlayTo (deprecated namespace)..."
    & $rnwinrtPath -input local -include Windows.Media.PlayTo -tsoutput $outDir -deprecatedincluded
    if ($LASTEXITCODE -ne 0) {
        throw "rnwinrt.exe failed with exit code $LASTEXITCODE"
    }

    $playToFile = Join-Path $outDir "Windows.Media.PlayTo.d.ts"
    if (-not (Test-Path $playToFile)) {
        throw "Expected output file not generated: $playToFile"
    }

    $content = Get-Content $playToFile -Raw

    Write-Host "`n=== Deprecated Type Annotations ==="

    # 1. PlayToConnection class should have @deprecated
    Test-Check "PlayToConnection class has @deprecated" `
        ($content -match '(?s)/\*\* @deprecated PlayToConnection.*?\*/\s*class PlayToConnection')

    # 2. PlayToManager class should have @deprecated
    Test-Check "PlayToManager class has @deprecated" `
        ($content -match '(?s)/\*\* @deprecated PlayToManager.*?\*/\s*class PlayToManager')

    # 3. PlayToSource class should have @deprecated
    Test-Check "PlayToSource class has @deprecated" `
        ($content -match '(?s)/\*\* @deprecated PlayToSource.*?\*/\s*class PlayToSource')

    # 4. PlayToSourceDeferral class should have @deprecated
    Test-Check "PlayToSourceDeferral class has @deprecated" `
        ($content -match '(?s)/\*\* @deprecated PlayToSourceDeferral.*?\*/\s*class PlayToSourceDeferral')

    # 5. PlayToSourceRequest class should have @deprecated
    Test-Check "PlayToSourceRequest class has @deprecated" `
        ($content -match '(?s)/\*\* @deprecated PlayToSourceRequest.*?\*/\s*class PlayToSourceRequest')

    Write-Host "`n=== Deprecated Enum Annotations ==="

    # 6. PlayToConnectionState enum should have deprecated values
    Test-Check "PlayToConnectionState has @deprecated enum values" `
        ($content -match '(?s)/\*\* @deprecated PlayToConnectionState.*?\*/\s*(disconnected|connected|rendering)')

    # 7. PlayToConnectionError enum should have deprecated values
    Test-Check "PlayToConnectionError has @deprecated enum values" `
        ($content -match '(?s)/\*\* @deprecated PlayToConnectionError.*?\*/\s*(none|deviceNotResponding|deviceError)')

    Write-Host "`n=== Deprecated Method/Property Annotations ==="

    # 8. Methods on deprecated classes should have @deprecated
    $deprecatedCount = ([regex]::Matches($content, '/\*\* @deprecated')).Count
    Test-Check "Multiple @deprecated annotations generated (found $deprecatedCount)" `
        ($deprecatedCount -ge 20)

    # 9. Non-deprecated types should NOT have @deprecated
    # CurrentTimeChangeRequestedEventArgs is NOT deprecated
    $nonDeprecatedSection = $content | Select-String -Pattern 'class CurrentTimeChangeRequestedEventArgs' -Context 2
    if ($nonDeprecatedSection) {
        $contextText = $nonDeprecatedSection.Context.PreContext -join "`n"
        Test-Check "Non-deprecated CurrentTimeChangeRequestedEventArgs lacks @deprecated" `
            ($contextText -notmatch '@deprecated')
    } else {
        Test-Check "Non-deprecated CurrentTimeChangeRequestedEventArgs lacks @deprecated" $true
    }

    Write-Host "`n=== TypeScript Validity ==="

    # 10. File should be valid TypeScript structure (starts with declare namespace)
    Test-Check "Output is valid TypeScript declaration structure" `
        ($content -match 'declare namespace Windows\.Media\.PlayTo')

    Write-Host "`n=== Results ==="
    Write-Host "$passed passed, $failed failed out of $($passed + $failed) checks"

    if ($failed -gt 0) {
        throw "$failed check(s) failed"
    }

    Write-Host "`nAll checks passed!" -ForegroundColor Green
}
finally {
    Remove-Item -Recurse -Force $outDir -ErrorAction SilentlyContinue
}
