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
 * \file  Environment.hpp
 * \brief This file contains the definition for the \c Environment and its members.
 */
/* TODO:
 *    Convert Environment to be constructed and used in similar way to Monitor.
 */
#ifndef __INC_HEARTH_CORE_ENVIRONMENT_HPP__
#define __INC_HEARTH_CORE_ENVIRONMENT_HPP__ 1
#include <memory>
#include <set>
#include <stdexcept>
#include "../Config.hpp"
#include "Forward.hpp"

namespace Hearth::Core {
/**
 * \enum    Platform
 * \brief   Describes the types of platforms that are available.
 * \details This is used to check what platform an Environment is and be able to cast it to it's appropriate type.
 */
/**
 * \fn      Environment::instance()
 * \brief   Gets or creates the environment singleton.
 * \details The environment singleton will only be created if it hasn't already. Otherwise the singleton will be
 *          returned. This function is implemented by it's appropriate platfrom.
 * \returns The environment singleton.
 */
/**
 * \fn      Environment::~Environment()
 * \brief   Default destructor for environments.
 * \details Must be provided for polymorphic types.
 */
/**
 * \fn        Environment::trackApplication(ApplicationView app)
 * \brief     Tracks the given running application.
 * \param[in] app A view of the application to track.
 */
/**
 * \fn        Environment::untrackApplication(ApplicationView app)
 * \brief     Stops tracking the given running application.
 * \param[in] app A view of the application to track.
 */
/**
 * \fn      Environment::trackedApplications()
 * \brief   Gets the set of running applications tracked by the environment.
 * \returns The running application views that are tracked by the environment.
 */
/**
 * \fn      Environment::isInitialized()
 * \brief   Checks that the environment was initialized.
 * \details If the environment is not initialized, no actions should be taken upon it and it should
 * 					be reported as an exception.
 * \returns The initialization state of the \c Environment.
 */
/**
 * \fn      Environment::initialize()
 * \brief   Initializes the environment at the beginning of the application execution.
 * \remarks Calling this function again will not reinitialize the environment. A environment implementor should
 * 					make caution to add in checks to prevent this.
 */
/**
 * \fn      Environment::termiante()
 * \brief   Terminates the environment at the end of application execution.
 * \remarks Calling this function again will not reterminate the environment. A environment implementor should
 * 					make caution to add in checks to prevent this.
 */
/**
 * \fn    Environment::pollEvents()
 * \brief Polls the native environment events.
 */
/**
 * \fn      Environment::platform()
 * \brief   Indicates what platform the environment represents.
 * \details Use the value returned to cast the environment interface into it's appropriate version.
 * \returns The platform corresponding to the one the environment was created on.
 */

  enum struct Platform {
    Windows
  };

  class Environment {
  public: // Constructors and Destructor
    virtual ~Environment() noexcept = default;

  public: // Member functions
    void trackApplication(ApplicationRef app) noexcept;
    void untrackApplication(ApplicationRef app) noexcept;
    const std::set<ApplicationRef>& trackedApps() const noexcept;
    bool isInitialized() const noexcept;

  public: // Virtuals member functions
    virtual void initialize() noexcept = 0;
    virtual void terminate() noexcept = 0;
    virtual void pollEvents() const noexcept = 0;
    virtual Platform platform() const noexcept = 0;

  public: // Static functions
    static EnvironmentView instanceView() noexcept;
    static EnvironmentRef  instanceRef() noexcept;

  protected:
    std::set<ApplicationRef> m_runningApps;
    bool m_initialized = false;
  };

}


#endif /* __INC_HEARTH_CORE_ENVIRONMENT_HPP__ */
