cmake_minimum_required(VERSION 3.9.0)
project(HearthEngine VERSION 1.0)

set(CMAKE_CXX_EXTENSIONS OFF)
set(DEFAULT_FLAGS "--std=c++2a -Wall -Wextra -Wpedantic")
set(QUIET_FLAGS "-Wno-missing-field-initializers")

if(WIN32)
  set(DEFAULT_FLAGS "${DEFAULT_FLAGS} -mwindows")
endif()

option(BUILD_QUIET "Shuts up the compiler :)" OFF)
option(BUILD_DEBUGGING "Enables build debugging." OFF)
option(BUILD_PROFILING "Enables build profiling." OFF)
option(BUILD_SHARED "Enables shared library build." OFF)
option(BUILD_TESTS "Builds the tests for this project." OFF)
option(BUILD_BENCHMARKS "Builds the benchmarks for this project." OFF)

if(BUILD_QUIET)
  message(STATUS "Quiet build enabled")
  set(DEFAULT_FLAGS "${DEFAULT_FLAGS} ${QUIET_FLAGS}")
endif()

if(BUILD_DEBUGGING)
  message(STATUS "Debugging build enabled")
  set(DEFAULT_FLAGS "${DEFAULT_FLAGS} -DHAPI_DEBUG")
endif()

if(BUILD_PROFILING)
  message(STATUS "Profiling build enabled")
  set(DEFAULT_FLAGS "${DEFAULT_FLAGS} -DHAPI_PROFILE")
endif()

if(BUILD_SHARED)
  message(STATUS "Shared build enabled")
  set(DEFAULT_FLAGS "${DEFAULT_FLAGS} -DHAPI_SHARED_EXPORT")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 ${DEFAULT_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g -O2 ${DEFAULT_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 ${DEFAULT_FLAGS}")

# Set application name
if(CMAKE_BUILD_TYPE MATCHES Debug)
  message(STATUS "Debug build type chosen")
  set(HAPI_LIBRARY_NAME "hearth_debug")
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  message(STATUS "Release with debug info build type chosen")
  set(HAPI_LIBRARY_NAME "hearth_dev")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
  message(STATUS "Release build type chosen")
  set(HAPI_LIBRARY_NAME "hearth")
endif()

# Build libraries and application.
add_subdirectory(source)

# Build tests.
if(BUILD_TESTS)
  message(STATUS "Source testing enabled")
  add_subdirectory(tests)
endif()

# Build benchmarks
if(BUILD_BENCHMARKS)
  message(STATUS "Benchmark testing enabled")
  add_subdirectory(benchmarks)
endif()
