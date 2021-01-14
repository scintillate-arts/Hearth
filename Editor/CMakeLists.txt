# Build editor application.
file(GLOB_RECURSE EDITOR_SOURCES false "./" "*.cpp")
add_executable(HearthFire ${EDITOR_SOURCES})
target_include_directories(
  HearthFire
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(HearthFire PRIVATE Hearth::Engine)

# Build benchmarks

# Build tests
