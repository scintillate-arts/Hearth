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
#include <functional>
#include <glm/glm.hpp>
#include "environment.hpp"

namespace HAPI_NAMESPACE_NAME {

  /*!
   * \brief Represents a platform agnostic window.
   */
  class Window {
  public:
    //! \brief The class name for windows created by Hearth.
    static const std::wstring_view kClassname;

    //! \brief The properties name for windows created by Hearth.
    static const std::wstring_view kPropname;

  public:
    /*!
     * \brief The information needed for creating a window.
     *
     * Most of the data of this is inlined into the window, because extra bit flags are added.
     */
    struct CreateInfo {
      //! \brief The name of the window being created.
      std::wstring_view wndName;

      //! \brief The environment the window being created, exists in.
      Environment* wndResidency;

      //! \brief The position of the window on screen.
      glm::ivec2 wndPosition;

      //! \brief The size of the window.
      glm::uvec2 wndSize;

      //! \brief The opacity of the window.
      float opacity;

      //! \brief Whether or not the window should be fullscreen on creation.
      bool fullscreen;

      //! \brief Whether or not the window should be decorated on creation.
      bool decorated;

      //! \brief Whether or not the window should be user resizable on creation.
      bool userResizable;

      /*!
       * \brief  Whether or not the window should be transparent on creation.
       * \remark The window opacity is set to 0 by default.
       */
      bool transparent;

      //! \brief Whether or not the window should be visible on creation.
      bool visible;
    };

  protected:
    /*!
     * \brief     Explicitly defined constructor, constructs this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     *
     * Makes sure this objecct is added to the environment.
     */
    Window(const CreateInfo& createInfo);

    /*!
     * \brief     Explicitly defined move constructor, constructs this object from the other.
     * \param[in] other The data to move into this object.
     */
    Window(Window&& other) noexcept = default;

    /*!
     * \brief     Explicitly defined move assignment operator, assigns the data of the other to this.
     * \param[in] other The data to move into this object.
     * \return    This window with the new data.
     */
    Window& operator=(Window&& other) noexcept = default;

  public:
    /*!
     * \brief Explicitly defined destructor, prepares this object for destruction.
     *
     * Makes sure this object is removed from the environment.
     */
    virtual ~Window() noexcept;

  public:
    /*!
     * \brief     Creates a new window and returns it's pointer.
     * \param[in] createInfo The information to create the window.
     * \return    The a pointer to the created window.
     */
    static Window* create(const CreateInfo& createInfo) noexcept;

    /*!
     * \brief     Destroys the given window.
     * \param[in] window The window to destroy.
     */
    static void destroy(Window* window) noexcept;

  public:
    //! \brief Sets the window to close.
    virtual void close() noexcept = 0;

    /*!
     * \brief  Makes this window invisible.
     * \return If the window is fullscreen, this won't do anything to it.
     */
    virtual void hide() noexcept = 0;

    //! \brief Brings this window to the front of all others and sets input focus.
    virtual void focus() noexcept = 0;

    //! \brief Maximizes this window on the screen.
    virtual void maximize() noexcept = 0;

    //! \brief Minimizes this window on the screen.
    virtual void minimize() noexcept = 0;

    //! \brief Requests user attention to this window.
    virtual void requestAttention() noexcept = 0;

    //! \brief Restores this window to it's last position and size state.
    virtual void restore() noexcept = 0;

    /*!
     * \brief  This this window visible.
     * \remark If the window is fullscreen, this won't do anything to it.
     */
    virtual void show() noexcept = 0;

    /*!
     * \brief     Changes the behaviour of this window's frame, enabling or disabling things like
     *            the title bar and borders.
     * \param[in] decorated Whether or not this window should be decorated.
     */
    virtual void enableDecorations(bool decorated) noexcept = 0;

    /*!
     * \brief     Changes the window's fullscreen state.
     * \param[in] fullscreen Whether or not the window should be fullscreen.
     */
    virtual void enableFullscreen(bool fullscreen) noexcept = 0;

    /*!
     * \brief     Changes the behaviour of this window's resizability, enabling or disabling it.
     * \param[in] canResize Whether or not the user can resize this window.
     */
    virtual void enableUserResizability(bool canResize) noexcept = 0;

    /*!
     * \brief     Changes the behaviour of this window's composition, allowing it to be transparent
     *            or not.
     * \param[in] transparent Whether or not the window should be able to transparent or not.
     */
    virtual void enableTransparency(bool transparent) noexcept = 0;

    /*!
     * \brief     Sets the opacity of this window.
     * \param[in] opacity The new opacity for this window.
     */
    virtual void reopacify(float opacity) noexcept = 0;

    /*!
     * \brief     Sets the position of this window on the screen.
     * \param[in] newPos The new position for this window in virtual coordinates.
     */
    virtual void reposition(const glm::ivec2& newPos) noexcept = 0;

    /*!
     * \brief     Sets the size of this window.
     * \param[in] newSize The new size for this window.
     */
    virtual void resize(const glm::uvec2& newSize) noexcept = 0;

    /*!
     * \brief     Sets the title of this window.
     * \param[in] name The new title for this window.
     */
    virtual void retitle(std::wstring_view name) noexcept = 0;

    /*!
     * \brief  Checks if this window is open still.
     * \return The open state of this window.
     */
    virtual bool isOpen() const noexcept = 0;

  public:
    /*!
     * \brief  Checks if this window is decorated.
     * \return The decorated state of this window.
     */
    bool isDecorated() const noexcept;

    /*!
     * \brief  Checks if this window is focused.
     * \return The focused state of this window.
     */
    bool isFocused() const noexcept;

    /*!
     * \brief  Checks if this window is fullscreen.
     * \return The fullscreen state of this window.
     */
    bool isFullscreen() const noexcept;

    /*!
     * \brief  Checks if this window is maximized.
     * \return The maximized state of this window.
     */
    bool isMaximized() const noexcept;

    /*!
     * \brief  Checks if this window is minimized.
     * \return The minimized state of this window.
     */
    bool isMinimized() const noexcept;

    /*!
     * \brief  Checks if this window is restored.
     * \return The restored state of this window.
     * \remark If the window is not maximized and not minimized, it is considered restored.
     */
    bool isRestored() const noexcept;

    /*!
     * \brief  Checks if this window has transparency enabled or is transparent.
     * \return The transparency state of this window.
     */
    bool isTransparent() const noexcept;

    /*!
     * \brief  Checks if this Window is user resizable.
     * \return The user resizability state of this window.
     */
    bool isUserResizable() const noexcept;

    /*!
     * \brief  Checks if this window is visible.
     * \return The visibility state of this window.
     */
    bool isVisible() const noexcept;

    /*!
     * \brief  Gets the opacity of this window.
     * \return The current opacity of this window.
     */
    float opacity() const noexcept;

    /*!
     * \brief  Gets the current position of this window in virtual coordinates.
     * \return The position of this window.
     */
    glm::ivec2 position() const noexcept;

    /*!
     * \brief  Gets the current size of this window.
     * \return The size of this window.
     */
    glm::uvec2 size() const noexcept;

    /*!
     * \brief  Gets the current title of this window.
     * \return The title of this window.
     */
    std::wstring_view title() const noexcept;

  protected:
    //! \brief The name of the window being created.
    std::wstring mName;

    //! \brief The environment the window being created, exists in.
    Environment* mResidency;

    //! \brief The virtual coordinates of the window on screen.
    glm::ivec2 mPosition;

    //! \brief The size of the window.
    glm::uvec2 mSize;

    //! \brief The opacity of this window.
    float mOpacity;

    //! \brief Whether or not the window is focused.
    bool mFocused : 1;

    //! \brief Whether or not the window is fullscreen.
    bool mFullscreen : 1;

    //! \brief Whether or not the window is decorated.
    bool mDecorated : 1;

    //! \brief Whether or not the window is maximized.
    bool mMaximized : 1;

    //! \brief Whether or not the window is minimized.
    bool mMinimized : 1;

    //! \brief Whether or not the window is resizable.
    bool mUserResizable : 1;

    //! \brief  Whether or not the window is transparent.
    bool mTransparent : 1;

    //! \brief Whether or not the window is visible.
    bool mVisible : 1;
  };

  // Defining these.
  constexpr std::wstring_view Window::kClassname = L"Hearth::Window";
  constexpr std::wstring_view Window::kPropname  = L"Hearth::Window::Prop";

}
