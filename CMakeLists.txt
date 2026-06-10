cmake_minimum_required(VERSION 3.14)
project(mlflow-cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)

include(FetchContent)
fetchcontent_declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
fetchcontent_declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
fetchcontent_makeavailable(json googletest)

add_library(mlflow-cpp src/client.cpp)

target_include_directories(mlflow-cpp
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(mlflow-cpp
    PUBLIC
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(mlflow-cpp PRIVATE
        -Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion -O3
    )
endif()

enable_testing()
add_executable(mlflow_tests tests/test_client.cpp)
target_link_libraries(mlflow_tests PRIVATE mlflow-cpp gtest_main)
add_test(NAME CoreClientTests COMMAND mlflow_tests)
