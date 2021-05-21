[CmdLetBinding()]
Param(
    [string] $buildDir
)

Copy-Item $buildDir\tests\TestArtifacts\ExperimentalFeatures.props -Destination $buildDir\tests\WinRTTests\windows