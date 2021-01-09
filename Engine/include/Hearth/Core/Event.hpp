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
 * \file    Event.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_CORE_EVENT_HPP__
#define __HEARTH_CORE_EVENT_HPP__ 1
#include <glm/glm.hpp>

namespace Hearth {

  // Forward.
  struct Window;

  /**
   * \brief   Represents the different types of events there are.
   * \details ...
   */
  enum struct EventType {
    Undefined,
    WindowClose,
    WindowFocus,
    WindowMove,
    WindowShow,
    WindowSize,
  };

  /**
   * \brief   Represents the base and outline form of an Event.
   * \details Extending classes only need implement their own data containment.
   */
  struct IEvent {
    /**
     * \brief   Default destructor.
     * \details This destructor is virtual in case extending classes need it.
     */
    virtual ~IEvent() noexcept = default;

  public:
    /**
     * \brief   Gets the type of this event.
     * \details ...
     * \returns The type of this event, set by subclasses.
     */
    [[nodiscard]]
    EventType type() const noexcept { return mType; }

    /**
     * \brief   Consumes (handles) an event.
     * \details Consumed events are no longer propagated.
     */
    void consume() noexcept { mConsumed = true; }

    /**
     * \brief   Checks if the event was consumed.
     * \details Consumed events should no longer be propagated, thus a dispatcher should check
     * 					if the event was consumed before sending it again.
     * \returns True if the event was consumed, false otherwise.
     */
    [[nodiscard]]
    bool isConsumed() const noexcept { return mConsumed; }

  protected:
    /**
     * \brief   The type of the event.
     * \details ...
     */
    EventType mType = EventType::Undefined;

    /**
     * \brief   Whether or not this event was consumed.
     * \details ...
     */
    bool mConsumed = false;
  };

  /**
   * \brief   Represents a Window specific event triggered by environment.
   * \details ...
   * \remarks Window events are triggered by user input to the window frame and client area.
   */
  class WindowEvent : public IEvent {
  public:
    /**
     * \brief     Constructs a new window event from the given window instance.
     * \details   ...
     * \param[in] window The window that had an event triggered on it.
     * \param[in] type The type of the window event.
     */
    explicit WindowEvent(const Window* window, EventType type) noexcept
      : mWindow{ window }
    {
      mType = type;
    }

  public:
    /**
     * \brief	  Gets the window that the event was triggered for.
     * \details ...
     * \returns The window that the event triggered for.
     */
    [[nodiscard]]
    const Window* window() const noexcept { return mWindow; }

  private:
    /**
     * \brief   The window the event happened on.
     * \details ...
     */
    const Window* mWindow;
  };

  /**
   * \brief   Represents an event described by the request to close a window by the user.
   * \details ...
   */
  class WindowCloseEvent final : public WindowEvent {
  public:
    /**
     * \brief     Constructs a new window close event from the given window.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     */
    explicit WindowCloseEvent(const Window* wnd) noexcept
      : WindowEvent(wnd, EventType::WindowClose)
    { }
  };

  /**
   * \brief   Represents an event described by the request to focus or unfocus a window.
   * \details ...
   */
  class WindowFocusEvent final : public WindowEvent {
  public:
    /**
     * \brief     Constructs a new window focus event from the given window and focus state.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     * \param[in] focused Whether or not the window was focused.
     */
    explicit WindowFocusEvent(const Window* wnd, bool focused) noexcept
      : WindowEvent(wnd, EventType::WindowFocus)
      , mFocused(focused)
    { }

  public:
    /**
     * \brief   Checks if the window was focused during this event.
     * \details ...
     * \returns True if the window was focused, false otherwise.
     */
    [[nodiscard]]
    bool wasFocused() const noexcept { return mFocused; }

  private:
    /**
     * \brief	  Whether or not the window was focused.
     * \details ...
     */
    bool mFocused;
  };

  /**
   * \brief   Represents an event described by the movement of a window.
   * \details ...
   */
  class WindowMoveEvent final : public WindowEvent {
  public:
    /**
     * \brief	    Constructs a new window move event from the given window and position.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     * \param[in] pos The new position of the window.
     */
    explicit WindowMoveEvent(const Window* wnd, glm::ivec2 pos) noexcept
      : WindowEvent(wnd, EventType::WindowMove)
      , mPosition{ pos }
    { }

  public:
    /**
     * \brief	  Gets the new position of the window.
     * \details ...
     * \returns The new position of the window as tracked by the environment.
     */
    [[nodiscard]]
    glm::ivec2 position() const noexcept { return mPosition; }

  private:
    /**
     * \brief   The position of the window.
     * \details ...
     */
    glm::ivec2 mPosition;
  };

  /**
   * \brief	  Represents an event described by the visibility change of a window.
   * \details ...
   */
  class WindowShowEvent final : public WindowEvent {
  public:
    /**
     * \brief	    Constructs a new window show event from the given window and visibility.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     * \param[in] visible Whether or not the window is visible.
     */
    explicit WindowShowEvent(const Window* wnd, bool visible) noexcept
      : WindowEvent(wnd, EventType::WindowShow)
      , mVisible(visible)
    { }

  public:
    /**
     * \brief   Check if the window was visible when the event was triggered.
     * \details ...
     * \returns The state of the window's visibility.
     */
    [[nodiscard]]
    bool visible() const noexcept { return mVisible; }

  private:
    /**
     * \brief   Whether or not the window is visible.
     * \details ...
     */
    bool mVisible;
  };

  /**
   * \brief   This enumeration represents the different types of size states for a window.
   * \details When the window is sized but neither maximized or minimized, it is considered restored.
   *          Otherwise, the size state of the window will either be maximized or minimized.
   */
  enum struct SizeState {
    Maximized,
    Minimized,
    Restored
  };

  /**
   * \brief   Represents an event described by the change in size of the window.
   * \details ...
   */
  class WindowSizeEvent final : public WindowEvent {
  public:
    /**
     * \brief     Constructs a new window size event from the window and the new size.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     * \param[in] size The new size of the window.
     * \param[in] sizeState The new size state of the window.
     */
    explicit WindowSizeEvent(const Window* wnd, glm::uvec2 size, SizeState sizeState) noexcept
      : WindowEvent(wnd, EventType::WindowSize)
      , mSize(size)
      , mSizeState(sizeState)
    { }

  public:
    /**
     * \brief   Gets the new size of the window.
     * \details ...
     * \returns The new size of the window.
     */
    [[nodiscard]]
    glm::uvec2 size() const noexcept { return mSize; }

    /**
     * \brief	  Gets the new size state of the window.
     * \details ...
     * \returns The new size state of the window.
     */
    [[nodiscard]]
    SizeState sizeState() const noexcept { return mSizeState; }

  private:
    /**
     * \brief   The size of the window.
     * \details ...
     * */
    glm::uvec2 mSize;

    /**
     * \brief   The size state of the window.
     * \details ...
     */
    SizeState mSizeState;
  };

}

#endif /* __HEARTH_CORE_EVENT_HPP__ */
