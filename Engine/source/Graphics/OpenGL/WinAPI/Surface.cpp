/* Copyright (c) 2020 Simular Games, LLC.
 * -------------------------------------------------------------------------------------------------
 *
 * MIT License
 * -------------------------------------------------------------------------------------------------
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * -------------------------------------------------------------------------------------------------
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/**
 * \file
 * \brief
 * \details
 */
#include <Hearth/Core/Logger.hpp>
#include "Surface.hpp"
#if HEARTH_WINDOWS_PLATFORM && HEARTH_GFX_OPENGL_API
#include "../../../Native/WinAPI/WinApiEnvironment.hpp"
#include "../../../Native/WinAPI/WinAPIWindow.hpp"

// HACK:
//    This is a rendering specific subsystem, it should be initialized by the rendering system.
//    It is currently temporarily initialized by the first instantiation of a render surface.
struct WGLInitializer final {
  // Makes sure WGL gets initialized properly.
  static void initialize() {
    // Create dummy window.
    auto dummyWindow = CreateWindowExW(
      0,
      Hearth::Core::Window::kClassName.data(),
      L"Dummy Window",
      0,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      nullptr,
      nullptr,
      GetModuleHandle(nullptr),
      nullptr
    );

    // Make sure dummy window was created.
    if (dummyWindow == nullptr) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to create dummy window for WGL initialization:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Grab dummy window and create PFD.
    auto dummyDC = GetDC(dummyWindow);
    auto pfd     = PIXELFORMATDESCRIPTOR{};
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion     = 1;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits   = 32;
    pfd.cAlphaBits   = 8;
    pfd.iLayerType   = PFD_MAIN_PLANE;
    pfd.cDepthBits   = 32;
    pfd.cStencilBits = 8;

    // Choose pixel format.
    int pxlFrmt = ChoosePixelFormat(dummyDC, &pfd);
    if (pxlFrmt == 0) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to find suitable pixel format for WGL initialization:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Set pixel format.
    if (!SetPixelFormat(dummyDC, pxlFrmt, &pfd)) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to set suitable pixel format for WGL initialization:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Create dummy WGL context.
    auto dummyRC = wglCreateContext(dummyDC);
    if (dummyRC == nullptr) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to create dummy WGL surface for WGL initialization:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Try to make context current.
    if (!wglMakeCurrent(dummyDC, dummyRC)) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to make dummy WGL surface current for WGL initialization:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Try to initialize WGL.
    if (wglewInit() != GLEW_OK) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to initialize WGL:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Clean up.
    wglMakeCurrent(dummyDC, NULL);
    wglDeleteContext(dummyRC);
    ReleaseDC(dummyWindow, dummyDC);
    DestroyWindow(dummyWindow);
    mInitialized = true;
    HEARTH_LOGGER_INFO("Successfully initialized WGL");
  }

  // Checks if the initializer succeeded.
  static bool didInitialize() noexcept {
    return mInitialized;
  }

private:
  inline static bool mInitialized = false;
};

namespace Hearth {

  WinAPIOpenGLSurface::WinAPIOpenGLSurface(const Core::Window* wnd) {
    // Make sure WGL was initialized.
    if (!WGLInitializer::didInitialize()) {
      WGLInitializer::initialize();
      HEARTH_LOGGER_WARN("Initializing WGL, expected to be initialized already");
    } else
      HEARTH_LOGGER_DEBUG("WGL was initialized as expected");

    // Fetch system handle.
    auto hWnd = reinterpret_cast<HWND>(wnd->systemHandle());

    // Set HDC.
    mDeviceContext = GetDC(hWnd);

    // Create pixel format attributes.
    int pixelFormatAttribs[] = {
      WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
      WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
      WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
      WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
      WGL_COLOR_BITS_ARB,     32,
      WGL_DEPTH_BITS_ARB,     24,
      WGL_STENCIL_BITS_ARB,   8,
      0,                      0
    };

    if (wglChoosePixelFormatARB == nullptr) {
      HEARTH_LOGGER_ERROR("WGL was not initialized properly, wglChoosePixelFormatARB undefined");
      return;
    }

    // Pick pixel format.
    INT  pixelFormat;
    UINT numFormats;
    wglChoosePixelFormatARB(mDeviceContext, pixelFormatAttribs, NULL, 1, &pixelFormat, &numFormats);
    if (numFormats == 0) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to choose pixel format for WGL surface:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Set pixel format.
    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(mDeviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    if (!SetPixelFormat(mDeviceContext, pixelFormat, &pfd)) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to choose compatible pixel format for WGL surface:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Specify OpenGL context attributes.
    int glContextAttribs[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 1,
      WGL_CONTEXT_MINOR_VERSION_ARB, 1,
      WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      0,                             0
    };

    // Try and create context.
    mRenderContext = wglCreateContextAttribsARB(mDeviceContext, NULL, glContextAttribs);
    if (mRenderContext == nullptr) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to create WGL render surface context:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Try to make render context current.
    if (!wglMakeCurrent(mDeviceContext, mRenderContext)) {
      // Grab error.
      const auto err = Hearth::WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());

      // Print log in two parts, then throw exception.
      HEARTH_LOGGER_CRITICAL("Failed to make WGL surface current:");
      HEARTH_LOGGER_CRITICAL(cnv);
      throw std::runtime_error(cnv);
    }

    // Success!
    HEARTH_LOGGER_DEBUG("Created WGL surface {0}", static_cast<void*>(mRenderContext));
  }

  WinAPIOpenGLSurface::~WinAPIOpenGLSurface() noexcept {
    wglDeleteContext(mRenderContext);
    HEARTH_LOGGER_DEBUG("Destroyed WGL surface {0}", static_cast<void*>(mRenderContext));
  }

}

#endif /* Windows platform and OpenGL API */