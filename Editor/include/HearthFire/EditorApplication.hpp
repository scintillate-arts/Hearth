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
 * \file    EditorApplication.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_FIRE_EDITOR_APPLICATION_HPP__
#define __HEARTH_FIRE_EDITOR_APPLICATION_HPP__ 1
#include <Hearth/Core/Application.hpp>
#include <Hearth/Core/Window.hpp>

namespace Hearth {
  /**
   * \brief		The information needed to create the editor application.
   * \details ...
   * */
  constexpr ApplicationCreateInfo kEditorCreateInfo {
    .appName     = "Hearth Fire",
    .companyName = "Simular Games, LLC.",
    .appVersion  = Version::v1_0_0
  };

  /**
   * \brief		Represents the Hearth Engine editor application.
   * \details ...
   */
  class EditorApplication : public Application {
  public:
    /**
     * \brief		Constructs a new editor application.
     * \details ...
     */
    EditorApplication();

  protected:
    /**
     * \copydoc Application::onInitialize()
     */
    void onInitialize() override;

    /**
     * \copydoc Application::onTerminate()
     */
    void onTerminate() noexcept override;

    /**
     * \copydoc Application::onWindowClose(WindowCloseEvent*)
     */
    void onWindowClose(WindowCloseEvent* wce) noexcept override;

  private:
    /**
     * \brief 	The application window.
     * \details ...
     */
    IWindow* mAppWindow;
  };

}

#endif /* __HEARTH_FIRE_EDITOR_APPLICATION_HPP__ */
