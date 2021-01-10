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
 * \file    EditorApplication.cpp
 * \brief   ...
 * \details ...
 */
#include <iostream>
#include <Hearth/Core/Monitor.hpp>
#include <HearthFire/EditorApplication.hpp>

namespace Hearth {

  EditorApplication::EditorApplication()
    : Application(&kEditorCreateInfo)
  {
  }

  void EditorApplication::onInitialize() {
    // Grab primary monitor.
    const Monitor&   monitor    = Monitor::primary();
    const glm::uvec2 resolution = monitor.sizeInPixels();
    const glm::uvec2 wndSize{ 1280, 720 };
    const glm::ivec2 wndPos{
      (resolution.x - wndSize.x) / 2,
      (resolution.y - wndSize.y) / 2
    };

    // Create application window.
    const Window::CreateInfo wndCreateInfo {
      .environment = Environment::instance(),
      .wndTitle    = L"Hearth Fire",
      .wndSize     = wndSize,
      .wndPosition = wndPos,
      .visible     = false
    };

    mAppWindow = Window::create(&wndCreateInfo);
    mAppWindow->show();

    // Create render context.
    const RenderContext::CreateInfo rdrctxCreateInfo {
      .app          = this,
      .requestedAPI = GraphicsAPI::OpenGL
    };

    mRenderContext = RenderContext::create(&rdrctxCreateInfo);

    // Create render surface.
    mRenderSurface = mRenderContext->createSurface(mAppWindow);
  }

  void EditorApplication::onTerminate() noexcept {
    mRenderContext->destroySurface(mRenderSurface);
    RenderContext::destroy(mRenderContext);
    Window::destroy(mAppWindow);
  }

  void EditorApplication::onWindowClose(WindowCloseEvent* wce) noexcept {
    if (wce->window() != mAppWindow)
      return;
    quit(true);
    wce->consume();
  }

}
