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
#include <hearth/environment.hpp>
#include "winapi.hpp"

namespace HAPI_NAMESPACE_NAME::native {

  //! \brief Represents a Windows operating system environment.
  class Win32Environment final : public Environment {
  public:
    //! \brief Default constructor.
    Win32Environment();

    //! \brief Default destructor.
   ~Win32Environment() noexcept;

  public:
    /*!
     * \brief  Gets the window class for this environment.
     * \return A const reference to the window class created by this environment.
     */
    const WNDCLASSEXW& windowClass() const noexcept;

    /*!
     * \brief  Gets the window class for this environment.
     * \return A non-const reference to the window class created by this environment.
     */
    WNDCLASSEX& windowClass() noexcept;

    /*!
     * \brief     Gets the error message for the provided error code.s
     * \param[in] error The error code for the message to retrieve.
     * \return    The collected error messsage.
     */
    std::wstring errorMessage(DWORD error) const noexcept;

    /*!
     * \brief     Handles the given signal from the console.
     * \param[in] signal The signal to process.
     * \return    True if the signal was handled, false otherwise.
     */
    static BOOL WINAPI consoleHandler(DWORD signal) noexcept;

  private:
    /*!
     * \brief Creates this Windows operating system environment.
     *
     * To create this environment the window class must be created and registered properly without
     * fault, and the working directory must be retrieved.
     */
    void create() final;

    /*!
     * \brief Destroys this Windows operating system environment.
     *
     * Destroying this environment means that all open Windows created by this environment will be
     * destroyed. Once all windows are destroyed, the window class will be unregistered and the
     * environment will returned to the state it was before application execution.
     */
    void destroy() noexcept final;

    //! \copydoc Environment::pollEvents()
    void pollEvents() const noexcept final;

  private:
    //! \brief The window class for windows created with this environment.
    WNDCLASSEXW mWindowClass;
  };

}
