## Building and Integrating

### Prerequisites

* Clang 18+ or GCC 13+
* CMake 3.22+
* libcurl development files
* nlohmann_json headers

### Compilation

```bash
cmake -DCMAKE_CXX_COMPILER=clang++ -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```