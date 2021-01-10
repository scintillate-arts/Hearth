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
 * \file
 * \brief
 * \details
 */
#include <Hearth/Core/Logger.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Hearth {

  void ConsoleLogger::initialize() {
  #if HEARTH_DEBUG
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_logger = spdlog::stdout_color_mt("HearthEngine");
    m_logger->set_level(spdlog::level::trace);
    m_logger->info("Console Logger Initialized");
  #endif
  }

  void ConsoleLogger::printTrace(std::string_view log) noexcept {
  #if HEARTH_DEBUG
    m_logger->trace(log);
  #endif
  }

  void ConsoleLogger::printDebug(std::string_view log) noexcept {
  #if HEARTH_DEBUG
    m_logger->debug(log);
  #endif
  }

  void ConsoleLogger::printInfo(std::string_view log) noexcept {
  #if HEARTH_DEBUG
    m_logger->info(log);
  #endif
  }

  void ConsoleLogger::printWarning(std::string_view log) noexcept {
  #if HEARTH_DEBUG
    m_logger->warn(log);
  #endif
  }

  void ConsoleLogger::printError(std::string_view log) noexcept {
  #if HEARTH_DEBUG
    m_logger->error(log);
  #endif
  }

  void ConsoleLogger::printCritical(std::string_view log) noexcept {
  #if HEARTH_DEBUG
    m_logger->critical(log);
  #endif
  }

  void ConsoleLogger::setSeverity(spdlog::level::level_enum level) noexcept {
  #if HEARTH_DEBUG
    m_logger->set_level(level);
  #endif
  }

  auto ConsoleLogger::getLoggerInstance() noexcept
    -> std::shared_ptr<spdlog::logger>&
  {
    return m_logger;
  }

}