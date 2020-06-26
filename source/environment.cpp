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
#include <hearth/config.hpp>
#if defined(HAPI_WINDOWS_OS)
#  include "win32/environment.hpp"
#endif

namespace HAPI_NAMESPACE_NAME {

  Environment::Environment(const CreateInfo& createInfo) noexcept
    : mCreateInfo(createInfo)
  { }

  Environment* Environment::instance() noexcept {
    // The environment we will be creating.
    static Environment* instance = nullptr;

    // Did we already create our instance?
    if (instance != nullptr)
      return instance;

    // Try and create instance.
    try {
    #if defined(HAPI_WINDOWS_OS)
      instance = new native::Win32Environment();
    #endif
    } catch (const std::runtime_error& err) {
      // TODO: report exception properly.
      std::cerr << err.what() << std::endl;
      instance = nullptr;
    }

    // Return result.
    return instance;
  }

  Environment::Platform Environment::platform() const noexcept {
    return mCreateInfo.platform;
  }

  std::wstring_view Environment::workingDirectory() const noexcept {
    return mWorkingDirectory;
  }

}
