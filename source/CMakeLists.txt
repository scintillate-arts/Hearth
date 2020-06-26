include_directories(
  ${PROJECT_SOURCE_DIR}/include/
  $ENV{VK_SDK_PATH}/Include/
  $ENV{GLM_PATH}/
)

link_directories(
  ${PROJECT_SOURCE_DIR}/library/
  $ENV{VK_SDK_PATH}/Lib/
)

set(
  HAPI_LIBRARY_FILES
  application.cpp
  environment.cpp
  event.cpp
  window.cpp
  graphics/cmdbuf.cpp
  graphics/dscset.cpp
  graphics/fence.cpp
  graphics/frmbuf.cpp
  graphics/gfxpip.cpp
  graphics/rdrctx.cpp
  graphics/rdrpss.cpp
  graphics/resbuf.cpp
  graphics/semphr.cpp
  graphics/swpchn.cpp
)

set(
  HAPI_LIBRARY_LINKS
  vulkan-1
)

if(WIN32)
  set(
    HAPI_LIBRARY_FILES
    ${HAPI_LIBRARY_FILES}
    win32/environment.cpp
    win32/winapi.cpp
    win32/window.cpp
  )

  set(
    HAPI_LIBRARY_LINKS
    ${HAPI_LIBRARY_LINKS}
    dwmapi
  )
endif()

# Create library.
add_library(${HAPI_LIBRARY_NAME} STATIC ${HAPI_LIBRARY_FILES})
target_link_libraries(${HAPI_LIBRARY_NAME} PUBLIC ${HAPI_LIBRARY_LINKS})
set_target_properties(
  ${HAPI_LIBRARY_NAME} PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY
  ${PROJECT_SOURCE_DIR}/library/
)
