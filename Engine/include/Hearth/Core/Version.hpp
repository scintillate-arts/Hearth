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
 * \file    Version.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_CORE_VERSION_HPP__
#define __HEARTH_CORE_VERSION_HPP__ 1
#include <cstdint>
#include "Forward.hpp"

namespace Hearth::Core {

  constexpr std::uint16_t kMajorVersion = 1;
  constexpr std::uint16_t kMinorVersion = 0;
  constexpr std::uint16_t kPatchVersion = 0;

  class Version {
  public:
    constexpr
    Version() noexcept : major(), minor(), patch() { }

    constexpr
    Version(std::uint16_t major_, std::uint16_t minor_, std::uint16_t patch_) noexcept
      : major(major_), minor(minor_), patch(patch_)
    { }

    constexpr explicit
    Version(std::uint32_t ver) noexcept
      : major(ver >> 22), minor((ver >> 12) & 0x03FF), patch(ver & 0x0FFF)
    { }

    constexpr explicit
    operator std::uint32_t() const noexcept {
      return static_cast<std::uint32_t>((major << 22) | (minor << 12) | patch);
    }

  public:
    static const Version v1_0_0;
    static const Version current;

  public:
    std::uint16_t major : 10;
    std::uint16_t minor : 10;
    std::uint16_t patch : 12;
  };

  // Initializes the static constexpr versions.
  constexpr Version Version::v1_0_0  = Version(1, 0, 0);
  constexpr Version Version::current = Version(kMajorVersion, kMinorVersion, kPatchVersion);
  constexpr Version makeVersion(std::uint16_t major, std::uint16_t minor, std::uint16_t patch) noexcept {
    return Version{ major, minor, patch };
  }

}


#endif /* __HEARTH_VERSION_HPP__ */
