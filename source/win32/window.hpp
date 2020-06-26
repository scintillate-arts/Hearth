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
#pragma once
#include <hearth/window.hpp>
#include "winapi.hpp"

namespace HAPI_NAMESPACE_NAME::native {

  /*!
   * \brief A Windows operating system specific window representation.
   */
  class Win32Window final: public Window {
  public:
    /*!
     * \brief     Creates this Windows operating system window from the information provided.
     * \param[in] createInfo The information for creating this window.
     */
    Win32Window(const CreateInfo& createInfo);

    /*!
     * \brief     Moves the other window into this one.
     * \param[in] other The other window to move data from.
     */
    Win32Window(Win32Window&& other) noexcept;

    /*!
     * \brief     Moves the other window into this one.
     * \param[in] other The other window to move data from.
     * \return    This window with the new data.
     */
    Win32Window& operator=(Win32Window&& other) noexcept;

    /*!
     * \brief Properly destructs this window, by returning it to the environment.
     */
   ~Win32Window() noexcept;

  public:
    //! \copydoc Window::close()
    void close() noexcept final;

    //! \copydoc Windoww::hide()
    void hide() noexcept final;

    //! \copydoc Window::focus()
    void focus() noexcept final;

    //! \copydoc Window::maximize()
    void maximize() noexcept final;

    //! \copydoc Window::minimize()
    void minimize() noexcept final;

    //! \copydoc Window::requestAttention()
    void requestAttention() noexcept final;

    //! \copydoc Window::restore()
    void restore() noexcept final;

    //! \copydoc Window::show()
    void show() noexcept final;

    //! \copydoc Window::enableDecorations()
    void enableDecorations(bool decorated) noexcept final;

    //! \copydoc Window::enableFullscreen()
    void enableFullscreen(bool fullscreen) noexcept final;

    //! \copydoc Windoww::enableUserResizability()
    void enableUserResizability(bool canResize) noexcept final;

    //! \copydoc Window::enableTransparency()
    void enableTransparency(bool transparent) noexcept final;

    //! \copydoc Window::reopacify()
    void reopacify(float opacity) noexcept final;

    //! \copydoc Window::reposition()
    void reposition(const glm::ivec2& newPos) noexcept final;

    //! \copydoc Window::resize()
    void resize(const glm::uvec2& newSize) noexcept final;

    //! \copydoc Window::retitle()
    void retitle(std::wstring_view name) noexcept final;

    //! \copydoc Window::isOpen()
    bool isOpen() const noexcept final;

  public:
    /*!
     * \brief  Gets the handle for this window.
     * \return The underlying HWND object.
     */
    HWND handle() const noexcept;

  public:
    /*!
     * \brief     Handles the processing of events sent to the window by the operating system.
     * \param[in] hWnd The handle to the window which recieved the events.
     * \param[in] uMsg The event that was emitted.
     * \param[in] wParam The primary parameter for the event.
     * \param[in] lParam The secondary parameter for the event.
     * \return    Whether or not the event was handled.
     */
    static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

  private:
    //! \brief The handle to this window.
    HWND mWindowHandle;
  };

}
