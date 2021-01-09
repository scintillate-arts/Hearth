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
#include <Hearth/Graphics/RenderContext.hpp>
#include "OpenGL/OpenGLRenderContext.hpp"

namespace Hearth {

  RenderContext* RenderContext::create(const CreateInfo* createInfo) noexcept {
    RenderContext* result = nullptr;
    try {
      // Create context based on type.
      switch (createInfo->requestedAPI) {
      case GraphicsAPI::OpenGL:
        result = new OpenGLRenderContext();
        break;
      }
    } catch (const std::runtime_error& err) {
      return nullptr;
    }

    return result;
  }

  void RenderContext::destroy(RenderContext* context) noexcept {
    // Don't delete null pointer.
    if (context == nullptr)
      return;

    // Delete proper context.
    switch (context->gfxApi()) {
    case GraphicsAPI::OpenGL:
      delete dynamic_cast<OpenGLRenderContext*>(context);
      break;
    }
  }

}