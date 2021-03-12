[CmdLetBinding()]
Param(
    [string] $expectedFilesDir,
    [string] $generatedFilesDir
)

function compareFiles
{
    param (
        [string] $fileName,
        [string] $expectedFilesDir,
        [string] $generatedFilesDir)

    $test = compare-object (get-content $expectedFilesDir\$fileName) (get-content $generatedFilesDir\$fileName);
    if ($test) {
        throw "Generated TypeScript differs from expected output"
    }
}


compareFiles "TestComponent.d.ts" $expectedFilesDir $generatedFilesDir
compareFiles "Windows.d.ts" $expectedFilesDir $generatedFilesDir
compareFiles "Windows.Foundation.Collections.d.ts" $expectedFilesDir $generatedFilesDir
compareFiles "Windows.Foundation.d.ts" $expectedFilesDir $generatedFilesDir
compareFiles "Windows.Foundation.Diagnostics.d.ts" $expectedFilesDir $generatedFilesDir
compareFiles "Windows.Foundation.Metadata.d.ts" $expectedFilesDir $generatedFilesDir
compareFiles "Windows.Foundation.Numerics.d.ts" $expectedFilesDir $generatedFilesDir