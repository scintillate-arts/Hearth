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
 * \file    Window.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_NATIVE_WINDOW_HPP__
#define __HEARTH_NATIVE_WINDOW_HPP__ 1
#include <Hearth/Config.hpp>
#include <Hearth/Core/Window.hpp>

/* Windows specific */
#if HEARTH_WINDOWS_PLATFORM
#include "WinAPI.hpp"

namespace Hearth {
  // Forward.
  class WinAPIEventHandler;

  /**
   * \brief 	A WinAPI implementation of the window interface.
   * \details ...
   */
  class WinAPIWindow : public Core::Window {
    // Can access the protected members to set them.
    friend class WinAPIEventHandler;

  public:
    static constexpr std::wstring_view kPropName = L"Hearth::Window::Prop";

  public:
    explicit WinAPIWindow(const Core::Window::CreateInfo* createInfo);
    ~WinAPIWindow() noexcept override;

  public:
    void blur() noexcept override;
    void decorate() noexcept override;
    void flash() noexcept override;
    void focus() noexcept override;
    void hide() noexcept override;
    void maximize() noexcept override;
    void minimize() noexcept override;
    void restore() noexcept override;
    void show() noexcept override;
    void undecorate() noexcept override;
    void userResizable(bool userCanResize) noexcept override;
    Core::WindowHandle systemHandle() const noexcept override;
    Core::Window* parent() const noexcept override;
    glm::ivec2 position() const noexcept override;
    glm::uvec2 size() const noexcept override;
    std::wstring title() const noexcept override;
    void reparent(Core::Window* parent) noexcept override;
    void reposition(glm::ivec2 pos) noexcept override;
    void resize(glm::uvec2 size) noexcept override;
    void retitle(std::wstring_view title) noexcept override;

  protected:
    HWND mNativeHandle;
  };

}

#endif /* HEARTH_WINDOWS_PLATFORM */
#endif /* __HEARTH_NATIVE_WINDOW_HPP__ */
