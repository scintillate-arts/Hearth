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
#include <glm/glm.hpp>
#include "config.hpp"

namespace HAPI_NAMESPACE_NAME {
  // Forward.
  class Window;

  //! \brief Defines the different types of events that are processed by Hearth.
  enum struct EventType {
    // KeyboardButton,
    // MouseButton,
    // MouseScroll,
    // CursorPosition,
    // CursorEnter,
    FrameBufferResize,
    // MonitorConnect,
    WindowClose,
    WindowFocus,
    WindowMaximize,
    WindowMinimize,
    WindowMoving,
    WindowResize,
    WindowReposition,
  };

  //! \brief Represents an event which can be passed and handled by engine objects.
  class Event {
  protected:
    /*!
     * \brief     Constructs this event with the given type.
     * \param[in] type The type of this event.
     */
    explicit Event(EventType type) noexcept;

  public:
    //! \brief Consumes this event. Prevents it from being processed further.
    void consume() noexcept;

    //! \brief Gets the type of this event.
    EventType type() const noexcept;

    //! \brief Checks if this event was consumed.
    bool isConsumed() const noexcept;

  private:
    //! \brief The type of this event.
    EventType mType;

    //! \brief Whether or not this event has been consumed.
    bool mConsumed;
  };

  //! \brief Represents that happen because of a window.
  class WindowEvent : public Event {
  protected:
    /*!
     * \brief     Constructs this \c WindowEvent from the given type and the \c Window the event was
     *            triggered from.
     * \param[in] type The type of this event.
     * \param[in] window The \c Window this event was triggered from.
     */
    explicit WindowEvent(EventType type, Window* window) noexcept;

  public:
    /*!
     * \brief  Gets the \c Window this event originated from.
     * \return The origin of this event as a non-constant.
     */
    Window* origin() const noexcept;

  private:
    //! \brief The window by which the event was triggered from.
    Window* mWindow;
  };

  // //! \brief Event emitted when a key on the keyboard is pressed, released, or held.
  // class KeyboardButtonEvent final: public WindowEvent {
  //   // Only the environment can send these types of events.
  //   friend class Environment;

  //   /*!
  //    * \brief     Constructs this \c KeyboardButtonEvent with the given event information.
  //    * \param[in] origin The \c Window this event originated from.
  //    * \param[in] key The \c KeyboardButton that was acted on.
  //    * \param[in] action The \c InputAction that took place.
  //    * \param[in] mods The \c ButtonModifiers that were present during the action.
  //    */
  //   KeyboardButtonEvent(Window* origin, KeyboardButton key, InputAction action, int mods) noexcept;

  // public:
  //   //! \brief Default destructor.
  //   ~KeyboardButtonEvent() = default;

  //   /*!
  //    * \brief  Gets the key that was acted on.
  //    * \return The key that this \c KeyboardButtonEvent was created with.
  //    */
  //   KeyboardButton actedKey() const noexcept;

  //   /*!
  //    * \brief  Gets the action that was taken.
  //    * \return The action that this \c KeyboardButtonEvent was created with.
  //    */
  //   InputAction actionTaken() const noexcept;

  //   /*!
  //    * \brief  The modifiers that were present.
  //    * \return The modifiers that this \c KeyboardButtonEvent was created with.
  //    */
  //   int modifiers() const noexcept;

  // private:
  //   //! \brief The key that was acted on.
  //   KeyboardButton mKey;

  //   //! \brief The action that took place.
  //   InputAction mAction;

  //   //! \brief The modifiers that were present during the action.
  //   int mModifiers;
  // };

  // //! \brief Event emitted when a button on the mouse is pressed, released, or held.
  // class MouseButtonEvent : public WindowEvent {
  //   // Only the environment can send these types of events.
  //   friend class Environment;

  //   /*!
  //    * \brief     Constructs this \c MouseButtonEvent with the given event information.
  //    * \param[in] origin The \c Window this event originates from.
  //    * \param[in] button The \c MouseButton that was acted on.
  //    * \param[in] action The \c InputAction that was taken.
  //    * \param[in] mods The \c ButtonModifiers that were present during the action.
  //    */
  //   MouseButtonEvent(Window* origin, MouseButton button, InputAction action, int mods) noexcept;

  // public:
  //   //! \brief Default destructor.
  //   ~MouseButtonEvent() = default;

  //   /*!
  //    * \brief  Gets the button that was acted on.
  //    * \return The button that this \c MouseButtonEvent was created with.
  //    */
  //   MouseButton actedButton() const noexcept;

  //   /*!
  //    * \brief  Gets the action that was taken.
  //    * \return The action that this \c MouseButtonEvent was created with.
  //    */
  //   InputAction actionTaken() const noexcept;

  //   /*!
  //    * \brief  The modifiers that were present.
  //    * \return The modifiers that this \c MouseButtonEvent was created with.
  //    */
  //   int modifiers() const noexcept;

  // private:
  //   //! \brief The button that was acted on.
  //   MouseButton mButton;

  //   //! \brief The action that was taken.
  //   InputAction mAction;

  //   //! \brief The modifiers present during the action.
  //   int mModifiers;
  // };

  // //! \brief Event emitted when the mouse scroll wheel is moved.
  // class MouseScrollEvent : public WindowEvent {
  //   // Only the environment can send these types of events.
  //   friend class Environment;

  //   /*!
  //    * \brief     Constructs this \c MouseScrollEvent with the given event information.
  //    * \param[in] origin The \c Window this event originates from.
  //    * \param[in] scroll The amount the scroll wheel was moved during this event.
  //    */
  //   MouseScrollEvent(Window* origin, glm::fvec2 scroll) noexcept;

  // public:
  //   //! \brief Default destructor.
  //   ~MouseScrollEvent() = default;

  //   /*!
  //    * \brief  Gets the amount the scroll wheel was moved during this event.
  //    * \return The amount the scroll wheel was moved when this event was created.
  //    */
  //   glm::fvec2 scrollValue() const noexcept;

  // private:
  //   //! \brief The amount the scroll wheel was moved during this event.
  //   glm::fvec2 mScroll;
  // };

  // //! \brief Event emitted when the cursor moves to a different location in the given window.
  // class CursorPositionEvent : public WindowEvent {
  //   // Only the environment can send these types of events.
  //   friend class Environment;

  //   /*!
  //    * \brief     Constructs this \c CursorPositionEvent with the given event information.
  //    * \param[in] origin The \c Window this event originated from.
  //    * \param[in] position The position of the cursor during this event.
  //    */
  //   CursorPositionEvent(Window* origin, glm::fvec2 position) noexcept;

  // public:
  //   //! \brief Default destructor.
  //   ~CursorPositionEvent() = default;

  //   /*!
  //    * \brief  Gets the cursor position during this event.
  //    * \return The position of the cursor when this \c CursorPositionEvent was created.
  //    */
  //   glm::fvec2 cursorPosition() const noexcept;

  // private:
  //   //! \brief The position of the cursor during this event.
  //   glm::fvec2 mCursorPosition;
  // };

  // //! \brief Event emitted when the cursor enters or exits the client area of a given window.
  // class CursorEnterEvent : public WindowEvent {
  //   // Only the environment can send these types of events.
  //   friend class Environment;

  //   /*!
  //    * \brief     Constructs this \c CursorEnterEvent with the given event information.
  //    * \param[in] origin The \c Window this event originated from.
  //    * \param[in] entered This value will be true if the cursor entered the \c Window client area
  //    *            and false if the cursor exitted the \c Window client area.
  //    */
  //   CursorEnterEvent(Window* origin, bool entered) noexcept;

  // public:
  //   //! \brief Default destructor.
  //   ~CursorEnterEvent() = default;

  //   /*!
  //    * \brief  Checks whether or not the cursor did enter the \c Window client area.
  //    * \return True if it did, false if it exitted.
  //    */
  //   bool entered() const noexcept;

  // private:
  //   //! \brief Whether or not the cursor entered the \c Window client area.
  //   bool mEntered;
  // };

  //! \brief Event emitted when the framebuffer of the given window changes size.
  class FrameBufferResizeEvent : public WindowEvent {
  public:
    /*!
     * \brief     Constructs this \c FrameBufferResizeEvent with the given event information.
     * \param[in] origin The \c Window this event originated from.
     * \param[in] size The new size of the framebuffer.
     */
    FrameBufferResizeEvent(Window* origin, glm::uvec2 size) noexcept;

    /*!
     * \brief  Gets the size of the framebuffer during this event.
     * \return The size of the framebuffer when this \c FrameBufferResizeEvent was created.
     */
    glm::uvec2 framebufferSize() const noexcept;

  private:
    //! \brief The new size of the framebuffer when this event is emitted.
    glm::uvec2 mFrameBufferSize;
  };

  // //! \brief Event emitted when a monitor is connected or disconnected from the system.
  // class MonitorConnectEvent : public Event {
  //   // Only the environment can send these types of events.
  //   friend class Environment;

  //   /*!
  //    * \brief     Constructs this \c MonitorConnectEvent with the given event information.
  //    * \param[in] monitor The \c Monitor that was connected or disconnected.
  //    * \param[in] connected Whether or not the \c Monitor was connected or disconnected.
  //    */
  //   MonitorConnectEvent(Monitor monitor, bool connected) noexcept;

  // public:
  //   //! \brief Default destructor.
  //   ~MonitorConnectEvent() = default;

  //   /*!
  //    * \brief  Gets the \c Monitor that was connected or disconnected.
  //    * \return The \c Monitor that was connected or disconnected.
  //    */
  //   Monitor monitor() const noexcept;

  //   /*!
  //    * \brief  Whether or not the \c Monitor was connected or disconnected.
  //    * \return True if the \c Monitor was connected and false if the \c Monitor was disconnected.
  //    */
  //   bool connected() const noexcept;

  // private:
  //   //! \brief The \c Monitor that was connected or disconnected.
  //   Monitor mMonitor;

  //   //! \brief Whether or not the \c Monitor was connected or disconnected.
  //   bool mConnected;
  // };

  //! \brief Event emitted when a window is closed.
  class WindowCloseEvent : public WindowEvent {
  public:
    /*!
     * \brief     Constructs this \c WindowCloseEvent with the given event information.
     * \param[in] origin The \c Window this event originated from.
     */
    WindowCloseEvent(Window* origin) noexcept;
  };

  //! \brief Event emitted when the size of the window changes.
  class WindowResizeEvent : public WindowEvent {
  public:
    /*!
     * \brief     Constructs this \c WindowResizeEvent with the given event information.
     * \param[in] origin The \c Window this event originated from.
     * \param[in] size The new size of the \c Window during this event.
     */
    WindowResizeEvent(Window* origin, glm::uvec2 size) noexcept;

    /*!
     * \brief  Gets the size of the \c Window during this event.
     * \return The size of the \c Window when this event was created.
     */
    glm::uvec2 windowSize() const noexcept;

  private:
    //! \brief The new size of the \c Window during this event.
    glm::uvec2 mWindowSize;
  };

  //! \brief Event emitted when the position of the window changes.
  class WindowRepositionEvent : public WindowEvent {
  public:
    /*!
     * \brief     Constructs this \c WindowResizeEvent with the given event information.
     * \param[in] origin The \c Window this event originated from.
     * \param[in] position The new position of the \c Window during this event.
     */
    WindowRepositionEvent(Window* origin, glm::ivec2 position) noexcept;

    /*!
     * \brief  Gets the position of the \c Window during this event.
     * \return The position of the \c Window when this event was created.
     */
    glm::ivec2 windowPosition() const noexcept;

  private:
    //! \brief The new position of the \c Window during this event.
    glm::ivec2 mWindowPosition;
  };

  //! \brief Event emitted when the window is focused or unfocused.
  class WindowFocusEvent : public WindowEvent {
  public:
    /*!
     * \brief     Constructs this \c WindowFocusEvent with the given event information.
     * \param[in] origin The \c Window this event originated from.
     * \param[in] focused Whether or not the \c Window was focused or unfocused.
     */
    WindowFocusEvent(Window* origin, bool focused) noexcept;

    /*!
     * \brief  Checks if the \c Window was focused or unfocused.
     * \return True if the \c Window was focused, false if unfocused.
     */
    bool focused() const noexcept;

  private:
    //! \brief Whether or not the \c Window was focused or unfocused.
    bool mFocused;
  };

  //! \brief Event emitted when the window is maximized or restored.
  class WindowMaximizeEvent : public WindowEvent {
  public:
    /*!
     * \brief     Constructs this \c WindowMaximizeEvent with the given event information.
     * \param[in] origin The \c Window this event originated from.
     * \param[in] maximized Whether or not the \c Window was maximized or restored.
     */
    WindowMaximizeEvent(Window* origin, bool maximized) noexcept;

    /*!
     * \brief  Checks if the \c Window was maximized or restored.
     * \return True if the \c Window was maximized, false if restored.
     */
    bool maximized() const noexcept;

  private:
    //! \brief Whether or not the \c Window was maximized or restored.
    bool mMaximized;
  };

  //! \brief Event emitted when the window is minimized or restored.
  class WindowMinimizeEvent : public WindowEvent {
  public:
    /*!
     * \brief     Constructs this \c WindowMinimizeEvent with the given event information.
     * \param[in] origin The \c Window this event originated from.
     * \param[in] minimized Whether or not the \c Window was minimized or restored.
     */
    WindowMinimizeEvent(Window* origin, bool minimized) noexcept;

    /*!
     * \brief  Checks if the \c Window was minimized or restored.
     * \return True if the \c Window was minimized, false if restored.
     */
    bool minimized() const noexcept;

  private:
    //! \brief Whether or not the \c Window was minimized or restored.
    bool mMinimized;
  };

  //! \brief Event emitted when the window is dragged around by the player.
  class WindowMovingEvent : public WindowEvent {
  public:
    /*!
     * \brief     Constructs this window moving event from the given information.
     * \param[in] origin The window that emitted this event.
     * \param[in] xpos The current xpos of the window.
     * \param[in] ypos The current ypos of the window.
     */
    WindowMovingEvent(Window* origin, std::int32_t* xpos, std::int32_t* ypos) noexcept;

    /*!
     * \brief  Gets the current x position so it may be modified.
     * \return The current x position of the window.
     */
    std::int32_t* currentXpos() const noexcept;

    /*!
     * \brief  Gets the current y position so it may be modified.
     * \return The current y position of the window.
     */
    std::int32_t* currentYpos() const noexcept;

  private:
    //! \brief The current x position of the window during this event.
    std::int32_t* mCurrentXpos;

    //! \brief The current y position of the window during this event.
    std::int32_t* mCurrentYpos;
  };

}
