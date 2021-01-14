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
#include <Hearth/Core/Application.hpp>
#include <Hearth/Core/Environment.hpp>
#include <Hearth/Core/Logger.hpp>
#include "EventDispatcher.hpp"

namespace Hearth::Core {

  Application::Application(const Application::CreateInfo *pAppCreateInfo)
    : m_environment{ Environment::instanceRef() }
    , m_name(pAppCreateInfo->appName)
    , m_companyName(pAppCreateInfo->companyName)
    , m_versionNo(pAppCreateInfo->appVersion)
    , m_quitting(false)
  {
    EventDispatcher::registerHandler(this, &Application::onEvent, 0);
  }

  Application::~Application() noexcept {
    EventDispatcher::unregisterHandler(this, &Application::onEvent, 0);
  }

  void Application::run() noexcept {
    // Try and initialize.
    try {
      initialize();
    } catch (const std::runtime_error& err) {
      std::cerr << err.what() << std::endl;
      return;
    }

    // Proceed with execution.
    execute();
    terminate();
  }

  void Application::quit(bool shouldQuit) noexcept {
    m_quitting = shouldQuit;
  }

  bool Application::isQuitting() const {
    return m_quitting;
  }

  void Application::initialize() {
    /* TODO:
     *    Move the console logger initialization into main Hearth initialization.
     */
    ConsoleLogger::initialize();
    m_environment->initialize();
    onInitialize();
    HEARTH_LOGGER_INFO("{0} Application initialized", m_name);
  }

  void Application::execute() noexcept {
    while (!m_quitting) frame();
  }

  void Application::frame() noexcept {
    // Track frame start time.
    // Track frame delta.

    m_environment->pollEvents();
    EventDispatcher::flush();

    // Execute simulations.
    // Execute updates.
    // Execute renders.
    // Track frame end time.
  }

  void Application::terminate() noexcept {
    onTerminate();
    HEARTH_LOGGER_INFO("{0} Application terminated", m_name);
  }

  void Application::onEvent(Event* evt) {
    // Dispatch events to sub classes.
    switch (evt->type()) {
    case EventType::WindowClose:
      // Call to implementation first.
      HEARTH_LOGGER_TRACE("{0} Application Received Window close event", m_name);
      onWindowClose(dynamic_cast<Window::CloseEvent*>(evt));
      break;
    case EventType::WindowFocus:
      HEARTH_LOGGER_TRACE("{0} Application Received Window focus event", m_name);
      onWindowFocus(dynamic_cast<Window::FocusEvent*>(evt));
      break;
    case EventType::WindowMove:
      HEARTH_LOGGER_TRACE("{0} Application Received Window move event", m_name);
      onWindowMove(dynamic_cast<Window::MoveEvent*>(evt));
      break;
    case EventType::WindowShow:
      HEARTH_LOGGER_TRACE("{0} Application Received Window show event", m_name);
      onWindowShow(dynamic_cast<Window::ShowEvent*>(evt));
      break;
    case EventType::WindowSize:
      HEARTH_LOGGER_TRACE("{0} Application Received Window size event", m_name);
      onWindowSize(dynamic_cast<Window::SizeEvent*>(evt));
      break;
    default:
      break;
    }
  }

}
