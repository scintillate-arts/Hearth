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
#include <cstdint>
#include "config.hpp"

namespace HAPI_NAMESPACE_NAME {
  // Hearth semantic version.
  constexpr std::uint16_t kMajorVersion = 1;
  constexpr std::uint16_t kMinorVersion = 0;
  constexpr std::uint16_t kPatchVersion = 0;

  /*!
   * \brief Represents a semantic version object.
   *
   * The "major" semantic version represents significant changes to a given application or library.
   * These changes are usually not backwards compatible. The "minor" semantic version represents
   * changes that are made on an application or library which are still usually backwards compatible
   * and/or new functionality. The "patch" semantic version represents, usually, bug fixes.
   */
  class Version {
  public:
    //! \brief Default constructor.
    constexpr Version() noexcept : major(), minor(), patch() { }

    /*!
     * \brief     Constructs a new semantic version with the given values.
     * \param[in] major The major semantic version.
     * \param[in] minor The minor semantic version.
     * \param[in] patch The patch semantic version.
     */
    constexpr Version(std::uint16_t major, std::uint16_t minor, std::uint16_t patch) noexcept
      : major(major), minor(minor), patch(patch)
    { }

    /*!
     * \brief     Constructs a new semantic version from the given integer.
     * \param[in] ver The integer value to convert to a version object.
     */
    constexpr Version(std::uint32_t ver) noexcept
      : major(ver >> 22), minor((ver >> 12) & 0x03FF), patch(ver & 0x0FFF)
    { }

  public:
    /*!
     * \brief  Implicitly converts this version to an integer embedded with the version info.
     * \return A single integer with the embedded version.
     */
    constexpr operator std::uint32_t() const noexcept {
      return ((major << 22) | (minor << 12) | patch);
    }

  public:
    //! \brief The first major version of any given project, but specifically Hearth.
    static const Version v1_0_0;

    //! \brief The current version of Hearth.
    static const Version current;

  public:
    //! \brief The major semantic version number.
    std::uint16_t major : 10;

    //! \brief The minor semantic version number.
    std::uint16_t minor : 10;

    //! \brief The patch semantic version number.
    std::uint16_t patch : 12;
  };

  // Initializes the static constexpr versions.
  constexpr Version Version::v1_0_0  = Version(1, 0, 0);
  constexpr Version Version::current = Version(kMajorVersion, kMinorVersion, kPatchVersion);

  /*!
   * \brief     Creates a version from the given semantic parts.
   * \param[in] major The major semantic version.
   * \param[in] minor The minor semantic version.
   * \param[in] patch The patch semantic version.
   * \return    The resulting version.
   */
  constexpr Version makeVersion(std::uint16_t major, std::uint16_t minor, std::uint16_t patch) noexcept {
    return Version{ major, minor, patch };
  }
}
