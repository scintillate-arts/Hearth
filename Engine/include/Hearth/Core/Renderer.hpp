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
#ifndef __INC_HEARTH_CORE_RENDERER_HPP__
#define __INC_HEARTH_CORE_RENDERER_HPP__ 1
#include <memory>

namespace Hearth {

  /**
   * \brief   Describes an object which manages and performs the necessary actions to render graphics to the currently
   *          set render surface, with the currently set render context.
   * \details
   */
  class Renderer final {
  public:
    /**
     * \brief   Retrieves the \c Renderer singleton.
     * \details The \c Renderer is a singleton object, because it's a system which manages the rendering of an
     *          application.
     * \returns The \c Renderer singleton.
     */
    static std::shared_ptr<Renderer> instance() noexcept;
  };

}

#endif /* __INC_HEARTH_CORE_RENDERER_HPP__ */