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
 * \file    OpenGLRenderContext.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_GFX_OPENGL_RENDER_CONTEXT_HPP__
#define __HEARTH_GFX_OPENGL_RENDER_CONTEXT_HPP__ 1
#include <Hearth/Graphics/RenderContext.hpp>
#if HEARTH_GFX_OPENGL_API

namespace Hearth {

  /**
   * \brief   ...
   * \details ...
   */
  class OpenGLRenderContext : public RenderContext {
  public:
    /**
     * \brief   Default constructor.
     * \details ...
     */
    OpenGLRenderContext();

    /**
     * \brief   Default destructor.
     * \details ...
     */
    ~OpenGLRenderContext() noexcept override;

  public:
    /**
     * \copydoc RenderContext::createSurface(const Window*)
     */
    Surface* createSurface(const Core::Window* wnd) noexcept override;

    /**
     * \copydoc RenderContext::destroySurface(Surface*)
     */
    void destroySurface(Surface* surface) noexcept override;
  };

}

#endif /* HEARTH_GFX_OPENGL_API */
#endif /* __HEARTH_GFX_OPENGL_RENDER_CONTEXT_HPP__ */
