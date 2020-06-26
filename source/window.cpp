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
#include <iostream>
#include <stdexcept>
#include <hearth/config.hpp>
#if defined(HAPI_WINDOWS_OS)
#  include "win32/window.hpp"
#endif

namespace HAPI_NAMESPACE_NAME {

  Window::Window(const CreateInfo& createInfo)
    : mName(createInfo.wndName.data())
    , mResidency(createInfo.wndResidency)
    , mPosition(createInfo.wndPosition)
    , mSize(createInfo.wndSize)
    , mOpacity(createInfo.opacity)
    , mFocused(true)
    , mFullscreen(createInfo.fullscreen)
    , mDecorated(createInfo.decorated)
    , mMaximized(false)
    , mMinimized(false)
    , mUserResizable(createInfo.userResizable)
    , mTransparent(createInfo.transparent)
    , mVisible(createInfo.visible)
  {
    // First things first, make sure the environment isn't null.
    if (createInfo.wndResidency == nullptr)
      throw std::runtime_error("Expected non-null window residency.");

    // Track this window.
    createInfo.wndResidency->mOpenWindows.push_back(this);
  }

  Window::~Window() noexcept {
    // Untrack this window.
    auto& windows = mResidency->mOpenWindows;
    auto itr      = std::find(windows.begin(), windows.end(), this);
    if (itr != windows.end())
      windows.erase(itr);
  }

  Window* Window::create(const CreateInfo& createInfo) noexcept {
    // What we will be returning.
    Window* result = nullptr;

    // Try to create window.
    try {
    #if defined(HAPI_WINDOWS_OS)
      result = new native::Win32Window(createInfo);
    #endif
    } catch (const std::runtime_error& err) {
      std::cerr << err.what() << std::endl;
      result = nullptr;
    }

    // Results.
    return result;
  }

  void Window::destroy(Window* window) noexcept {
    delete window;
  }

  bool Window::isDecorated() const noexcept {
    return mDecorated;
  }

  bool Window::isFocused() const noexcept {
    return mFocused;
  }

  bool Window::isFullscreen() const noexcept {
    return mFullscreen;
  }

  bool Window::isMaximized() const noexcept {
    return mMaximized;
  }

  bool Window::isMinimized() const noexcept {
    return mMinimized;
  }

  bool Window::isRestored() const noexcept {
    return !mMaximized && !mMinimized;
  }

  bool Window::isTransparent() const noexcept {
    return mTransparent;
  }

  bool Window::isUserResizable() const noexcept {
    return mUserResizable;
  }

  bool Window::isVisible() const noexcept {
    return mVisible;
  }

  float Window::opacity() const noexcept {
    return mOpacity;
  }

  glm::ivec2 Window::position() const noexcept {
    return mPosition;
  }

  glm::uvec2 Window::size() const noexcept {
    return mSize;
  }

  std::wstring_view Window::title() const noexcept {
    return mName;
  }

}
