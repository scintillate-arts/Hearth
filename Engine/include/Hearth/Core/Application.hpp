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
 * \file    Application.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_CORE_APPLICATION_HPP__
#define __HEARTH_CORE_APPLICATION_HPP__ 1
#include <string_view>
#include "Config.hpp"
#include "Event.hpp"
#include "Environment.hpp"
#include "Version.hpp"

namespace Hearth {

  /**
   * \brief    A base implementation of an application ran and managed by Hearth.
   * \details ...
   */
  class Application {
  public:
    /**
     * \brief   The necessary information to create a new application.
     * \details ...
     */
    struct CreateInfo final {
      /**
       * \brief   The name of the application to create.
       * \details ...
       */
      std::string_view appName;

      /**
       * \brief   The name of the company or user that created the application.
       * \details ...
       */
      std::string_view companyName;

      /**
       * \brief   The version of the application to create.
       * \details ...
       */
      Version appVersion;
    };

  public:
    /**
     * \brief     Creates a new application instance from the given create information.
     * \details   ...
     * \param[in] pAppCreateInfo A pointer to the information needed to create an application.
     */
    explicit Application(const CreateInfo* pAppCreateInfo);

    /**
     * \brief   Default destructor.
     * \details ...
     */
    virtual ~Application() noexcept;

  public:
    /**
     * \brief   Executes the main loop of the application.
     * \details ...
     */
    void run() noexcept;

    /**
     * \copydoc IApplication::quit(bool)
     */
    void quit(bool shouldQuit) noexcept;

    /**
     * \copydoc IApplication::isQuitting()
     */
    [[nodiscard]]
    bool isQuitting() const;

  protected:
    /**
     * \brief   Called when the application is initializing.
     * \details Designed to be implemented by extending classes to perform extra initialization tasks.
     */
    virtual void onInitialize() {}

    /**
     * \brief   Called when the application is terminating.
     * \details Designed to be implemented by extending classes to perform extra termination tasks.
     */
    virtual void onTerminate() {}

    /**
     * \brief     Called when the application receives a window close event.
     * \details   Designed to be implemented by extending classes to handle the event before other objects.
     * \param[in] wce The window close event.
     */
    virtual void onWindowClose([[maybe_unused]] WindowCloseEvent* wce) { }

    /**
     * \brief     Called when the application receives a window focus event.
     * \details   Designed to be implemented by extending classes to handle the event before other objects.
     * \param[in] wfe The window focus event.
     */
    virtual void onWindowFocus([[maybe_unused]] WindowFocusEvent* wfe) { }

    /**
     * \brief     Called when the application receives a window move event.
     * \details   Designed to be implemented by extending classes to handle the event before other objects.
     * \param[in] wme The window move event.
     */
    virtual void onWindowMove([[maybe_unused]] WindowMoveEvent* wme) { }

    /**
     * \brief     Called when the application receives a window show event.
     * \details   Designed to be implemented by extending classes to handle the event before other objects.
     * \param[in] wse The window show event.
     */
    virtual void onWindowShow([[maybe_unused]] WindowShowEvent* wse) { }

    /**
     * \brief     Called when the application receives a window size event.
     * \details   Designed to be implemented by extending classes to handle the event before other objects.
     * \param[in] wse The window size event.
     */
    virtual void onWindowSize([[maybe_unused]] WindowSizeEvent* wse) { }

  private:
    /**
     * \brief   Makes sure the application is initialized properly.
     * \details ...
     */
    void initialize();

    /**
     * \brief   Executes application frames.
     * \details Starts the application loop.
     */
    void execute() noexcept;

    /**
     * \brief   Performs a single application frame.
     * \details ...
     */
    void frame() noexcept;

    /**
     * \brief   Makes sure the application terminates properly.
     * \details ...
     */
    void terminate() noexcept;

    /**
     * \brief     Receives and reemits an event.
     * \details   ...
     * \param[in] event The event that was triggered.
     */
    void onEvent(IEvent* event);

  private:
    /**
     * \brief   A pointer to the environment singleton.
     * \details The application must use this to initialize the the platform dependent code.
     */
    std::shared_ptr<Environment> mEnvironment;

    /**
     * \brief   The name of this application.
     * \details ...
     */
    std::string_view mName;

    /**
     * \brief   The name of the company that created this application.
     * \details ...
     */
    std::string_view mCompanyName;

    /**
     * \brief   The version of this Application.
     * \details ...
     */
    Version mVersionNo;

    /**
     * \brief   The application's quitting state.
     * \details ...
     */
    bool mQuitting;
  };

}

#endif /* __HEARTH_CORE_APPLICATION_HPP__ */
