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
#include <hearth/event.hpp>

namespace HAPI_NAMESPACE_NAME {

  Event::Event(EventType type) noexcept
    : mType(type), mConsumed(false)
  { }

  void Event::consume() noexcept {
    mConsumed = true;
  }

  EventType Event::type() const noexcept {
    return mType;
  }

  bool Event::isConsumed() const noexcept {
    return mConsumed;
  }

  WindowEvent::WindowEvent(EventType type, Window* window) noexcept
    : Event(type)
    , mWindow(window)
  { }

  Window* WindowEvent::origin() const noexcept {
    return mWindow;
  }

  FrameBufferResizeEvent::FrameBufferResizeEvent(Window* origin, glm::uvec2 size) noexcept
    : WindowEvent(EventType::FrameBufferResize, origin)
    , mFrameBufferSize(size)
  { }

  glm::uvec2 FrameBufferResizeEvent::framebufferSize() const noexcept {
    return mFrameBufferSize;
  }

  WindowCloseEvent::WindowCloseEvent(Window* origin) noexcept
    : WindowEvent(EventType::WindowClose, origin)
  { }

  WindowResizeEvent::WindowResizeEvent(Window* origin, glm::uvec2 size) noexcept
    : WindowEvent(EventType::WindowResize, origin)
    , mWindowSize(size)
  { }

  glm::uvec2 WindowResizeEvent::windowSize() const noexcept {
    return mWindowSize;
  }

  WindowRepositionEvent::WindowRepositionEvent(Window* origin, glm::ivec2 position) noexcept
    : WindowEvent(EventType::WindowReposition, origin)
    , mWindowPosition(position)
  { }

  glm::ivec2 WindowRepositionEvent::windowPosition() const noexcept {
    return mWindowPosition;
  }

  WindowFocusEvent::WindowFocusEvent(Window* origin, bool focused) noexcept
    : WindowEvent(EventType::WindowFocus, origin)
    , mFocused(focused)
  { }

  bool WindowFocusEvent::focused() const noexcept {
    return mFocused;
  }

  WindowMaximizeEvent::WindowMaximizeEvent(Window* origin, bool maximized) noexcept
    : WindowEvent(EventType::WindowMaximize, origin)
    , mMaximized(maximized)
  { }

  bool WindowMaximizeEvent::maximized() const noexcept {
    return mMaximized;
  }

  WindowMinimizeEvent::WindowMinimizeEvent(Window* origin, bool minimized) noexcept
    : WindowEvent(EventType::WindowMinimize, origin)
    , mMinimized(minimized)
  { }

  bool WindowMinimizeEvent::minimized() const noexcept {
    return mMinimized;
  }

  WindowMovingEvent::WindowMovingEvent(Window* origin, std::int32_t* xpos, std::int32_t* ypos) noexcept
    : WindowEvent(EventType::WindowMoving, origin)
    , mCurrentXpos(xpos)
    , mCurrentYpos(ypos)
  { }

  std::int32_t* WindowMovingEvent::currentXpos() const noexcept {
    return mCurrentXpos;
  }

  std::int32_t* WindowMovingEvent::currentYpos() const noexcept {
    return mCurrentYpos;
  }

}
