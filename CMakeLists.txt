# Project base.
cmake_minimum_required(VERSION 3.14.0...3.19.2 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0077 NEW)
project(Hearth VERSION 0.0.1 LANGUAGES CXX)

# Grab gtest and gbench
include(FetchContent)
message(STATUS "Fetching Googletest...")
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)
message(STATUS "Fetched Googletest")
message(STATUS "Fetching Googlebenchmark...")
FetchContent_Declare(
  gbench
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)
message(STATUS "Fetched Googlebenchmark")

# Make the tools available.
set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
set(BENCHMARK_ENABLE_TESTING OFF)
set(ENABLE_INSTALL OFF)
set(ENABLE_GTEST_TESTS OFF)
FetchContent_MakeAvailable(gtest gbench)
message(STATUS "Googletest and Googlebenchmark made available")

# Populate if necessary.
message(STATUS "Checking gtest population...")
FetchContent_GetProperties(gtest)
if(NOT gtest_POPULATED)
  message(STATUS "Populating gtest...")
  FetchContent_Populate(gtest)
else()
  message(STATUS "gtest populated")
endif()

# Populate if necessary.
message(STATUS "Checking gbench population...")
if(NOT gbench_POPULATED)
  message(STATUS "Populating gbench...")
  FetchContent_Poplulate(gbench)
else()
  message(STATUS "gbench populated")
endif()

# Needed files for building.
include(CMake/ProjectSettings.cmake)
include(CMake/CompilerWarnings.cmake)

# Create project settings interface library.
add_library(hearth_project_settings INTERFACE)
set_project_settings(hearth_project_settings)
set_project_warnings(hearth_project_settings)

# Build tests/benches or no?
option(HEARTH_BUILD_TESTS "Builds the tests for the library and editor." ON)
option(HEARTH_BUILD_BENCHMARKS "Builds the benchmarks for the library and editor." ON)

# Engine and Editor projects.
add_subdirectory(Engine)
add_subdirectory(Editor)
