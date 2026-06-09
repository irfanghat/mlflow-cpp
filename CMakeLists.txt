cmake_minimum_required(VERSION 3.22)
project(mlflow_cpp VERSION 0.0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)


add_library(mlflow_client STATIC
    src/net/curl_client.cpp
    src/services/experiment_service.cpp
    src/services/run_service.cpp
)

target_include_directories(mlflow_client
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(mlflow_client
    PUBLIC
        CURL::libcurl
        nlohmann_json::nlohmann_json
)


if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(mlflow_client PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror           # Treat warnings as errors
        -Wshadow          # Warn if a variable shadows another
        -Wconversion      # Warn on implicit type casting/loss of data
        -Wunused
        -O3               # Maximize aggressive optimizations
    )
elseif(MSVC)
    target_compile_options(mlflow_client PRIVATE
        /W4
        /WX
        /O2
    )
endif()