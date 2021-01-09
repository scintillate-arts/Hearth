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
  class WinAPIWindow : public IWindow{
    // Can access the protected members to set them.
    friend class WinAPIEventHandler;

  public:
    /**
     * \brief 	The name of window properties used by Hearth.
     * \details ...
     */
    static constexpr std::wstring_view kPropName = L"Hearth::Window::Prop";

  public:
    /**
     * \brief 		Constructs a new WinAPI window instance from the given create information.
     * \details 	...
     * \param[in] createInfo The information needed to create the window.
     */
    explicit WinAPIWindow(const WindowCreateInfo* createInfo);

    /**
     * \brief 	Makes sure the native window is destroyed properly.
     * \details ...
     */
    ~WinAPIWindow() noexcept override;

  public:
    /**
     * \copydoc IWindow::blur()
     */
    void blur() noexcept override;

    /**
     * \copydoc IWindow::decorate()
     */
    void decorate() noexcept override;

    /**
     * \copydoc IWindow::flash()
     */
    void flash() noexcept override;

    /**
     * \copydoc IWindow::focus()
     */
    void focus() noexcept override;

    /**
     * \copydoc IWindow::hide()
     */
    void hide() noexcept override;

    /**
     * \copydoc IWindow::maximize()
     */
    void maximize() noexcept override;

    /**
     * \copydoc IWindow::minimize()
     */
    void minimize() noexcept override;

    /**
     * \copydoc IWindow::restore()
     */
    void restore() noexcept override;

    /**
     * \copydoc IWindow::show()
     */
    void show() noexcept override;

    /**
     * \copydoc IWindow::undecorate()
     */
    void undecorate() noexcept override;

    /**
     * \copydoc IWindow::userResizable(bool)
     */
    void userResizable(bool userCanResize) noexcept override;

    /**
     * \copydoc IWindow::handle()
     */
    [[nodiscard]]
    WindowHandle handle() const noexcept override;

    /**
     * \copydoc IWindow::position()
     */
    [[nodiscard]]
    glm::ivec2 position() const noexcept override;

    /**
     * \copydoc IWindow::size()
     */
    [[nodiscard]]
    glm::uvec2 size() const noexcept override;

    /**
     * \copydoc IWindow::title()
     */
    [[nodiscard]]
    std::wstring title() const noexcept override;

    /**
     * \copydoc IWindow::reposition(glm::ivec2)
     */
    void reposition(glm::ivec2 pos) noexcept override;

    /**
     * \copydoc IWindow::resize(glm::uvec2)
     */
    void resize(glm::uvec2 size) noexcept override;

    /**
     * \copydoc IWindow::retitle(std::wstring_view)
     */
    void retitle(std::wstring_view title) noexcept override;

  protected:
    /**
     * \brief 	The native window handle.
     * \details ...
     */
    HWND mNativeHandle;
  };

}

#endif /* HEARTH_WINDOWS_PLATFORM */
#endif /* __HEARTH_NATIVE_WINDOW_HPP__ */