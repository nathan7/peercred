# vim: ft=python
{
  "targets": [
    {
      "target_name": "binding",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "sources": [
        "binding.cc",
        "credentials_linux.cc",
        "credentials_osx.cc"
      ],
      "conditions": [
        [ 'OS != "linux"', {
          "sources!": [
            "credentials_linux.cc",
          ]
        } ],
        [ 'OS != "mac"', {
          "sources!": [
            "credentials_osx.cc",
          ]
        } ],
      ]
    }
  ],
}
