{
  "targets": [
    {
      "target_name": "winrtaddon",
      "sources": [
        "src/pch.cpp",
        "src/addon.cpp",
        "<!@(node scripts/list_generated_cpp.js)"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "src",
        "int/inc",
        "int/nuget-packages/Microsoft.WindowsAppSDK.Foundation.1.8.250906002/include"
      ],
      "defines": [ "NAPI_CPP_EXCEPTIONS" ],
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": "1",
          "AdditionalOptions": [ "/GR-" ],
          "PrecompiledHeader": "Use",
          "PrecompiledHeaderFile": "pch.h",
          "ForcedIncludeFiles": [ "pch.h" ]
        },
        "VCLinkerTool": {
          "AdditionalDependencies": [
            "windowsapp.lib",
            "rpcrt4.lib"
          ]
        }
      },
      "msvs_precompiled_header": "src/pch.h",
      "msvs_precompiled_source": "src/pch.cpp"
    }
  ]
}
