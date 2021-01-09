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
 * \file    Application.cpp
 * \brief   ...
 * \details ...
 */
#include <iostream>
#include <Hearth/Core/Application.hpp>
#include "EventDispatcher.hpp"

namespace Hearth {

  Application::Application(const Application::CreateInfo *pAppCreateInfo)
    : mEnvironment{ Environment::instance() }
    , mName(pAppCreateInfo->appName)
    , mCompanyName(pAppCreateInfo->companyName)
    , mVersionNo(pAppCreateInfo->appVersion)
    , mQuitting(false)
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
    mQuitting = shouldQuit;
  }

  [[nodiscard]]
  bool Application::isQuitting() const {
    return mQuitting;
  }

  void Application::initialize() {
    mEnvironment->initialize();
    mEnvironment->trackApplication(this);
    onInitialize(); // Main window should be implemented by extending classes.
  }

  void Application::execute() noexcept {
    while (!mQuitting) frame();
  }

  void Application::frame() noexcept {
    // Track frame start time.
    // Track frame delta.

    mEnvironment->pollEvents();
    EventDispatcher::flush();

    // Execute simulations.
    // Execute updates.
    // Execute renders.
    // Track frame end time.
  }

  void Application::terminate() noexcept {
    onTerminate();
    mEnvironment->untrackApplciation(this);
    mEnvironment->terminate();
  }

  void Application::onEvent(IEvent *event) {
    // Dispatch events to sub classes.
    switch (event->type()) {
    case EventType::WindowClose:
      // Call to implementation first.
      onWindowClose(dynamic_cast<WindowCloseEvent*>(event));
      break;
    case EventType::WindowFocus:
      onWindowFocus(dynamic_cast<WindowFocusEvent*>(event));
      break;
    case EventType::WindowMove:
      onWindowMove(dynamic_cast<WindowMoveEvent*>(event));
      break;
    case EventType::WindowShow:
      onWindowShow(dynamic_cast<WindowShowEvent*>(event));
      break;
    case EventType::WindowSize:
      onWindowSize(dynamic_cast<WindowSizeEvent*>(event));
      break;
    default:
      break;
    }
  }

}
