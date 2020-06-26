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
#include <hearth/config.hpp>
#if !defined(UNICODE)
#  define UNICODE
#endif
#if !defined(WIN32_LEAN_AND_MEAN)
#  define WIN32_LEAN_AND_MEAN
#endif
#if !defined(VC_EXTRALEAN)
#  define VC_EXTRALEAN
#endif

// Set version for Windows 8 or eariler.
#if defined(WINVER)
#  undef  WINVER
#  define WINVER 0x0602
#else
#  define WINVER 0x0602
#endif

// Set version for Windows 8 or earlier.
#if defined(_WIN32_WINNT)
#  undef  _WIN32_WINNT
#  define _WIN32_WINNT 0x0602
#else
#  define _WIN32_WINNT 0x0602
#endif

// Include the rest of the headers we need.
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#include <versionhelpers.h>

#if !defined(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)
#  define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((HANDLE)-4)
#endif

/*!
 * \brief     Checks if the version of the running Windows operating system instance is Windows 10
 *            or earlier.
 * \param[in] ver The Windows operating system revision number.
 * \return    True if the version is Windows 10 or eariler.
 */
BOOL WINAPI IsWindows10OrEarlier(DWORD ver) noexcept;

/*!
 * \brief  Checks if the version of the running Windows operating system instance is Windows 10
 *         anniversary update or earlier.
 * \return True if the version is Windows 10 anniversary update or earlier.
 */
BOOL WINAPI IsWindows10AnniversaryOrEarlier() noexcept;

/*!
 * \brief  Checks if the version of the running Windows operating system instance is Windows 10
 *         creators update or earlier.
 * \return True if the version is Windows 10 creators update or earlier.
 */
BOOL WINAPI IsWindows10CreatorsOrEarlier() noexcept;

/*!
 * \brief  Checks if the version of the running Windowss operating system instance is Window 10
 *         fall creators update or earlier.
 * \return True if the version is Windows 10 fall creators or earlier.
 */
BOOL WINAPI IsWindows10FallCreatorsOrEarlier() noexcept;

extern "C" {
  HAPI_IMPORT BOOL WINAPI EnableNonClientDpiScaling(HWND hWnd);
  HAPI_IMPORT UINT WINAPI GetDpiForWindow(HWND hWnd);
  HAPI_IMPORT BOOL WINAPI AdjustWindowRectExForDpi(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi);
}
