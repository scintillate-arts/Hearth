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
 * \file    WinAPI.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_NATIVE_WINAPI_HPP__
#define __HEARTH_NATIVE_WINAPI_HPP__ 1
#include <Hearth/Core/Config.hpp>

// First verison of the UNICODE macro defined by Windows.
#ifndef UNICODE
#  define UNICODE 1
#endif /* UNICODE */

// Second version of the UNICODE macro defined by Windows.
#ifndef _UNICODE
#  define _UNICODE 1
#endif /* _UNICODE */

// Generally removes stuff we don't need right now, like cryptography.
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN 1
#endif /* WIN32_LEAN_AND_MEAN */

// Removes stuff we don't need when running on Visual Studio
#ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN 1
#endif /* VC_EXTRALEAN */

// Must be enabled to prevent lib issues, might already be defined by MinGW.
#ifndef NOMINMAX
#  define NOMINMAX 1
#endif /* NOMINMAX */

// Set version Windows 8 or later.
#ifndef WINVER
#  define WINVER 0x0602
#else
#  undef WINVER
#  define WINVER 0x0602
#endif /* WINVER */

// Set version Windows 8 or later.
#ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0602
#else
#  undef _WIN32_WINNT
#  define _WIN32_WINNT 0x0602
#endif

// Include necessary types.
#include <cwctype>
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <dbt.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#include <versionhelpers.h>

// This may not be defined where it should be and thus may not have been included.
#ifndef EDS_ROTATEDMODE
#  define EDS_ROTATEDMODE 0x0000'0004
#endif /* EDS_ROTATEDMODE */

// This may not be defined where it should be and thus may not have been included.
#ifndef DISPLAY_DEVICE_ACTIVE
#  define DISPLAY_DEVICE_ACTIVE 0x0000'0001
#endif /* DISPLAY_DEVICE_ACTIVE */

// This may not be defined where it should be and thus may not have been included.
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
#  define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((HANDLE)-4)
#endif /* DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 */

// Importing functions from DLLs instead of opening a DLL instance and pulling them.
extern "C" {
  HEARTH_IMPORT BOOL WINAPI EnableNonClientDpiScaling(HWND hWnd);
  HEARTH_IMPORT UINT WINAPI GetDpiForWindow(HWND hWnd);
  HEARTH_IMPORT BOOL WINAPI AdjustWindowRectExForDpi(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi);
}

#endif /* __HEARTH_NATIVE_WINAPI_HPP__ */
