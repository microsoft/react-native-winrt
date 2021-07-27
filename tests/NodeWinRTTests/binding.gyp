{
  "targets": [
    {
      "target_name": "winrt",
      "sources": [
        "pch.cpp",
        "module.cpp",
        "<!@(python -c \"import os; print('\\n'.join(['\\\"Generated Files/nodewinrt/%s\\\"' % x for x in os.listdir('./Generated Files/nodewinrt') if x[-4:] == '.cpp']))\")"
      ],
      "include_dirs": [
        "./",
        "Generated Files/"
      ],
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1,
          "AdditionalOptions": [
            "-std:c++17",
            "/Zc:externConstexpr"
          ]
        }
      },
      "msvs_precompiled_header": "pch.h",
      "msvs_precompiled_source": "pch.cpp"
    }
  ]
}