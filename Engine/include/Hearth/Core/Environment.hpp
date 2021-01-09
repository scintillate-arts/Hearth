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
 * \file     Environment.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_CORE_ENVIRONMENT_HPP__
#define __HEARTH_CORE_ENVIRONMENT_HPP__ 1
#include <memory>
#include <set>
#include <stdexcept>
#include "../Config.hpp"

namespace Hearth {

  // Forward.
  class Application;
  struct IEvent;

  /**
   * \brief   Describes the types of platforms that are available.
   * \details This is used to check what platform an Environment is and be able to cast it to it's appropriate type.
   */
  enum struct Platform { Windows };

  /**
   * \brief    Responsible for enabling the environment the application runs in.
   * \details ...
   * */
  struct Environment {
    /**
     * \brief   Gets or creates the environment singleton.
     * \details The environment singleton will only be created if it hasn't already. Otherwise the singleton will be returned.
     * 					This function is implemented by it's appropriate platfrom.
     * \returns The environment singleton.
     */
    static std::shared_ptr<Environment> instance() noexcept;

  public:
    /**
     * \brief   Default destructor for environments.
     * \details Must be provided for polymorphic types.
     * */
    virtual ~Environment() noexcept = default;

    /**
     * \brief   Initializes the environment at the beginning of the application execution.
     * \details ...
     * \remarks Calling this function again will not reinitialize the environment. A environment implementor should
     * 					make caution to add in checks to prevent this.
     */
    virtual void initialize() noexcept = 0;

    /**
     * \brief   Terminates the environment at the end of application execution.
     * \details ...
     * \remarks Calling this function again will not reterminate the environment. A environment implementor should
     * 					make caution to add in checks to prevent this.
     */
    virtual void terminate() noexcept = 0;

    /**
     * \brief   Polls the native environment events.
     * \details ...
     */
    virtual void pollEvents() const noexcept = 0;

    /**
     * \brief   Indicates what platform the environment represents.
     * \details Use the value returned to cast the environment interface into it's appropriate version.
     * \returns The platform corresponding to the one the environment was created on.
     */
    [[nodiscard]]
    virtual Platform platform() const noexcept = 0;

  public:
    /**
     * \brief     Tracks the given running application.
     * \details   ...
     * \param[in] app A pointer to the app to be tracked.
     */
    void trackApplication(Application* app) noexcept {
      mRunningApps.insert(app);
    }

    /**
     * \brief     Stops tracking the given running application.
     * \details   ...
     * \param[in] app A pointer to the app to stop tracking.
     */
    void untrackApplciation(Application* app) noexcept {
      mRunningApps.erase(app);
    }

    /**
     * \brief   Gets the set of running applications tracked by the environment.
     * \details ...
     * \returns The running applications that are tracked by the environment.
     */
    [[nodiscard]]
    auto trackedApps() const noexcept
      -> const std::set<Application*>&
    {
      return mRunningApps;
    }

    /**
     * \brief   Checks that the environment was initialized.
     * \details If the environment is not initialized, no actions should be taken upon it and it should
     * 					be reported as an exception.
     * \returns Its initialization state.
     */
    [[nodiscard]]
    bool isInitialized() const noexcept { return mIsInitialized; }

  protected:
    /**
     * \brief   Tracks the running applications on the environment.
     * \details ...
     * \remarks This is only specific to Hearth!
     */
    std::set<Application*> mRunningApps;

    /**
     * \brief	  Whether or not the environment is initialized.
     * \details Applications and various other classes use this to assure that environment specific
     * 					actions can be taken.
     */
    bool mIsInitialized = false;
  };

}


#endif /* __HEARTH_CORE_ENVIRONMENT_HPP__ */
