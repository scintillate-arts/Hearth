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

# We need Vulkan, so find it
find_package(Vulkan REQUIRED)

# Make tools available.
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ONLY_LIBS ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm glew)
message(STATUS "GLM and GLEW made available")

# Build primary library.
add_library(HearthLibs INTERFACE)
target_compile_definitions(HearthLibs INTERFACE GLEW_STATIC)
target_include_directories(HearthLibs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(
  HearthLibs
  INTERFACE hearth_project_settings
  INTERFACE glm::glm
  INTERFACE htl::htl
  INTERFACE spdlog
)

# Build core library.
file(GLOB HEARTH_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/Core/*.cpp)
add_library(HearthCore STATIC ${HEARTH_CORE_SOURCES})
add_library(Hearth::Core ALIAS HearthCore)
target_link_libraries(HearthCore PUBLIC HearthLibs)

# Create Native interface.
add_library(HearthNative INTERFACE)
add_library(Hearth::Native ALIAS HearthNative)

# Build WinAPI library.
if(WIN32)
  file(GLOB HEARTH_WINAPI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/Native/WinAPI/*.cpp)
  add_library(HearthWinAPI STATIC ${HEARTH_WINAPI_SOURCES})
  add_library(Hearth::WinAPI ALIAS HearthWinAPI)
  target_link_libraries(HearthWinAPI PUBLIC HearthLibs PUBLIC HearthCore)
  target_link_libraries(HearthNative INTERFACE HearthWinAPI)
endif()

# OpenGL graphics
file(GLOB_RECURSE HEARTH_OPENGL_SOURCES TRUE ${CMAKE_CURRENT_SOURCE_DIR}/source/Graphics/OpenGL/*.cpp)
add_library(HearthOpenGL STATIC ${HEARTH_OPENGL_SOURCES})
add_library(Hearth::OpenGL ALIAS HearthOpenGL)
target_link_libraries(HearthOpenGL PUBLIC HearthLibs PUBLIC libglew_static PUBLIC opengl32)

# Full graphics library.
file(GLOB HEARTH_GRAPHICS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/Graphics/*.cpp)
add_library(HearthGraphics STATIC ${HEARTH_GRAPHICS_SOURCES})
add_library(Hearth::Graphics ALIAS HearthGraphics)
target_link_libraries(HearthGraphics PUBLIC HearthOpenGL)

# Hearth proper
file(GLOB HEARTH_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_library(HearthProper STATIC ${HEARTH_SOURCES})
add_library(Hearth::Proper ALIAS HearthProper)
target_link_libraries(HearthProper PUBLIC HearthLibs)

# Build all in one target.
add_library(HearthEngine INTERFACE)
add_library(Hearth::Engine ALIAS HearthEngine)
target_link_libraries(
  HearthEngine
  INTERFACE HearthProper
  INTERFACE HearthCore
  INTERFACE HearthNative
  INTERFACE HearthGraphics
)

# Build benchmarks.

# Build tests.
