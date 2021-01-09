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
#ifndef __HEARTH_CORE_WINDOW_HPP__
#define __HEARTH_CORE_WINDOW_HPP__ 1
#include <cstdint>
#include <memory>
#include <string_view>
#include <glm/glm.hpp>
#include "Environment.hpp"
#include "Event.hpp"

namespace Hearth {

  /**
   * \breif   Represents a platform agnostic native window handle.
   * \details ...
   */
  typedef struct tagWindowHandle* WindowHandle;

  /**
   * \brief	  Represents an abstraction of the window class.
   * \details	...
   */
  struct Window {
    /**
     * \brief   The name of the class that is used by Hearth.
     * \details ...
     */
    static constexpr std::wstring_view kClassName = L"Hearth::Window";

    /**
     * \brief   The name of the window menu class is used by Hearth.
     * \details ...
     */
    static constexpr std::wstring_view kMenuName = L"Hearth::Window::Menu";

  public:
    /**
     * \brief	  The necessary information needed to create a window.
     * \details ...
     */
    struct CreateInfo final {
      /**
       * \brief   The environment the window is being created for.
       * \details ...
       */
      std::shared_ptr<Environment> environment;

      /**
       * \brief   The title of the window.
       * \details ...
       */
      std::wstring_view wndTitle;

      /**
       * \brief   The size of the window we want to create.
       * \details ...
       */
      glm::uvec2 wndSize;

      /**
       * \brief   The position of the window we want to create.
       * \details ...
       */
      glm::ivec2 wndPosition;
    };

  public:
    /**
     * \brief 	  Creates a new window from the given create information.
     * \details   This function constructs a window on the appropriate environment and returns it. The caller is the owner
     * 					  of the created window.
     * \param[in] createInfo The information needed to create the window.
     * \returns   A pointer to the created window.
     */
    static Window* create(const CreateInfo* createInfo) noexcept;

    /**
     * \brief     Destroys the given window.
     * \details   ...
     * \param[in] window The window to destroy.
     */
    static void destroy(Window* wnd) noexcept;

  public:
    /**
     * \brief   Default destructor for window types.
     * \details ...
     */
    virtual ~Window() noexcept = default;

    /**
     * \brief   Unfocuses the window.
     * \details This will leave the window at the top, but it will no longer receive keyboard input.
     */
    virtual void blur() noexcept = 0;

    /**
     * \brief   Changes the decorated state of this window to decorated.
     * \details The window is considered decorated if it has borders and a caption.
     */
    virtual void decorate() noexcept = 0;

    /**
     * \brief   Attempts to get user attention.
     * \details Flashes the window, and often the taskbar button for the window, to grab the user's
     *          attention.
     */
    virtual void flash() noexcept = 0;

    /**
     * \brief   Requests the window back into focus.
     * \details This will bring the window to the top, set it as the foreground, and finally set it
     *          to receive input focus.
     */
    virtual void focus() noexcept = 0;

    /**
     * \brief   Makes the window hidden.
     * \details Hiding a window, at least on Microsoft systems, just means that it gets put into the
     *          `system tray`. It still exists, it's not minimized technically, it's just hiding.
     */
    virtual void hide() noexcept = 0;

    /**
     * \brief   Maximizes the window.
     * \details This will bring the frame of the window to the size of the monitor it is on. Though,
     *          it will not work if the window is fullscreen.
     */
    virtual void maximize() noexcept = 0;

    /**
     * \brief   Minimizes the window.
     * \details This will bring the frame of the window to 0 width and 0 height, and dock it on the
     *          taskbar. This will not work if the window is fullscreen.
     */
    virtual void minimize() noexcept = 0;

    /**
     * \brief   Restores the window.
     * \details If the window was recently maximized or minimized, restoring it will bring it back
     *          to it's original size and position before maximization or minimization. If the
     *          window is fullscreen, this will still restore it back to it's original state.
     */
    virtual void restore() noexcept = 0;

    /**
     * \brief   Makes the window visible.
     * \details Showing a window, at least on Microsoft systems, just means that it will become
     *          visible again from any other showing state. It normally gets restored as well,
     *          unless the specific command was to maximize.
     */
    virtual void show() noexcept = 0;

    /**
     * \brief   Changes the decorated state of this window to undecorated.
     * \details The window is considered decorated if it has borders and a caption.
     */
    virtual void undecorate() noexcept = 0;

    /**
     * \brief     Sets whether or not the user can resize the window.
     * \details   The window is considered resizable, at least on Microsoft systems, when the
     * 						borders are small and the maximization button is missing/disabled.
     * \param[in] userCanResize Whether or not the user can resize the window.
     */
    virtual void userResizable(bool userCanResize) noexcept = 0;

    /**
     * \brief   Gets the window handle for the window.
     * \details The window handle is a type erased version of the actual native window handle. It
     * 					should be casted to it's appropriate type.
     * \returns The WindowHandle instance for the window.
     */
    [[nodiscard]]
    virtual WindowHandle handle() const noexcept = 0;

    /**
     * \brief   Gets the current position of the window in screen coordinates.
     * \details The position of the window is relative to virtual screen space. This value can be
     *          used for other window operations.
     * \returns The position of the window.
     */
    [[nodiscard]]
    virtual glm::ivec2 position() const noexcept = 0;

    /**
    * \brief   Gets the current size of the window in pixels.
    * \details The size of the window is measured in pixels. This is very important for performing
    *          rendering operations on the window. It is to be noted that the size of the window
    *          also include the frame of the window.
    * \returns The size of the window.
    */
    [[nodiscard]]
    virtual glm::uvec2 size() const noexcept = 0;

    /**
     * \brief   Gets the current title for the window.
     * \details The title of the window is a UTF-16 string presented on the caption of the window.
     * \returns The title for the window.
     */
    [[nodiscard]]
    virtual std::wstring title() const noexcept = 0;

    /**
     * \brief     Sets the position of this window.
     * \details   The position of the window is relative to the virtual screen space. The virtual
     *            screen space is described as the space that can encompass all the monitors on the
     *            system.
     * \param[in] pos The new position of the window.
     */
    virtual void reposition(glm::ivec2 pos) noexcept = 0;

    /**
     * \brief     Sets the size of this window.
     * \details   Setting the size of the window will change the size of the entire frame, if the
     *            window is decorated. If the window isn't decorated, the client size will be the
     *            size provided.
     * \param[in] size The new size for this window.
     */
    virtual void resize(glm::uvec2 size) noexcept = 0;

    /**
     * \brief     Sets the title of this window.
     * \details   Settings the title of the window will change the caption of the window. The
     *            caption of the window is located at the top of the window between the window icon
     *            and the control buttons. When the window is undecorated, the caption will not show.
     * \param[in] title The new title of the window.
     */
    virtual void retitle(std::wstring_view title) noexcept = 0;

  public:
    /**
     * \brief   Checks if this window is decorated.
     * \details This will return the decorated state of the window. To change it call either:
     *          `decorate()` or `undecorate()`.
     * \returns True if the window is decorated, false otherwise.
     */
    [[nodiscard]]
    bool isDecorated() const noexcept { return mDecorated; }

    /**
     * \brief   Checks if the window is focused.
     * \details This will return the focused state of the window. If the window becomes unfocused,
     *          you can refocus it with `focus()`.
     * \returns True if the window is focused, false otherwise.
     */
    [[nodiscard]]
    bool isFocused() const noexcept { return mFocused; }

    /**
     * \brief   Checks if the window is maximized.
     * \details This will return the maximized state of the window. To change it call either:
     *          `maximize()`, `minimize()`, or `restore()`. The window cannot be maximized when it
     *          is fullscreen, so this will return false in that case.
     * \returns True if the window is maximized, false otherwise.
     */
    [[nodiscard]]
    bool isMaximized() const noexcept { return mMaximized; }

    /**
     * \brief   Checks if the window is minimized.
     * \details This will return the minimized state of the window. To change it call either:
     *          `maximize()`, `minimize()`, or `restore()`. The window cannot be minimized when it
     *          is fullscreen, so this will return false in that case.
     * \returns True if the window is minimized, false otherwise.
     */
    [[nodiscard]]
    bool isMinimized() const noexcept { return mMinimized; }

    /**
     * \brief   Checks if the window is restored.
     * \details This will return true if the window is not maximized and not minimized. This will
     *          return false if the window is either maximized or minimized. If the window is
     *          fullscreen, this will return true, because the fullscreen state is considered to be
     *          a restored state.
     * \returns True if the window is restored, false otherwise.
     */
    [[nodiscard]]
    bool isRestored() const noexcept { return !mMinimized && !mMaximized; }

    /**
     * \brief   Checks whether or not the window is user resizable.
     * \details The window is considered user resizable if the window has thick borders and the
     *          maximize control button. Otherwise it is not user resizable. To change this state,
     *          use the `enableUserResizability(bool)` function.
     * \returns True if the window is user resizable, false otherwise.
     */
    [[nodiscard]]
    bool isUserResizable() const noexcept { return mUserResizable; }

    /**
     * \brief   Checks if the window is visible.
     * \details The window is considered visible if it's shown, even if it is minimized. It's only
     *          when the window is hidden that it is not visible. Hiding a window puts it into the
     *          system tray. To change this state use either `show()` or `hide()`.
     * \returns True if the window is visible, false otherwise.
     */
    [[nodiscard]]
    bool isVisible() const noexcept { return mVisible; }

  protected:
    /**
     * \brief   Whether or not this window currently has focus.
     * \details The window has focus when it is the top level window, the foreground process, and
     *          can accept input and produce events.
     */
    bool mFocused : 1 = false;

    /**
     * \brief   Whether or not this window is decorated.
     * \details The window is created in the decorated state. When decorated the window has it's
     *          title and it's borders. When undecorated the window has nothing and is just the
     *          client area. The client area refers to the portion by which rendering can be done on
     *          the window. It's what we'd also refer to as a `pop-up window`.
     */
    bool mDecorated : 1 = false;

    /**
     * \brief   Whether or not this window is maximized.
     * \details A window is maximized, in non-fullscreen mode, if the window received the state
     *          change event from the platform. This event forces the window to be the size of the
     *          monitor, excluding the task bar. A undecorated maximized window has a state that
     *          most applications are calling `borderless fullscreen` nowadays. Performance in this
     *          mode is actually better sometimes.
     */
    bool mMaximized : 1 = false;

    /**
     * \brief   Whether or not this window is minimized.
     * \details A window is minimized, in non-fullscreen mode, if the window received the state
     *          change event from the platform. This event forces the window to be set into the
     *          background and have 0 width and 0 height. It's only available on the task bar in
     *          this state. It's not to be confused with being hidden however, which would move it
     *          to the `Tray` (on Microsoft systems).
     */
    bool mMinimized : 1 = false;

    /**
     * \brief   Whether or not this window is user resizable.
     * \details A user resizable window has very specific window features which enable a user to
     *          resize it. Usually the maximize button is enabled and has large borders. When the
     *          window is not user resizable, those features of the window are missing.
     */
    bool mUserResizable : 1 = false;

    /**
     * \brief   Whether or not the window is visible.
     * \details This has nothing to do with the opacity of the window but rather whether or not the
     *          window is showing or is hiding. When the window is hiding, at least on Microsoft
     *          systems, it's in the `Tray`. Clicking on it should give you options, if an
     *          application has set up the registry options for it.
     */
    bool mVisible : 1 = false;

  private:
    char unused : 2 = 0; // This is to pad out the struct.
  };

}


#endif /* __HEARTH_CORE_WINDOW_HPP__ */
