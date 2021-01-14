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
#ifndef __INC_HEARTH_CORE_WINDOW_HPP__
#define __INC_HEARTH_CORE_WINDOW_HPP__ 1
#include <cstdint>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Forward.hpp"
#include "Event.hpp"

namespace Hearth::Core {
/**
 * \typedef WindowHandle
 * \brief   Represents a platform agnostic native window handle.
 */
/**
 * \property Window::kClassName
 * \brief    The name of the class that is used by Hearth.
 */
/**
 * \property Window::kMenuName
 * \brief    The name of the window menu class is used by Hearth.
 */
/**
 * \fn    Window::~Window()
 * \brief Default destructor for window types.
 */
/**
 * \fn      Window::isDecorated()
 * \brief   Checks if this window is decorated.
 * \details This will return the decorated state of the window. To change it call either:
 *          `decorate()` or `undecorate()`.
 * \returns True if the window is decorated, false otherwise.
 */
/**
 * \fn      Window::isFocused()
 * \brief   Checks if the window is focused.
 * \details This will return the focused state of the window. If the window becomes unfocused,
 *          you can refocus it with `focus()`.
 * \returns True if the window is focused, false otherwise.
 */
/**
 * \fn      Window::isMaximized()
 * \brief   Checks if the window is maximized.
 * \details This will return the maximized state of the window. To change it call either:
 *          `maximize()`, `minimize()`, or `restore()`. The window cannot be maximized when it
 *          is fullscreen, so this will return false in that case.
 * \returns True if the window is maximized, false otherwise.
 */
/**
 * \fn      Window::isMinimized()
 * \brief   Checks if the window is minimized.
 * \details This will return the minimized state of the window. To change it call either:
 *          `maximize()`, `minimize()`, or `restore()`. The window cannot be minimized when it
 *          is fullscreen, so this will return false in that case.
 * \returns True if the window is minimized, false otherwise.
 */
/**
 * \fn      Window::isRestored()
 * \brief   Checks if the window is restored.
 * \details This will return true if the window is not maximized and not minimized. This will
 *          return false if the window is either maximized or minimized. If the window is
 *          fullscreen, this will return true, because the fullscreen state is considered to be
 *          a restored state.
 * \returns True if the window is restored, false otherwise.
 */
/**
 * \fn      Window::isUserResizable()
 * \brief   Checks whether or not the window is user resizable.
 * \details The window is considered user resizable if the window has thick borders and the
 *          maximize control button. Otherwise it is not user resizable. To change this state,
 *          use the `enableUserResizability(bool)` function.
 * \returns True if the window is user resizable, false otherwise.
 */
/**
 * \fn      Window::isVisible()
 * \brief   Checks if the window is visible.
 * \details The window is considered visible if it's shown, even if it is minimized. It's only
 *          when the window is hidden that it is not visible. Hiding a window puts it into the
 *          system tray. To change this state use either `show()` or `hide()`.
 * \returns True if the window is visible, false otherwise.
 */
/**
 * \fn      Window::blur()
 * \brief   Unfocuses the window.
 * \details This will leave the window at the top, but it will no longer receive keyboard input.
 */
/**
 * \fn      Window::decorate()
 * \brief   Changes the decorated state of this window to decorated.
 * \details The window is considered decorated if it has borders and a caption.
 */
/**
 * \fn      Window::flash()
 * \brief   Attempts to get user attention.
 * \details Flashes the window, and often the taskbar button for the window, to grab the user's
 *          attention.
 */
/**
 * \fn      Window::focus()
 * \brief   Requests the window back into focus.
 * \details This will bring the window to the top, set it as the foreground, and finally set it
 *          to receive input focus.
 */
/**
 * \fn      Window::hide()
 * \brief   Makes the window hidden.
 * \details Hiding a window, at least on Microsoft systems, just means that it gets put into the
 *          `system tray`. It still exists, it's not minimized technically, it's just hiding.
 */
/**
 * \fn      Window::maximize()
 * \brief   Maximizes the window.
 * \details This will bring the frame of the window to the size of the monitor it is on. Though,
 *          it will not work if the window is fullscreen.
 */
/**
 * \fn      Window::minimize()
 * \brief   Minimizes the window.
 * \details This will bring the frame of the window to 0 width and 0 height, and dock it on the
 *          taskbar. This will not work if the window is fullscreen.
 */
/**
 * \fn      Window::restore()
 * \brief   Restores the window.
 * \details If the window was recently maximized or minimized, restoring it will bring it back
 *          to it's original size and position before maximization or minimization. If the
 *          window is fullscreen, this will still restore it back to it's original state.
 */
/**
 * \fn      Window::show()
 * \brief   Makes the window visible.
 * \details Showing a window, at least on Microsoft systems, just means that it will become
 *          visible again from any other showing state. It normally gets restored as well,
 *          unless the specific command was to maximize.
 */
/**
 * \fn      Window::undecorate()
 * \brief   Changes the decorated state of this window to undecorated.
 * \details The window is considered decorated if it has borders and a caption.
 */
/**
 * \fn        Window::userResizable(bool)
 * \brief     Sets whether or not the user can resize the window.
 * \details   The window is considered resizable, at least on Microsoft systems, when the
 * 						borders are small and the maximization button is missing/disabled.
 * \param[in] userCanResize Whether or not the user can resize the window.
 */
/**
 * \fn      Window::systemHandle()
 * \brief   Gets the window handle for the window.
 * \details The window handle is a type erased version of the actual native window handle. It
 * 					should be casted to it's appropriate type.
 * \returns The WindowHandle instance for the window.
 */
/**
 * \fn      Window::parent()
 * \brief   ...
 * \details ...
 * \returns Window*
 */
/**
 * \fn      Window::position()
 * \brief   Gets the current position of the window in screen coordinates.
 * \details The position of the window is relative to virtual screen space. This value can be
 *          used for other window operations.
 * \returns The position of the window.
 */
/**
 * \fn      Window::size()
 * \brief   Gets the current size of the window in pixels.
 * \details The size of the window is measured in pixels. This is very important for performing
 *          rendering operations on the window. It is to be noted that the size of the window
 *          also include the frame of the window.
 * \returns The size of the window.
 */
/**
 * \fn      Window::title()
 * \brief   Gets the current title for the window.
 * \details The title of the window is a UTF-16 string presented on the caption of the window.
 * \returns The title for the window.
 */
/**
 * \fn        Window::reparent(Window*)
 * \brief     ...
 * \details   ...
 * \param[in] parent
 */
/**
 * \fn        Window::reposition(glm::ivec2)
 * \brief     Sets the position of this window.
 * \details   The position of the window is relative to the virtual screen space. The virtual
 *            screen space is described as the space that can encompass all the monitors on the
 *            system.
 * \param[in] pos The new position of the window.
 */
/**
 * \fn        Window::resize(glm::uvec2)
 * \brief     Sets the size of this window.
 * \details   Setting the size of the window will change the size of the entire frame, if the
 *            window is decorated. If the window isn't decorated, the client size will be the
 *            size provided.
 * \param[in] size The new size for this window.
 */
/**
 * \fn        Window::retitle(WideStringView)
 * \brief     Sets the title of this window.
 * \details   Settings the title of the window will change the caption of the window. The
 *            caption of the window is located at the top of the window between the window icon
 *            and the control buttons. When the window is undecorated, the caption will not show.
 * \param[in] title The new title of the window.
 */
/**
 * \fn        Window::create(const Window::CreateInfo*)
 * \brief 	  Creates a new window from the given create information.
 * \details   This function constructs a window on the appropriate environment and returns it. The caller is the owner
 * 					  of the created window.
 * \param[in] createInfo The information needed to create the window.
 * \returns   A pointer to the created window.
 */
/**
 * \fn        Window::destory(Window*)
 * \brief     Destroys the given window.
 * \param[in] window The window to destroy.
 */
/**
 * \struct  Window::CreateInfo
 * \brief	  The necessary information needed to create a window.
 */
/**
 * \property Window::CreateInfo::environment
 * \brief    The environment the window is being created for.
 */
/**
 * \property Window::CreateInfo::wndTitle
 * \brief    The title of the window to create.
 */
/**
 * \property Window::CreateInfo::wndSize
 * \brief    The size of the window we want to create.
 */
/**
 * \property Window::CreateInfo::wndPosition
 * \brief    The position of the window we want to create.
 */
/**
 * \property Window::CreateInfo::visible
 * \brief    Whether or not the window should be visible on create.
 */


  typedef struct tagWindowHandle* WindowHandle;

  class Window {
  public:
    static constexpr WideStringView kClassName = L"Hearth::Window";
    static constexpr WideStringView kMenuName  = L"Hearth::Window::Menu";

  public:
    struct CreateInfo;
    class Event;
    class CloseEvent;
    class FocusEvent;
    class MoveEvent;
    class ShowEvent;
    class SizeEvent;

  public:
    bool isDecorated() const noexcept { return mDecorated; }
    bool isFocused() const noexcept { return mFocused; }
    bool isMaximized() const noexcept { return mMaximized; }
    bool isMinimized() const noexcept { return mMinimized; }
    bool isRestored() const noexcept { return !mMinimized && !mMaximized; }
    bool isUserResizable() const noexcept { return mUserResizable; }
    bool isVisible() const noexcept { return mVisible; }

  public:
    virtual ~Window() noexcept = default;
    virtual void blur() noexcept = 0;
    virtual void decorate() noexcept = 0;
    virtual void flash() noexcept = 0;
    virtual void focus() noexcept = 0;
    virtual void hide() noexcept = 0;
    virtual void maximize() noexcept = 0;
    virtual void minimize() noexcept = 0;
    virtual void restore() noexcept = 0;
    virtual void show() noexcept = 0;
    virtual void undecorate() noexcept = 0;
    virtual void userResizable(bool userCanResize) noexcept = 0;
    virtual WindowHandle systemHandle() const noexcept = 0;
    virtual Window* parent() const noexcept = 0;
    virtual glm::ivec2 position() const noexcept = 0;
    virtual glm::uvec2 size() const noexcept = 0;
    virtual WideString title() const noexcept = 0;
    virtual void reparent(Window* parent) noexcept = 0;
    virtual void reposition(glm::ivec2 pos) noexcept = 0;
    virtual void resize(glm::uvec2 size) noexcept = 0;
    virtual void retitle(WideStringView title) noexcept = 0;

  public:
    static Window* create(const CreateInfo* createInfo) noexcept;
    static void destroy(Window* wnd) noexcept;

  protected:
    std::vector<WindowView> mChildren;
    Window* mParent;
    bool mFocused : 1 = false;
    bool mDecorated : 1 = false;
    bool mMaximized : 1 = false;
    bool mMinimized : 1 = false;
    bool mUserResizable : 1 = false;
    bool mVisible : 1 = false;
    bool mFirstShow : 1 = true;

  private:
    char unused : 1 = 0; // This is to pad out the struct.
  };

  struct Window::CreateInfo final {
    EnvironmentView environment;
    WideStringView wndTitle;
    glm::uvec2 wndSize;
    glm::ivec2 wndPosition;
    bool visible;
  };

  /**
   * \brief   Represents a Window specific event triggered by environment.
   * \details ...
   * \remarks Window events are triggered by user input to the window frame and client area.
   */
  class Window::Event : public Core::Event {
  public:
    /**
     * \brief     Constructs a new window event from the given window instance.
     * \details   ...
     * \param[in] window The window that had an event triggered on it.
     * \param[in] type The type of the window event.
     */
    explicit Event(const Window* window, EventType type) noexcept
      : mWindow{ window }
    {
      m_type = type;
    }

    virtual ~Event() noexcept = default;

  public:
    /**
     * \brief	  Gets the window that the event was triggered for.
     * \details ...
     * \returns The window that the event triggered for.
     */
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
  class Window::CloseEvent final : public Window::Event {
  public:
    /**
     * \brief     Constructs a new window close event from the given window.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     */
    explicit CloseEvent(const Window* wnd) noexcept
      : Window::Event(wnd, EventType::WindowClose)
    { }
  };

  /**
   * \brief   Represents an event described by the request to focus or unfocus a window.
   * \details ...
   */
  class Window::FocusEvent final : public Window::Event {
  public:
    /**
     * \brief     Constructs a new window focus event from the given window and focus state.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     * \param[in] focused Whether or not the window was focused.
     */
    explicit FocusEvent(const Window* wnd, bool focused) noexcept
      : Window::Event(wnd, EventType::WindowFocus)
      , mFocused(focused)
    { }

  public:
    /**
     * \brief   Checks if the window was focused during this event.
     * \details ...
     * \returns True if the window was focused, false otherwise.
     */
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
  class Window::MoveEvent final : public Window::Event {
  public:
    /**
     * \brief	    Constructs a new window move event from the given window and position.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     * \param[in] pos The new position of the window.
     */
    explicit MoveEvent(const Window* wnd, glm::ivec2 pos) noexcept
      : Window::Event(wnd, EventType::WindowMove)
      , mPosition{ pos }
    { }

  public:
    /**
     * \brief	  Gets the new position of the window.
     * \details ...
     * \returns The new position of the window as tracked by the environment.
     */
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
  class Window::ShowEvent final : public Window::Event {
  public:
    /**
     * \brief	    Constructs a new window show event from the given window and visibility.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     * \param[in] visible Whether or not the window is visible.
     */
    explicit ShowEvent(const Window* wnd, bool visible) noexcept
      : Window::Event(wnd, EventType::WindowShow)
      , mVisible(visible)
    { }

  public:
    /**
     * \brief   Check if the window was visible when the event was triggered.
     * \details ...
     * \returns The state of the window's visibility.
     */
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
  class Window::SizeEvent final : public Window::Event {
  public:
    /**
     * \brief     Constructs a new window size event from the window and the new size.
     * \details   ...
     * \param[in] wnd The window to create this event from.
     * \param[in] size The new size of the window.
     * \param[in] sizeState The new size state of the window.
     */
    explicit SizeEvent(const Window* wnd, glm::uvec2 size, SizeState sizeState) noexcept
      : Window::Event(wnd, EventType::WindowSize)
      , mSize(size)
      , mSizeState(sizeState)
    { }

  public:
    /**
     * \brief   Gets the new size of the window.
     * \details ...
     * \returns The new size of the window.
     */
    glm::uvec2 size() const noexcept { return mSize; }

    /**
     * \brief	  Gets the new size state of the window.
     * \details ...
     * \returns The new size state of the window.
     */
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


#endif /* __INC_HEARTH_CORE_WINDOW_HPP__ */
