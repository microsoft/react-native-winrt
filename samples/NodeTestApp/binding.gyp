{
  "targets": [
    {
      "target_name": "winrtaddon",
      "sources": [ "src/addon.cpp" ],
      "include_dirs": [ "<!@(node -p \"require('node-addon-api').include\")" ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}
