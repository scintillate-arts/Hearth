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
#include <iostream>
#include <stdexcept>
#include <hearth/application.hpp>
#include "environment.hpp"
#include "window.hpp"

namespace HAPI_NAMESPACE_NAME::native {

  Win32Environment::Win32Environment() :
    Environment(Environment::CreateInfo{ Environment::Platform::Windows })
  {
    create();
  }

  Win32Environment::~Win32Environment() noexcept {
    destroy();
  }

  const WNDCLASSEXW& Win32Environment::windowClass() const noexcept {
    return mWindowClass;
  }

  WNDCLASSEXW& Win32Environment::windowClass() noexcept {
    return mWindowClass;
  }

  std::wstring Win32Environment::errorMessage(DWORD error) const noexcept {
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
    return std::wstring(errMsg, errMsg + msgLen);
  }

  BOOL WINAPI Win32Environment::consoleHandler(DWORD signal) noexcept {
    // Process events.
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) {
      // Tell all applications to quit.
      for (auto* app : mRunningApps)
        app->quit(true);

      // Was handled.
      return TRUE;
    }

    // Was not handled.
    return FALSE;
  }

  void Win32Environment::create() {
    // Clear memory.
    ZeroMemory(&mWindowClass, sizeof(WNDCLASSEXW));

    // Set properties of window class.
    mWindowClass.cbSize        = sizeof(WNDCLASSEXW);
    mWindowClass.style         = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
    mWindowClass.hInstance     = GetModuleHandle(nullptr);
    mWindowClass.lpfnWndProc   = &Win32Window::windowProcedure;
    mWindowClass.lpszClassName = Win32Window::kClassname.data();

    // Attempt to register window class.
    if (RegisterClassExW(&mWindowClass) == 0) {
      // Get error.
      std::wstring err(errorMessage(GetLastError()));

      // TODO: throw Hearth::Exception.
      // Convert wstring to regular string, construct from aggregate
      throw std::runtime_error({ err.begin(), err.end() });
    }

    // Get working directory.
          WCHAR path[512];
    const DWORD result = GetCurrentDirectoryW(512, path);

    // Check result.
    if (result == 0) {
      // Get error.
      std::wstring err(errorMessage(GetLastError()));

      // TODO: throw Hearth::Exception.
      // Convert wstring to regular string, construct from aggregate
      throw std::runtime_error({ err.begin(), err.end() });
    }

    // Success, put path into working directory member.
    mWorkingDirectory.insert(mWorkingDirectory.begin(), path, path + 512);

    // Set console control handler.
    if (SetConsoleCtrlHandler(&consoleHandler, TRUE) == 0) {
      // Get error.
      std::wstring err(errorMessage(GetLastError()));

      // TODO: throw Hearth::Exception.
      // Convert wstring to regular string, construct from aggregate
      throw std::runtime_error({ err.begin(), err.end() });
    }

    // All done!
  }

  void Win32Environment::destroy() noexcept {
    // Destroy all currently open windows.
    for (auto* wnd : mOpenWindows)
      Window::destroy(wnd);

    // Unregister.
    UnregisterClassW(mWindowClass.lpszClassName, mWindowClass.hInstance);
  }

  void Win32Environment::pollEvents() const noexcept {
    MSG msg;
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }

}
