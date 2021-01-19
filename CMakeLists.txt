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
message(STATUS "Fetching SPDLog...")
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)
message(STATUS "Fetched SPDLog")

# Make the tools available.
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(SPDLOG_MASTER_PROJECT OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest gbench spdlog)
message(STATUS "Googletest, Googlebenchmark, and SPDLog made available")

# Provide test and benchmark build functions.
function(add_custom_gtest TESTNAME TESTFILE)
  add_executable(${TESTNAME} ${TESTFILE})
  target_link_libraries(${TESTNAME} gtest_main)
  add_test(NAME "${TESTNAME}_custom_gtest" COMMAND ${TESTNAME})
endfunction()
function(add_custom_gbench BENCHNAME BENCHFILE)
add_executable(${BENCHNAME} ${BENCHFILE})
target_link_libraries(${BENCHNAME} gtest_main)
add_test(NAME "${BENCHNAME}_custom_gtest" COMMAND ${BENCHNAME})
endfunction()

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
add_subdirectory(HTL)
add_subdirectory(Engine)
add_subdirectory(Editor)
