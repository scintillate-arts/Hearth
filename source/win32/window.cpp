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
#include <stdexcept>
#include "environment.hpp"
#include "window.hpp"
#include "../eventbus.hpp"

namespace HAPI_NAMESPACE_NAME::native {

  Win32Window::Win32Window(const CreateInfo& createInfo)
    : Window(createInfo)
    , mWindowHandle(nullptr)
  {
    // Get the environment as the platform environment.
    const auto* residency = static_cast<Win32Environment*>(createInfo.wndResidency);
    const auto& wndClass  = residency->windowClass();

    // Get window styles.
    LONG_PTR exstyle = WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW;
    LONG_PTR nmstyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;

    // Should the window be visible?
    if (createInfo.visible)
      nmstyle |= WS_VISIBLE;

    // Prepare window for creation.
    mWindowHandle = CreateWindowExW(
      exstyle,
      kClassname.data(),
      createInfo.wndName.data(),
      nmstyle,
      createInfo.wndPosition.x,
      createInfo.wndPosition.y,
      createInfo.wndSize.x,
      createInfo.wndSize.y,
      nullptr,
      nullptr,
      wndClass.hInstance,
      nullptr
    );

    // Check that window was created.
    if (!mWindowHandle) {
      // Get error message.
      std::wstring err = residency->errorMessage(GetLastError());

      // TODO: throw Hearth::Exception.
      // Convert wstring to regular string, construct from aggregate
      throw std::runtime_error({ err.begin(), err.end() });
    }

    // Set window prop.
    if (SetPropW(mWindowHandle, kPropname.data(), reinterpret_cast<HANDLE>(this)) == 0) {
      // Get error message.
      std::wstring err = residency->errorMessage(GetLastError());

      // TODO: throw Hearth::Exception.
      // Convert wstring to regular string, construct from aggregate
      throw std::runtime_error({ err.begin(), err.end() });
    }

    // Enable decorations, transparency?
    enableDecorations(createInfo.decorated);
    enableTransparency(createInfo.transparent);
    reopacify(createInfo.opacity);

    // All done.
  }

  Win32Window::Win32Window(Win32Window&& other) noexcept
    : Window(std::move(other))
    , mWindowHandle(std::move(other.mWindowHandle))
  {
    // Assure this.
    other.mWindowHandle = nullptr;
  }

  Win32Window& Win32Window::operator=(Win32Window&& other) noexcept {
    Window::operator=(std::move(other));
    std::swap(mWindowHandle, other.mWindowHandle);
    return *this;
  }

  Win32Window::~Win32Window() noexcept {
    // Attempt to close window.
    close();
  }

  void Win32Window::close() noexcept {
    // Try to delete window.
    if (mWindowHandle != nullptr)
      DestroyWindow(mWindowHandle);
  }

  void Win32Window::hide() noexcept {
    ShowWindow(mWindowHandle, SW_HIDE);
  }

  void Win32Window::focus() noexcept {
    BringWindowToTop(mWindowHandle);
    SetForegroundWindow(mWindowHandle);
    SetFocus(mWindowHandle);
  }

  void Win32Window::maximize() noexcept {
    ShowWindow(mWindowHandle, SW_MAXIMIZE);
  }

  void Win32Window::minimize() noexcept {
    ShowWindow(mWindowHandle, SW_MINIMIZE);
  }

  void Win32Window::requestAttention() noexcept {
    FlashWindow(mWindowHandle, TRUE);
  }

  void Win32Window::restore() noexcept {
    ShowWindow(mWindowHandle, SW_RESTORE);
  }

  void Win32Window::show() noexcept {
    ShowWindow(mWindowHandle, SW_SHOWNORMAL);
  }

  void Win32Window::enableDecorations(bool decorated) noexcept {
    if (decorated) {
      LONG_PTR style  = GetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE);
               style |= WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW;

      SetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE, style);
      style = GetWindowLongPtrW(mWindowHandle, GWL_STYLE);

      if (mUserResizable) {
        style |= WS_OVERLAPPEDWINDOW;
      } else {
        style &= ~WS_OVERLAPPEDWINDOW;
        style |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
      }

      SetWindowLongPtrW(mWindowHandle, GWL_STYLE, style);
      mDecorated = decorated;
    } else {
      LONG_PTR style  = GetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE);
               style &= ~(WS_EX_OVERLAPPEDWINDOW);

      SetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE, style);
      style  = GetWindowLongPtrW(mWindowHandle, GWL_STYLE);
      style &= ~WS_OVERLAPPEDWINDOW;
      SetWindowLongPtrW(mWindowHandle, GWL_STYLE, style);
      mDecorated = false;
    }
  }

  void Win32Window::enableFullscreen(bool) noexcept {
    // Not yet implemented.
  }

  void Win32Window::enableUserResizability(bool canResize) noexcept {
    mUserResizable = canResize;
    enableDecorations(mDecorated);
  }

  void Win32Window::enableTransparency(bool transparent) noexcept {
    // Check with windows whether or not we can do this.
    BOOL enabled = FALSE;
    if (SUCCEEDED(DwmIsCompositionEnabled(&enabled)) && enabled && transparent) {
      HRGN           region = CreateRectRgn(0, 0, -1, -1);
      DWM_BLURBEHIND bb;
      bb.dwFlags                = DWM_BB_ENABLE | DWM_BB_BLURREGION;
      bb.hRgnBlur               = region;
      bb.fEnable                = TRUE;
      bb.fTransitionOnMaximized = FALSE;

      // Enable the blur behind.
      if (SUCCEEDED(DwmEnableBlurBehindWindow(mWindowHandle, &bb))) {
        // Get style to update.
        LONG_PTR exstyle  = GetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE);
                 exstyle |= WS_EX_LAYERED;

        // Update style and set layered attributes.
        SetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE, exstyle);
        SetLayeredWindowAttributes(mWindowHandle, RGB(255, 255, 255), 255, LWA_COLORKEY);
      }

      // Let Windows reclaim our region.
      DeleteObject(region);
    } else {
      // Get style to update.
      LONG_PTR exstyle  = GetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE);
               exstyle &= ~WS_EX_LAYERED;

      // Update style and redraw window.
      SetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE, exstyle);
      RedrawWindow(mWindowHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME);
    }
  }

  void Win32Window::reopacify(float opacity) noexcept {
    // Do if transparent.
    if (!mTransparent) {
      mOpacity = (opacity < 0.0f) ? 0.0f : (opacity > 1.0f) ? 1.0f : opacity;
      return;
    }

    // Set new opacity.
    if (opacity < 1.0f)
      SetLayeredWindowAttributes(mWindowHandle, 0, (BYTE)(255 * ((opacity < 0.0f) ? 0.0f : opacity)), LWA_ALPHA);
    else
      SetLayeredWindowAttributes(mWindowHandle, 0, 255, LWA_ALPHA);
  }

  void Win32Window::reposition(const glm::ivec2& newPos) noexcept {
    // Convert types and create rect.
    const LONG xpos = static_cast<LONG>(newPos.x);
    const LONG ypos = static_cast<LONG>(newPos.y);
          RECT rect = { xpos, ypos, xpos, ypos };

    // Get current styles.
    const LONG_PTR exstyle = GetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE);
    const LONG_PTR nmstyle = GetWindowLongPtrW(mWindowHandle, GWL_STYLE);
    const LONG     wndDpi  = GetDpiForWindow(mWindowHandle);

    // Check our type.
    if (IsWindows10AnniversaryOrEarlier())
      AdjustWindowRectExForDpi(&rect, nmstyle, FALSE, exstyle, wndDpi);
    else
      AdjustWindowRectEx(&rect, nmstyle, FALSE, exstyle);

    // Set new position.
    // TODO: Make relative to montior.
    const UINT flags = SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE;
    SetWindowPos(mWindowHandle, nullptr, rect.left, rect.top, 0, 0, flags);
  }

  void Win32Window::resize(const glm::uvec2& newSize) noexcept {
    // Convert types and create rect.
    const LONG width  = static_cast<LONG>(newSize.x);
    const LONG height = static_cast<LONG>(newSize.y);
          RECT rect   = { 0, 0, width, height };

    // Get current styles.
    const LONG_PTR exstyle = GetWindowLongPtrW(mWindowHandle, GWL_EXSTYLE);
    const LONG_PTR nmstyle = GetWindowLongPtrW(mWindowHandle, GWL_STYLE);
    const LONG     wndDpi  = GetDpiForWindow(mWindowHandle);

    // Check our type.
    if (IsWindows10AnniversaryOrEarlier())
      AdjustWindowRectExForDpi(&rect, nmstyle, FALSE, exstyle, wndDpi);
    else
      AdjustWindowRectEx(&rect, nmstyle, FALSE, exstyle);

    // Set new position.
    const LONG newWidth  = rect.right  - rect.left;
    const LONG newHeight = rect.bottom - rect.top;
    const UINT flags     = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER;
    SetWindowPos(mWindowHandle, HWND_TOP, 0, 0, newWidth, newHeight, flags);
  }

  void Win32Window::retitle(std::wstring_view name) noexcept {
    SetWindowTextW(mWindowHandle, name.data());
  }

  bool Win32Window::isOpen() const noexcept {
    return static_cast<bool>(IsWindow(mWindowHandle));
  }

  HWND Win32Window::handle() const noexcept {
    return mWindowHandle;
  }

  LRESULT CALLBACK Win32Window::windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept {
    // Get prop.
    auto* window = reinterpret_cast<Win32Window*>(GetPropW(hWnd, kPropname.data()));

    // Check if the window exists.
    if (window == nullptr) {
      // Perform actions because this window was just created.
      switch (uMsg) {
      case WM_NCCREATE:
        if (IsWindows10AnniversaryOrEarlier())
          EnableNonClientDpiScaling(hWnd);
        break;
      }

      // Return defaults.
      return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }

    // Act on window events.
    switch (uMsg) {
    case WM_SETFOCUS:
      EventBus::emit(WindowFocusEvent{ window, true });
      break;
    case WM_KILLFOCUS:
      EventBus::emit(WindowFocusEvent{ window, false });
      break;
    case WM_MOVING: {
      // Prepare event information.
            auto* rect   = reinterpret_cast<LPRECT>(lParam);
      const auto  width  = rect->right  - rect->left;
      const auto  height = rect->bottom - rect->top;
            auto* xpos   = reinterpret_cast<std::int32_t*>(&rect->left);
            auto* ypos   = reinterpret_cast<std::int32_t*>(&rect->top);

      // Emit event.
      EventBus::emit(WindowMovingEvent{ window, xpos, ypos });

      // Adjust rect size.
      rect->right  = rect->left + width;
      rect->bottom = rect->top  + height;
      break;
    }
    case WM_MOVE: {
      const auto xpos = static_cast<glm::i32>(LOWORD(lParam));
      const auto ypos = static_cast<glm::i32>(HIWORD(lParam));
      const auto  pos = glm::ivec2{ xpos, ypos };
      EventBus::emit(WindowRepositionEvent{ window, pos });
      window->mPosition = pos;
      return FALSE;
    }
    case WM_SIZE: {
      // Did we minimize or maximize?
      const auto minimized = wParam == SIZE_MINIMIZED;
      const auto maximized = wParam == SIZE_MAXIMIZED || (window->mMaximized && wParam != SIZE_RESTORED);
      if (window->mMinimized != minimized)
        EventBus::emit(WindowMinimizeEvent{ window, minimized });
      if (window->mMaximized != maximized)
        EventBus::emit(WindowMaximizeEvent{ window, maximized });

      // Emit size changes.
      const auto width  = static_cast<glm::u32>(LOWORD(lParam));
      const auto height = static_cast<glm::u32>(HIWORD(lParam));
      const auto size   = glm::uvec2{ width, height };
      EventBus::emit(FrameBufferResizeEvent{ window, size });
      EventBus::emit(WindowResizeEvent{ window, size });

      // Set window properties.
      window->mMaximized = maximized;
      window->mMinimized = minimized;
      window->mSize = size;
      return FALSE;
    }
    case WM_ERASEBKGND:
      return TRUE;
    case WM_NCACTIVATE:
    case WM_NCPAINT:
      if (!window->mDecorated)
        return TRUE;
      break;
    case WM_DWMCOMPOSITIONCHANGED:
      if (window->mTransparent)
        window->enableTransparency(window->mTransparent);
      return FALSE;
    case WM_CLOSE:
      EventBus::emit(WindowCloseEvent{ window });
      return FALSE;
    }

    // Return defaults.
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }

}
