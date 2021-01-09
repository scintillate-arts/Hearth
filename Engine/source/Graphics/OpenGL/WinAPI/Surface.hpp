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
#ifndef __INC_HEARTH_GFX_OPENGL_WINAPI_SURFACE_HPP__
#define __INC_HEARTH_GFX_OPENGL_WINAPI_SURFACE_HPP__ 1
#include <Hearth/Graphics/Surface.hpp>
#if HEARTH_WINDOWS_PLATFORM && HEARTH_GFX_OPENGL_API
#include <GL/glew.h>
#include <GL/wglew.h>
#include "../../../Native/WinAPI/WinAPI.hpp"

namespace Hearth {

  struct WinAPIOpenGLSurface final : public Surface {
    /**
     * \brief     Constructs a new \c WinAPIOpenGLSurface object.
     * \details   ...
     * \param[in] wnd The window to create the surface from.
     */
    explicit WinAPIOpenGLSurface(const Window* wnd);

    /**
     * \brief   Default destructor for \c WinAPIOpenGLSurface objects.
     * \details ...
     */
    ~WinAPIOpenGLSurface() noexcept override;

  private:
    /**
     * \brief   The device context for the window.
     * \details ...
     */
    HDC mDeviceContext = nullptr;

    /**
     * \brief   The render context object from WGL.
     * \details ...
     */
    HGLRC mRenderContext = nullptr;
  };

}

#endif /* Windows Platform and OpenGL API */
#endif /* __INC_HEARTH_GFX_OPENGL_WINAPI_SURFACE_HPP__ */