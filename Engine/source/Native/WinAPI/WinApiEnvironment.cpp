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
 * \file    WinApiEnvironment.cpp
 * \brief   ...
 * \details ...
 */
#include <iostream>
#include <Hearth/Core/Application.hpp>
#include "WinApiEnvironment.hpp"
#include "WinApiEventHandler.hpp"
#if HEARTH_WINDOWS_PLATFORM

static void throwMessageBoxAssertion(const wchar_t* errMsg) noexcept {
  MessageBoxW(nullptr, errMsg, L"Error encountered", MB_OK);
  std::exit(EXIT_FAILURE);
}

namespace Hearth {

  void WinAPIEnvironment::initialize() noexcept {
    // Do not reinitialize.
    if (mIsInitialized)
      return;

    // Prep object for population.
    ZeroMemory(&mWindowClass, sizeof(WNDCLASSEX));

    // Set properties of window class.
    mWindowClass.cbSize        = sizeof(WNDCLASSEX);
    mWindowClass.style         = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
    mWindowClass.hInstance     = GetModuleHandle(nullptr);
    mWindowClass.lpfnWndProc   = &WinAPIEventHandler::wndProc;
    mWindowClass.lpszClassName = Window::kClassName.data();
    mWindowClass.lpszMenuName  = Window::kMenuName.data();
    if (mWindowClass.hInstance == nullptr)
      std::cout << "Hmm seems fishy.\n";

    // Attempt to register window class.
    if (FAILED(RegisterClassEx(&mWindowClass))) {
      std::wstring err(errorMessage(GetLastError()));
      throwMessageBoxAssertion(err.c_str());
    }

    // Set console control handler.
    if (FAILED(SetConsoleCtrlHandler(&consoleControlHandler, TRUE))) {
      std::wstring err(errorMessage(GetLastError()));
      throwMessageBoxAssertion(err.c_str());
    }

    // Whew we made it!
    mIsInitialized = true;
  }

  void WinAPIEnvironment::terminate() noexcept {
    // Do not reterminate.
    if (!mIsInitialized)
      return;

    // Unregister window class.
    if (FAILED(UnregisterClass(mWindowClass.lpszClassName, mWindowClass.hInstance))) {
      std::wstring err(errorMessage(GetLastError()));
      throwMessageBoxAssertion(err.c_str());
    }

    // Good to go!
    mIsInitialized = false;
  }

  void WinAPIEnvironment::pollEvents() const noexcept {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  [[nodiscard]]
  Platform WinAPIEnvironment::platform() const noexcept {
    return Platform::Windows;
  }

  [[nodiscard]]
  const WNDCLASSEX& WinAPIEnvironment::windowClass() const noexcept {
    return mWindowClass;
  }

  BOOL CALLBACK WinAPIEnvironment::consoleControlHandler(DWORD dwCtrlType) noexcept {
    // Check if this was a CTRL+C event.
    if (dwCtrlType == CTRL_C_EVENT) {
      // Get the running environment.
      auto env = Environment::instance();
      for (auto app : env->trackedApps())
        app->quit(true);
      return TRUE;
    }

    // Event wasn't handled, leave to windows to handle.
    return FALSE;
  }

  std::wstring HEARTHCALL WinAPIEnvironment::errorMessage(DWORD error) noexcept {
    WCHAR errMsg[512];
    DWORD fmwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    DWORD msgLen   = FormatMessageW(fmwFlags, nullptr, error, 0, errMsg, 512, nullptr);

    // Error code not found, try different source.
    if (msgLen == 0) {
      HINSTANCE ntdsbmsg = LoadLibraryW(L"Ntdsbmsg.dll");
      if (ntdsbmsg == nullptr)
        return L"Unknown fatal error occurred!";

      fmwFlags = FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS;
      msgLen   = FormatMessageW(fmwFlags, ntdsbmsg, error, 0, errMsg, 512, nullptr);
      FreeLibrary(ntdsbmsg);
    }

    // If we still don't have an error message.
    if (msgLen == 0)
      return L"Unknown fatal error occurred!";

    // Return what we got.
    return std::wstring(errMsg,errMsg + msgLen);
  }

  BOOL HEARTHCALL WinAPIEnvironment::IsWindows10OrLater(DWORD ver) noexcept {
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion      = 10;
    osvi.dwMinorVersion      = 0;
    osvi.dwBuildNumber       = ver;

    DWORD     mask = VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER;
    ULONGLONG cond = ::VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);

    cond = VerSetConditionMask(cond, VER_MINORVERSION, VER_GREATER_EQUAL);
    cond = VerSetConditionMask(cond, VER_BUILDNUMBER,  VER_GREATER_EQUAL);
    return VerifyVersionInfo(&osvi, mask, cond) == 0;
  }

  BOOL HEARTHCALL WinAPIEnvironment::IsWindows10AnniversaryOrLater() noexcept {
    return IsWindows10OrLater(14393);
  }

  BOOL HEARTHCALL WinAPIEnvironment::IsWindows10CreatorsOrLater() noexcept {
    return IsWindows10OrLater(15063);
  }

  BOOL HEARTHCALL WinAPIEnvironment::IsWindows10FallCreatorsOrLater() noexcept {
    return IsWindows10OrLater(16299);
  }

  std::shared_ptr<Environment> Environment::instance() noexcept {
    static std::shared_ptr<Environment> spSingleton = std::make_shared<WinAPIEnvironment>();
    return spSingleton;
  }

}

#endif /* HEARTH_WINDOWS_PLATFORM */