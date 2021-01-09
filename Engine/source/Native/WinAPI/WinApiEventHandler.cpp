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
#include <Hearth/Core/Application.hpp>
#include "../../Core/EventDispatcher.hpp"
#include "WinApiEnvironment.hpp"
#include "WinApiEventHandler.hpp"
#if HEARTH_WINDOWS_PLATFORM

namespace Hearth {

  LRESULT CALLBACK WinAPIEventHandler::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept {
    auto wnd = reinterpret_cast<WinAPIWindow*>(GetProp(hWnd, WinAPIWindow::kPropName.data()));
    if (wnd == nullptr) {
      switch (uMsg) {
      case WM_NCCREATE:
        if (WinAPIEnvironment::IsWindows10AnniversaryOrLater())
          EnableNonClientDpiScaling(hWnd);
        break;
      case WM_SHOWWINDOW:
        ShowWindowAsync(hWnd, SW_SHOWNORMAL);
        return FALSE;
      default:
        break;
      }

      return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    switch (uMsg) {
    case WM_CLOSE:      return handleCloseEvent(wnd);
    case WM_DESTROY:    return handleDestroyEvent(wnd);
    case WM_ERASEBKGND: return TRUE;
    case WM_KILLFOCUS:  return handleFocusEvent(wnd, false);
    case WM_MOVE:       return handleMoveEvent(wnd, lParam);
    case WM_SETFOCUS:   return handleFocusEvent(wnd, true);
    case WM_SHOWWINDOW: return handleShowEvent(wnd, wParam);
    case WM_SIZE:       return handleSizeEvent(wnd, wParam, lParam);
    default:            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  LRESULT HEARTHAPI WinAPIEventHandler::handleCloseEvent(const WinAPIWindow* wnd) noexcept {
    // Push event.
    EventDispatcher::pushEvent(std::make_unique<WindowCloseEvent>(wnd));
    return FALSE;
  }

  LRESULT WinAPIEventHandler::handleDestroyEvent([[maybe_unused]]const WinAPIWindow *wnd) noexcept {
    return FALSE;
  }

  LRESULT WinAPIEventHandler::handleFocusEvent(WinAPIWindow *wnd, bool focused) noexcept {
    EventDispatcher::pushEvent(std::make_unique<WindowFocusEvent>(wnd, focused));
    wnd->mFocused = focused;
    return FALSE;
  }

  LRESULT WinAPIEventHandler::handleMoveEvent(WinAPIWindow *wnd, LPARAM lParam) noexcept {
    const glm::ivec2 position{ static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)) };
    EventDispatcher::pushEvent(std::make_unique<WindowMoveEvent>(wnd, position));
    return FALSE;
  }

  LRESULT WinAPIEventHandler::handleSizeEvent(WinAPIWindow *wnd, WPARAM wParam, LPARAM lParam) noexcept {
    // Did we minimize or maximize?
    const auto minimized = wParam == SIZE_MINIMIZED;
    const auto maximized = wParam == SIZE_MAXIMIZED || (wnd->mMaximized && wParam != SIZE_RESTORED);
    const auto width     = static_cast<glm::u32>(LOWORD(lParam));
    const auto height    = static_cast<glm::u32>(HIWORD(lParam));
    const auto size      = glm::uvec2{ width, height };
    const auto state     = minimized ? SizeState::Minimized :
                           maximized ? SizeState::Maximized :
                                       SizeState::Restored;
    // Emit event.
    EventDispatcher::pushEvent(std::make_unique<WindowSizeEvent>(wnd, size, state));
    wnd->mMaximized = maximized;
    wnd->mMinimized = minimized;
    return FALSE;
  }

  LRESULT WinAPIEventHandler::handleShowEvent(WinAPIWindow *wnd, WPARAM wParam) noexcept {
    // Check if state changed.
    if (wParam == TRUE && wnd->mVisible != TRUE) {
      // emit.
      EventDispatcher::pushEvent(std::make_unique<WindowShowEvent>(wnd, true));
      wnd->mVisible = true;
    } else if (wParam == FALSE && wnd->mVisible != FALSE) {
      // emit.
      EventDispatcher::pushEvent(std::make_unique<WindowShowEvent>(wnd, false));
      wnd->mVisible = false;
    }

    return FALSE;
  }

}

#endif /* HEARTH_WINDOWS_PLATFORM */
