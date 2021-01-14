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
 * \file    Event.hpp
 * \brief   This file contains the definition for all the event types and their members.
 */
#ifndef __INC_HEARTH_CORE_EVENT_HPP__
#define __INC_HEARTH_CORE_EVENT_HPP__ 1
#include <glm/glm.hpp>
#include "Forward.hpp"

namespace Hearth::Core {
/**
 * \enum  EventType
 * \brief Represents the different types of events there are.
 */
/**
 * \fn      Event::~Event()
 * \brief   Default destructor.
 * \details This destructor is virtual in case extending classes need it.
 */
/**
 * \fn      Event::consume()
 * \brief   Consumes (handles) an event.
 * \remarks Consumed events are no longer propagated.
 */
/**
 * \fn      Event::type()
 * \brief   Gets the type of this event.
 * \returns The type of this event, set by subclasses.
 */
/**
 * \fn      Event::isConsumed()
 * \brief   Checks if the event was consumed.
 * \details Consumed events should no longer be propagated, thus a dispatcher should check
 * 					if the event was consumed before sending it again.
 * \returns True if the event was consumed, false otherwise.
 */

  enum struct EventType {
    Undefined,
    WindowClose,
    WindowFocus,
    WindowMove,
    WindowShow,
    WindowSize,
  };

  class Event {
  public:
    virtual ~Event() noexcept = default;

  public:
    void consume() noexcept { m_consumed = true; }
    EventType type() const noexcept { return m_type; }
    bool isConsumed() const noexcept { return m_consumed; }

  protected:
    EventType m_type     = EventType::Undefined;
    bool      m_consumed = false;
  };

}

#endif /* __INC_HEARTH_CORE_EVENT_HPP__ */
