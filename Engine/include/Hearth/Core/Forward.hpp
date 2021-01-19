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
 * \file    Forward.hpp
 * \brief   A forwarding declaration file for various types and type definitions necessary for the \c Hearth::Core
 *          namespace.
 * \details This file contains declarations for all the classes within the \c Hearth::Core namespace. It also contains
 *          useful type definitions for those core types, such as each one's \c Memory::View type.
 */
#include <string>
#include <string_view>
#include <htl/Memory.hpp>

namespace Hearth::Core {
/**
 * \class   Application
 * \brief   Base form of an application that can be ran and managed by HearthEngine.
 * \details An application is an abstraction of an Operating System program, specifically one that an end user can
 *          interact with. We call these, typically, GUI applications. But we've taken the liberty to remove the GUI
 *          aspect of the application and leave that up to the developers. Perhaps the developers do not need the
 *          application GUI, but still need the application's primary loop.
 */
/**
 * \class   ConsoleLogger
 * \brief   A debugging logger specifically for printing to the console.
 * \details HearthEngine is a big software library and has many parts, it was a good idea to delegate the task of
 *          printing debugging reports to the console to its own class. We likewise have a \c FileLogger for file
 *          based logging. An implementing application which wants to display GUI debugging reports should use one of
 *          these two loggers, obtain their backlog and clear them afterwards.
 */
/**
 * \class   Environment
 * \brief   An environment is a representation of the operating system.
 * \details When the engine is in its initialization phase, it performs some subsystem initialization. The environment
 *          is one of those subsystems. The environment represents the operating system and is responsible for
 *          initializing some operating system specific information, and obtaining HearthEngine specific information
 *          from the operating system. Some information required by HearthEngine, from the operating system, are: the
 *          current working directory, ...
 * \todo    List other things that HearthEngine requires from the operating system on initialize.
 */
/**
 * \class   Event
 * \brief   An event describing something that took place within the engine.
 * \details HearthEngine is an event based system. It tries to do as little work as possible until it needs to. To do
 *          this, HearthEngine sends events and lets an event dispatcher notify registered listeners. Many of the
 *          events present in HearthEngine are related to the Window and user input. But Hearth does have a number of
 *          non-GUI events that are internal and hidden away, such as the update event and render event.
 */
/**
 * \class   FileLogger
 * \brief   A debugging logger specifically for writing to files.
 * \details HearthEngine is a big software library with many parts, it was a good idea to delegate the task of logging
 *          debugging reports to a file to its own class. We likewise have a \c ConsoleLogger for console based
 *          logging. An implementing application which wants to display GUI debugging reports should use one of these
 *          two loggers, obtain their backlog and clear them afterwards.
 */
/**
 * \class   Monitor
 * \brief   Represents a monitor peripheral on the end user's machine.
 * \details HearthEngine typically works with GUI applications, and one aspect of GUI applications is knowing where
 *          and how to render them. We delegate some information about this to the \c Monitor class. The Monitor class
 *          ends up being an abstraction over the operating system representation of a monitor attached to the end
 *          user computer. The \c Monitor is able to provide information about its resolution, its gamma ramp, and
 *          its video mode to developers. All of these are necessary for rendering in their own respects.
 */
/**
 * \class   Window
 * \brief   Represents the main GUI frame that the user will interact with.
 * \details HearthEngine typically works with GUI applications. Because of this, we need some way of presenting the
 *          GUI and receiving the user input. That's where the \c Window class comes in. The \c Window class becomes
 *          the primary resource for rendering and handling user input. The \c Window is used in the creation of a
 *          \c Graphics::RenderSurface and is responsible for providing all of the primary top level events such as
 *          keyboard events and mouse events.
 */
/**
 * \class   Version
 * \brief   Represents a semantic version object.
 * \details The "major" semantic version represents significant changes to a given application or
 * 					library. These changes are usually not backwards compatible. The "minor" semantic
 * 					version represents changes that are made on an application or library which are still
 * 					usually backwards compatible and/or new functionality. The "patch" semantic version
 * 					represents, usually, bug fixes.
 */
/**
 * \typedef String
 * \brief   A type representing a string.
 * \remarks This is just a type definition of the already defined \c std::string, to make our lives easier.
 */
/**
 * \typedef WideString
 * \brief   A type representing a wide string.
 * \remarks This is just a type definition of the already defined \c std::wstring, to make our lives easier.
 */
/**
 * \typedef ApplicationView
 * \brief   A type representing the view of an application class object.
 */
/**
 * \typedef ConsoleLoggerView
 * \brief   A type representing the view of a console logger object.
 */
/**
 * \typedef EnvironmentView
 * \brief A type representing the view of an environment object.
 */
/**
 * \typedef EventView
 * \brief   A type representing the view of an event object.
 */
/**
 * \typedef FileLoggerView
 * \brief   A type representing the view of a file logger object.
 */
/**
 * \typedef MonitorView
 * \brief   A type representing the view of a monitor object.
 */
/**
 * \typedef WindowView
 * \brief   A type representing the view of a window object.
 */
/**
 * \typedef StringView
 * \brief   A type representing the view of a string.
 * \remarks This is just a type definition of the already defined \c std::string_view, to make our lives easier.
 */
/**
 * \typedef WideStringView
 * \brief   A type representing the view of a wide string.
 * \remarks This is just a type definition of the already defined \c std::wstring_view, to make our lives easier.
 */

  class Application;
  class ConsoleLogger;
  class Environment;
  class Event;
  class FileLogger;
  class Monitor;
  class Window;
  class Version;

  using String            = std::string;
  using WideString        = std::wstring;
  using ApplicationRef    = htl::Reference<Application>;
  using ConsoleLoggerRef  = htl::Reference<ConsoleLogger>;
  using EnvironmentRef    = htl::Reference<Environment>;
  using EventRef          = htl::Reference<Event>;
  using FileLoggerRef     = htl::Reference<FileLogger>;
  using MonitorRef        = htl::Reference<Monitor>;
  using WindowRef         = htl::Reference<Window>;
  using ApplicationView   = htl::View<Application>;
  using ConsoleLoggerView = htl::View<ConsoleLogger>;
  using EnvironmentView   = htl::View<Environment>;
  using EventView         = htl::View<Event>;
  using FileLoggerView    = htl::View<FileLogger>;
  using MonitorView       = htl::View<Monitor>;
  using WindowView        = htl::View<Window>;
  using StringView        = std::string_view;
  using WideStringView    = std::wstring_view;

}
