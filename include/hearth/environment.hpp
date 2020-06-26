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
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include "config.hpp"

namespace HAPI_NAMESPACE_NAME {

  // Forwarded.
  class Application;
  class Window;

  /*!
   * \brief Represents the environment, or even residency, of the operating system.
   *
   * This object helps applications know their execution space.
   */
  class Environment {
    // Applications need to be able to communicate with the Environment, especially for tracking.
    friend class Application;

    // Windows need to be able to communicate with the Environment, especially for tracking.
    friend class Window;

  public:
    //! \brief An enumeration of platforms that the environment could be built from.
    enum Platform : std::uint8_t { Windows, };

  protected:
    //! \brief The information needed to create an Environment.
    struct CreateInfo {
      //! \brief The platform the environment was built from.
      Platform platform;
    };

  protected:
    /*!
     * \brief     Explicitly defined constructor, constructs this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    Environment(const CreateInfo& createInfo) noexcept;

    //! \brief Default destructor.
    virtual ~Environment() = default;

  public:
    /*!
     * \brief  Gets the singleton instance of the environment.
     * \return The singleton instance of the determined environment at runtime.
     */
    static Environment* instance() noexcept;

  public:
    /*!
     * \brief  Gets the platform this environment was created from.
     * \return The platform this environment was given when it was constructed.
     */
    Platform platform() const noexcept;

    /*!
     * \brief  Gets the current working directory of the environment.
     * \return The directory the environment was spawned in.
     */
    std::wstring_view workingDirectory() const noexcept;

  protected:
    //! \brief Creates this environment from the operating system it is running on.
    virtual void create() = 0;

    //! \brief Returns this environment back to the operating system it was created from.
    virtual void destroy() noexcept = 0;

    //! \brief Tells the operating system to process events sent to the open windows.
    virtual void pollEvents() const noexcept = 0;

  protected:
    //! \brief Keeps track of running applications.
    inline static std::vector<Application*> mRunningApps;

    //! \brief Keeps track of open applications.
    inline static std::vector<Window*> mOpenWindows;

  protected:
    //! \brief The information this environment was created with.
    CreateInfo mCreateInfo;

    //! \brief The working directory for this environment.
    std::wstring mWorkingDirectory;
  };

}

