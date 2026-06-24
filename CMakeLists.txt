cmake_minimum_required(VERSION 3.14)
project(mlflow-cpp VERSION 0.1.0 LANGUAGES CXX)


set(PROJECT_AUTHOR "Irfan Ghat")
set(PROJECT_DESCRIPTION "A C++ interface to interact directly with an MLflow tracking server via its standard REST endpoints.")
set(PROJECT_EMAIL "irfanghat@gmail.com")
set(PROJECT_HOMEPAGE_URL "https://github.com/irfanghat/mlflow-cpp")
set(PROJECT_LICENSE "Apache-2.0")


message(STATUS "Project : ${PROJECT_NAME}")
message(STATUS "Version : ${PROJECT_VERSION}")
message(STATUS "Description : ${PROJECT_DESCRIPTION}")
message(STATUS "Author : ${PROJECT_AUTHOR}")
message(STATUS "Homepage : ${PROJECT_HOMEPAGE_URL}")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)

include(FetchContent)

FetchContent_Declare(
	json
	URL
	https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_Declare(
	googletest
	URL
	https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)

FetchContent_MakeAvailable(json googletest)

add_library(mlflow-cpp
	src/client.cpp
	src/experiments.cpp
	src/runs.cpp
	src/metrics.cpp
	src/timestamp.cpp
)

target_include_directories(mlflow-cpp
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(mlflow-cpp
	PUBLIC
		CURL::libcurl
		nlohmann_json::nlohmann_json
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(mlflow-cpp
		PRIVATE
			-Wall
			-Wextra
			-Wpedantic
			-Werror
			-Wshadow
			-Wconversion
			-O3
	)
endif()

######################################
# Test Configuration
######################################
enable_testing()

add_executable(mlflow_cpp_tests
	tests/test_client.cpp
	tests/test_experiments.cpp
	tests/test_runs.cpp
)

target_link_libraries(mlflow_cpp_tests
	PRIVATE
		mlflow-cpp
		GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(
	mlflow_cpp_tests
	DISCOVERY_TIMEOUT
	30
)
