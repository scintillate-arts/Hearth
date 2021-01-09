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
 * \file    WinApiEventHandler.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_NATIVE_WINAPI_EVENTHANDLER_HPP__
#define __HEARTH_NATIVE_WINAPI_EVENTHANDLER_HPP__ 1
#include <Hearth/Core/Config.hpp>

/* Window specific */
#if HEARTH_WINDOWS_PLATFORM
#include "WinAPI.hpp"
#include "WinAPIWindow.hpp"

namespace Hearth {

  /**
   * \brief 	Handles the events triggered by WinAPI by re-emitting them to running applications.
   * \details ...
   */
  class WinAPIEventHandler final {
    // No construct!
    WinAPIEventHandler() noexcept = default;
    ~WinAPIEventHandler() noexcept = default;

  public:
    /**
     * \brief 		The callback for WinAPI to use when a window receives a message.
     * \details   ...
     * \param[in] hWnd The handle of the window which received a message.
     * \param[in] uMsg The message that was received.
     * \param[in] wParam One set of event data.
     * \param[in] lParam Another set of event data.
     * \returns   Whether or not the event was handled.
     * \remarks   There are two sets of event data. Each has a different purpose and may be null at
     * 					  given time depending on the event. Why Microsoft thought this was a good idea is
     * 					  beyond me, but it is what it is. The return value isn't very consitent either.
     * 					  Each event requires its own unique return value. Though it's typically either TRUE
     * 					  or FALSE.
     */
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

  private:
    /***/
    static LRESULT HEARTHAPI handleChangeUIStateEvent(const WinAPIWindow* wnd, HWND hWnd, WPARAM wParam) noexcept;

    /**
     * \brief 		Handles a call to close a window.
     * \details   ...
     * \param[in] wnd The window that needs to be closed.
     * \returns   FALSE representing that the event was handled.
     */
    static LRESULT HEARTHAPI handleCloseEvent(const WinAPIWindow* wnd) noexcept;

    /**
     * \brief 		Handles a call to destroy a window.
     * \details 	...
     * \param[in] wnd ...
     * \returns 	FALSE representing that the event was handled.
     */
    static LRESULT HEARTHAPI handleDestroyEvent([[maybe_unused]] const WinAPIWindow* wnd) noexcept;

    /**
     * \brief 		Handles a call to focus or unfocus a window.
     * \details   ...
     * \param[in] wnd The window that had its focus changed.
     * \param[in] focused Whether or not the window was focused.
     * \returns   FALSE representing that the event was handled.
     */
    static LRESULT HEARTHAPI handleFocusEvent(WinAPIWindow* wnd, bool focused) noexcept;

    /**
     * \brier 		Handles a call to move the window.
     * \details 	...
     * \param[in] wnd The window that was moved.
     * \param[in] lParam Contains the new position of the window.
     * \returns   FALSE representing that the event was handled.
     * */
    static LRESULT HEARTHAPI handleMoveEvent(WinAPIWindow* wnd, LPARAM lParam) noexcept;

    /**
     * \brief 		Handles a call to size the window.
     * \details 	...
     * \param[in] wnd The window whose size changed.
     * \param[in] wParam Contains the size state of the window.
     * \param[in] lParam Contains the new size of the window.
     * \returns 	FALSE representing that the event was handled.
     */
    static LRESULT HEARTHAPI handleSizeEvent(WinAPIWindow* wnd, WPARAM wParam, LPARAM lParam) noexcept;

    /**
     * \brief 		Handles a call to change the window visibility.
     * \details 	...
     * \param[in] wnd The window whose visibility changed.
     * \param[in] wParam Whether or not the window is visible.
     * \returns 	FALSE representing that the event was handled.
     */
    static LRESULT HEARTHAPI handleShowEvent(WinAPIWindow* wnd, WPARAM wParam) noexcept;
  };

}

#endif /* HEARTH_WINDOWS_PLATFORM */
#endif /* __HEARTH_NATIVE_WINAPI_EVENTHANDLER_HPP__ */
