# Needed tools for this project.
include(FetchContent)
message(STATUS "Fetching GLM...")
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)
message(STATUS "Fetch complete")
message(STATUS "Fetching GLEW...")
FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
)
message(STATUS "Fetch complete")

# Make tools available.
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ONLY_LIBS ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm glew)
message(STATUS "GLM and GLEW made available")

# Build primary library.
add_library(hearth_engine INTERFACE)
target_compile_definitions(hearth_engine INTERFACE UNICODE INTERFACE GLEW_STATIC)
target_link_libraries(
  hearth_engine
  INTERFACE hearth_project_settings
  INTERFACE glm::glm
  INTERFACE libglew_static
  INTERFACE opengl32
)
target_include_directories(hearth_engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Build core library.
file(GLOB_RECURSE CORE_SOURCES false "./Core/" "*.cpp")
add_library(hearth_core STATIC ${CORE_SOURCES})
add_library(hearth::core ALIAS hearth_core)
target_link_libraries(hearth_core PUBLIC hearth_engine)

# Create Native interface.
add_library(hearth_native INTERFACE)
add_library(hearth::native ALIAS hearth_native)

# Build WinAPI library.
if(WIN32)
  file(GLOB_RECURSE NATIVE_WINAPI_SOURCES false "./Native/WinAPI/" "*.cpp")
  add_library(hearth_winapi STATIC ${NATIVE_WINAPI_SOURCES})
  add_library(hearth::winapi ALIAS hearth_winapi)
  target_link_libraries(hearth_winapi PUBLIC hearth_engine)
  target_link_libraries(hearth_native INTERFACE hearth_winapi)
endif()

# Full graphics library.
file(GLOB_RECURSE GFX_SOURCES false "./Graphics/" "*.cpp")
add_library(hearth_graphics STATIC ${GFX_SOURCES})
add_library(hearth::graphics ALIAS hearth_graphics)
target_link_libraries(hearth_graphics PUBLIC hearth_engine)

# Build all in one target.
add_library(hearth INTERFACE)
target_link_libraries(
  hearth
  INTERFACE hearth::core
  INTERFACE hearth::native
  INTERFACE hearth::graphics
)

# Build benchmarks.

# Build tests.
