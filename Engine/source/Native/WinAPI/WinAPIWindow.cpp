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
 * \file    Window.cpp
 * \brief   ...
 * \details ...
 */
#include "WinAPIWindow.hpp"
#include "WinApiEnvironment.hpp"
#include <iostream>

namespace Hearth {

  WinAPIWindow::WinAPIWindow(const Window::CreateInfo *createInfo) {
    // Get environment.
    auto winEnv = dynamic_cast<const WinAPIEnvironment*>(createInfo->environment.get());
    auto wndCls = winEnv->windowClass();

    // Set the styles for the window.
    const LONG_PTR exstyle = WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW;
    const LONG_PTR nmstyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;

    // Prepare window for creation.
    mNativeHandle = CreateWindowEx(
      exstyle,
      kClassName.data(),
      createInfo->wndTitle.data(),
      nmstyle,
      createInfo->wndPosition.x,
      createInfo->wndPosition.y,
      static_cast<int>(createInfo->wndSize.x),
      static_cast<int>(createInfo->wndSize.y),
      nullptr,
      nullptr,
      wndCls.hInstance,
      nullptr
    );

    // Check that the window was created.
    if (mNativeHandle == nullptr) {
      const auto err = WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());
      throw std::runtime_error(cnv.c_str());
    }

    // Set Window prop.
    if (FAILED(SetProp(mNativeHandle, kPropName.data(), reinterpret_cast<HANDLE>(this)))) {
      const auto err = WinAPIEnvironment::errorMessage(GetLastError());
      const auto cnv = std::string(err.begin(), err.end());
      throw std::runtime_error(cnv.c_str());
    }
  }

  WinAPIWindow::~WinAPIWindow() noexcept {
    DestroyWindow(mNativeHandle);
  }

  void WinAPIWindow::blur() noexcept {
    SetFocus(nullptr);
  }

  void WinAPIWindow::decorate() noexcept {
    LONG_PTR style  = GetWindowLongPtrW(mNativeHandle, GWL_EXSTYLE);
             style |= WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW;

    SetWindowLongPtrW(mNativeHandle, GWL_EXSTYLE, style);
    style = GetWindowLongPtrW(mNativeHandle, GWL_STYLE);

    if (mUserResizable) {
      style |= WS_OVERLAPPEDWINDOW;
    } else {
      style &= ~WS_OVERLAPPEDWINDOW;
      style |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    }

    SetWindowLongPtrW(mNativeHandle, GWL_STYLE, style);
    mDecorated = true;
  }

  void WinAPIWindow::flash() noexcept {
    FlashWindow(mNativeHandle, TRUE);
  }

  void WinAPIWindow::focus() noexcept {
    BringWindowToTop(mNativeHandle);
    SetForegroundWindow(mNativeHandle);
    SetFocus(mNativeHandle);
  }

  void WinAPIWindow::hide() noexcept {
    ShowWindow(mNativeHandle, SW_HIDE);
  }

  void WinAPIWindow::maximize() noexcept {
    ShowWindow(mNativeHandle, SW_MAXIMIZE);
  }

  void WinAPIWindow::minimize() noexcept {
    ShowWindow(mNativeHandle, SW_MINIMIZE);
  }

  void WinAPIWindow::restore() noexcept {
    ShowWindow(mNativeHandle, SW_RESTORE);
  }

  void WinAPIWindow::show() noexcept {
    ShowWindow(mNativeHandle, SW_SHOW);
  }

  void WinAPIWindow::undecorate() noexcept {
    LONG_PTR style  = GetWindowLongPtrW(mNativeHandle, GWL_EXSTYLE);
             style &= ~(WS_EX_OVERLAPPEDWINDOW);

    SetWindowLongPtrW(mNativeHandle, GWL_EXSTYLE, style);
    style  = GetWindowLongPtrW(mNativeHandle, GWL_STYLE);
    style &= ~WS_OVERLAPPEDWINDOW;
    SetWindowLongPtrW(mNativeHandle, GWL_STYLE, style);
    mDecorated = false;
  }

  void WinAPIWindow::userResizable(bool userCanResize) noexcept {
    mUserResizable = userCanResize;
    if (mDecorated)
      decorate();
    else
      undecorate();
  }

  [[nodiscard]]
  WindowHandle WinAPIWindow::handle() const noexcept {
    return reinterpret_cast<WindowHandle>(mNativeHandle);
  }

  [[nodiscard]]
  glm::ivec2 WinAPIWindow::position() const noexcept {
    RECT rect;
    GetWindowRect(mNativeHandle, &rect);
    return glm::ivec2 { rect.left, rect.top };
  }

  [[nodiscard]]
  glm::uvec2 WinAPIWindow::size() const noexcept {
    RECT rect;
    GetWindowRect(mNativeHandle, &rect);
    return glm::uvec2{
      static_cast<glm::u32>(rect.right - rect.left),
      static_cast<glm::u32>(rect.bottom - rect.top)
    };
  }

  [[nodiscard]]
  std::wstring WinAPIWindow::title() const noexcept {
    std::wstring title;
    const auto len = GetWindowTextLength(mNativeHandle);
    title.resize(static_cast<std::size_t>(len));
    GetWindowText(mNativeHandle, title.data(), len);
    return title;
  }

  void WinAPIWindow::reposition(glm::ivec2 pos) noexcept {
    // Convert types and create rect.
    const LONG xpos = static_cast<LONG>(pos.x);
    const LONG ypos = static_cast<LONG>(pos.y);
          RECT rect = { xpos, ypos, xpos, ypos };

    // Get current styles.
    const LONG_PTR exstyle = GetWindowLongPtrW(mNativeHandle, GWL_EXSTYLE);
    const LONG_PTR nmstyle = GetWindowLongPtrW(mNativeHandle, GWL_STYLE);
    const UINT     wndDpi  = GetDpiForWindow(mNativeHandle);

    // Check our type.
    if (WinAPIEnvironment::IsWindows10AnniversaryOrLater())
      AdjustWindowRectExForDpi(&rect, nmstyle, FALSE, exstyle, wndDpi);
    else
      AdjustWindowRectEx(&rect, nmstyle, FALSE, exstyle);

    // Set new position.
    const UINT flags = SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE;
    SetWindowPos(mNativeHandle, nullptr, rect.left, rect.top, 0, 0, flags);
  }

  void WinAPIWindow::resize(glm::uvec2 size) noexcept {
    // Convert types and create rect.
    const LONG width  = static_cast<LONG>(size.x);
    const LONG height = static_cast<LONG>(size.y);
          RECT rect   = { 0, 0, width, height };

    // Get current styles.
    const LONG_PTR exstyle = GetWindowLongPtrW(mNativeHandle, GWL_EXSTYLE);
    const LONG_PTR nmstyle = GetWindowLongPtrW(mNativeHandle, GWL_STYLE);
    const UINT     wndDpi  = GetDpiForWindow(mNativeHandle);

    // Check our type.
    if (WinAPIEnvironment::IsWindows10AnniversaryOrLater())
      AdjustWindowRectExForDpi(&rect, nmstyle, FALSE, exstyle, wndDpi);
    else
      AdjustWindowRectEx(&rect, nmstyle, FALSE, exstyle);

    // Set new position.
    const LONG newWidth  = rect.right  - rect.left;
    const LONG newHeight = rect.bottom - rect.top;
    const UINT flags     = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER;
    SetWindowPos(mNativeHandle, HWND_TOP, 0, 0, newWidth, newHeight, flags);
  }

  void WinAPIWindow::retitle(std::wstring_view title) noexcept {
    SetWindowText(mNativeHandle, title.data());
  }

  Window* Window::create(const Window::CreateInfo* createInfo) noexcept {
    Window* result = nullptr;
    try {
      result = new WinAPIWindow(createInfo);
    } catch (const std::runtime_error& err) {
      std::cerr << err.what() << std::endl;
    }
    return result;
  }

  void Window::destroy(Window* window) noexcept {
    try {
      delete dynamic_cast<WinAPIWindow*>(window);
    } catch (const std::runtime_error& err) {
      return;
    }
  }

}