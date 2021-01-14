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
 * \file  Application.hpp
 * \brief This file contains the definition for the \c Application class and its members.
 */
/* TODO:
 *    Move Application::execute() and Application::frame() to an EngineExecutor private class.
 */
#ifndef __INC_HEARTH_CORE_APPLICATION_HPP__
#define __INC_HEARTH_CORE_APPLICATION_HPP__ 1
#include <string_view>
#include "../Config.hpp"
#include "Forward.hpp"
#include "Window.hpp"
#include "Version.hpp"

namespace Hearth::Core {
/**
 * \fn        Application::Application(const Application::CreateInfo*)
 * \brief     Creates a new application instance from the given create information.
 * \param[in] createInfo The create information for the application.
 */
/**
 * \fn    Application::~Application()
 * \brief Destructs an \c Application instance appropriately.
 */
/**
 * \fn        Application::Application(const Application&)
 * \brief     Copy constructs an \c Application with the information of another application.
 * \param[in] otherApp The \c Application to copy data from.
 */
/**
 * \fn        Application::Application(Application&&)
 * \brief     Creates a new \c Application from the information of the other application by moving its data.
 * \param[in] otherApp The \c Application to move data from.
 */
/**
 * \fn        Application::operator=(const Application&)
 * \brief     Copies the data of one \c Application into an already existing application.
 * \param[in] otherApp The \c Application to copy data from.
 * \returns   The \c Application with the copied data.
 */
/**
 * \fn        Application::operator=(Application&&)
 * \brief     Moves data of one \c Application into an already existing application.
 * \param[in] otherApp The \c Application to move data from.
 * \returns   The \c Application with the moved data.
 */
/**
 * \fn        Application::operator==(const Application&)
 * \brief     Equality comparison of an \c Application to another.
 * \param[in] otherApp The \c Application to compare to.
 * \returns   True if the two applications are equal.
 * \returns   False if the two applications are *not* equal.
 */
/**
 * \fn        Application::operator!=(const Application&)
 * \brief     Equality comparison of an \c Application to another.
 * \param[in] otherApp The \c Application to compare to.
 * \returns   True if the two applications are *not* equal.
 * \returns   False if the two applications are equal.
 */
/**
 * \fn      Application::run()
 * \brief   Executes the main loop of the \c Application.
 * \details The \c Application::run() function is responsible for starting, executing, and terminating the main loop of
 *          the entire application. Actual frame execution and and event dispatching do not occur within the application
 *          but within another class that the application depends on.
 */
/**
 * \fn        Application::quit(bool)
 * \brief     Sets the quitting state of the \c Application.
 * \details   When the \c Application is set to quit, on the next frame the application will perform all its necessary
 *            shutdown operations. Another system can request the application doesn't shutdown, by handling the shutdown
 *            event. The only time the application doesn't send a shutdown request event is if the application is
 *            shutdown from outside the application, for example, by CTRL+C on the console or the task being shutdown
 *            via the task manager.
 * \param[in] shouldQuit Whether or not the application should quit.
 */
/**
 * \fn      Application::isQuitting()
 * \brief   Checks if the \c Application is quitting.
 * \details The qutting state of the application is not thread locked, it can be set anytime and checked anytime. This
 *          is because we use events to notify observers that the quitting state of the application has changed, thus
 *          synchronizing the application's state across all threads if observers exist on other threads.
 * \returns The quitting state of the application.
 */
/**
 * \fn      Application::onInitialize()
 * \brief   Called when the application is initializing.
 * \details Designed to be implemented by extending classes to perform extra initialization tasks.
 */
/**
 * \fn      Application::onTerminate()
 * \brief   Called when the application is terminating.
 * \details Designed to be implemented by extending classes to perform extra termination tasks.
 */
/**
 * \brief     Called when the application receives a window close event.
 * \details   Designed to be implemented by extending classes to handle the event before other objects.
 * \param[in] evt The window close event.
 */
/**
 * \brief     Called when the application receives a window focus event.
 * \details   Designed to be implemented by extending classes to handle the event before other objects.
 * \param[in] evt The window focus event.
 */
/**
 * \brief     Called when the application receives a window move event.
 * \details   Designed to be implemented by extending classes to handle the event before other objects.
 * \param[in] evt The window move event.
 */
/**
 * \brief     Called when the application receives a window show event.
 * \details   Designed to be implemented by extending classes to handle the event before other objects.
 * \param[in] evt The window show event.
 */
/**
 * \brief     Called when the application receives a window size event.
 * \details   Designed to be implemented by extending classes to handle the event before other objects.
 * \param[in] evt The window size event.
 */
/**
 * \struct  Application::CreateInfo
 * \brief   The necessary information to create a new application.
 */
/**
 * \property Application::CreateInfo::appName
 * \brief    The name of the application to create.
 */
/**
 * \property Application::CreateInfo::companyName
 * \brief    The name of the company that created the application.
 * \details  We prefer to track the name of the company that created the application as a means to tell users who to
 *           contact when the application crashes, if at all possible.
 */
/**
 * \property Application::CreateInfo::appVersion
 * \brief    The current version of the application as set by the company who created it.
 * \details  We prefer to track the version of the application as a means to tell users what application version they
 *           are running when the application crashes, if at all possible. We also need this for Vulkan, as vulkan wants
 *           to know similar information.
 */

  class Application {
  public: // Types
    struct CreateInfo;

  public: // Constructors, Destructor, and operators
    explicit Application(const CreateInfo* pAppCreateInfo);
    virtual ~Application() noexcept;
    Application(const Application& otherApp) = default;
    Application(Application&& otherApp) noexcept = default;
    Application& operator=(const Application& otherApp) = default;
    Application& operator=(Application&& otherApp) noexcept = default;
    bool operator==(const Application& otherApp) const noexcept = default;
    bool operator!=(const Application& otherApp) const noexcept = default;

  public: // Member functions
    void run() noexcept;
    void quit(bool shouldQuit) noexcept;
    bool isQuitting() const;

  protected: // Virtual member functions
    virtual void onInitialize() {}
    virtual void onTerminate() {}
    virtual void onWindowClose([[maybe_unused]] Window::CloseEvent* evt) { }
    virtual void onWindowFocus([[maybe_unused]] Window::FocusEvent* evt) { }
    virtual void onWindowMove([[maybe_unused]] Window::MoveEvent* evt) { }
    virtual void onWindowShow([[maybe_unused]] Window::ShowEvent* evt) { }
    virtual void onWindowSize([[maybe_unused]] Window::SizeEvent* evt) { }

  private:
    void initialize();
    void terminate() noexcept;
    void execute() noexcept;
    void frame() noexcept;
    void onEvent(Event* evt);

  private:
    EnvironmentRef m_environment;
    StringView     m_name;
    StringView     m_companyName;
    Version        m_versionNo;
    mutable bool   m_quitting;
  };

  struct Application::CreateInfo final {
    StringView appName;
    StringView companyName;
    Version    appVersion;
  };

}

#endif /* __INC_HEARTH_CORE_APPLICATION_HPP__ */
