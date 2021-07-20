{
  "targets": [
    {
      "target_name": "winrt",
      "sources": [
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
      }
    }
  ]
}