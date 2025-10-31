{
  "targets": [
    {
      "target_name": "winrtaddon",
      "sources": [
        "src/addon.cpp",
        "<!@(node scripts/list_generated_cpp.js)"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "src",
        "int/inc",
        "int/nuget-packages/Microsoft.WindowsAppSDK.Foundation.1.8.250906002/include"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}
