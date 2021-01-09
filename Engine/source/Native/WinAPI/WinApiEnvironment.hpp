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
 * \file    WinApiEnvironment.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_NATIVE_WINAPI_ENVIRONMENT_HPP__
#define __HEARTH_NATIVE_WINAPI_ENVIRONMENT_HPP__ 1
#include "Hearth/Core/Environment.hpp"

/* Windows only */
#if HEARTH_WINDOWS_PLATFORM
#include "WinAPI.hpp"

namespace Hearth {

  /**
   * \brief 	A window's specific implementation of the Environment.
   * \details ...
   */
  class WinAPIEnvironment : public IEnvironment {
  public:
    /**
     * \copydoc IEnvironment::initialize()
     */
    void initialize() noexcept override;

    /**
     * \copydoc IEnvironment::terminate()
     */
    void terminate() noexcept override;

    /**
     * \copydoc IEnvironment::pollEvents()
     */
    void pollEvents() const noexcept override;

    /**
     * \copydoc IEnvironment::platform()
     * \returns The correct platform, in this case Platform::Windows.
     */
    [[nodiscard]]
    Platform platform() const noexcept override;

  public:
    /**
     * \brief 	Gets the window class managed by this environment.
     * \details ...
     * \returns The window class that was created with this environment.
     */
    [[nodiscard]]
    const WNDCLASSEX& windowClass() const noexcept;

  public:
    /**
     * \brief     Handles control events from the command line.
     * \details   ...
     * \param[in] dwCtrlType The type of the control input that was made.
     * \returns   TRUE if handled, FALSE otherwise.
     */
    static BOOL CALLBACK consoleControlHandler(DWORD dwCtrlType) noexcept;

    /**
     * \brief 		Gets the error message for a given error code.
     * \details   ...
     * \param[in] error The error code to get the error message for.
     * \returns   The retrieved error message.
     */
    static std::wstring HEARTHCALL errorMessage(DWORD error) noexcept;

    /**
     * \brief			Checks if the environment is Windows 10 or later given a version number.
     * \details 	...
     * \param[in] ver The version to check.
     * \returns   TRUE if the version provided is Windows 10 or later, FALSE otherwise.
     */
    static BOOL HEARTHCALL IsWindows10OrLater(DWORD ver) noexcept;

    /**
     * \brief 	Checks if the environment is Windows 10 Anniversary verison or Later.
     * \details ...
     * \returns TRUE if the verison is Windows 10 Anniversary or later, FALSE otherwise.
     */
    static BOOL HEARTHCALL IsWindows10AnniversaryOrLater() noexcept;

    /**
     * \brief 	Checks if the environment is Windows 10 Creators verison or Later.
     * \details ...
     * \returns TRUE if the verison is Windows 10 Creators or later, FALSE otherwise.
     */
    static BOOL HEARTHCALL IsWindows10CreatorsOrLater() noexcept;

    /**
     * \brief 	Checks if the environment is Windows 10 Fall Creators verison or Later.
     * \details ...
     * \returns TRUE if the verison is Windows 10 Fall Creators or later, FALSE otherwise.
     */
    static BOOL HEARTHCALL IsWindows10FallCreatorsOrLater() noexcept;

  private:
    /**
     * \brief 	The window class for the environment.
     * \details ...
     */
    WNDCLASSEX mWindowClass;
  };

}

#endif /* HEARTH_WINDOWS_PLATFORM */
#endif /* __HEARTH_NATIVE_WINAPI_ENVIRONMENT_HPP__ */
