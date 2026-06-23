## Building and Integrating

### Prerequisites

* Clang 18+ or GCC 13+
* CMake 3.22+
* libcurl development files
* nlohmann_json headers

### IDE Configuration
#### Visual Studio Code
`.vscode/settings.json`

```json
{
  "clangd.arguments": [
    "--compile-commands-dir=build",
    "--header-insertion=never",
    "--clang-tidy=false"
  ]
}
```

### Compilation

```bash
cmake -DCMAKE_CXX_COMPILER=clang++ -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Alternatively
CC=clang CXX=clang++ cmake .
cmake --build .
```