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
 * \file    EventDispatcher.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_CORE_EVENT_DISPATCHER_HPP__
#define __HEARTH_CORE_EVENT_DISPATCHER_HPP__ 1
#include <cstdint>
#include <memory>
#include <vector>
#include <Hearth/Core/Event.hpp>

namespace Hearth::Core {

  /**
   * \brief		Represents an interface by which an event emitter can send an event to be handled.
   * \details ...
   */
  class EventHandler {
  protected:
    /**
     * \brief			Constructs an event handler with the given priority.
     * \details 	...
     * \param[in] priority The priority for the event handler.
     */
    explicit EventHandler(std::uint32_t priority) noexcept
      : mPriority{ priority }
    { }

  public:
    /**
     * \brief		Default destructor.
     * \details ...
     */
    virtual ~EventHandler() noexcept = default;

    /**
     * \brief			Called by an event emitter which can pass a given event to the handler.
     * \details ...
     * \param[in] event A pointer to the event to handle.
     */
    virtual void notify(Event* event) const noexcept = 0;

  public:
    /**
     * \brief 	Gets the priority of this event handler.
     * \details The priority of an event handler is used in making sure that event handlers are
     * 					sorted.
     * \returns The priority of this handler.
     */
    [[nodiscard]]
    std::uint32_t priority() const noexcept { return mPriority; }

  protected:
    /**
     * \brief 	The priority of the event handler.
     * \details ...
     */
    std::uint32_t mPriority;
  };

  /**
   * \brief 	A member function specific event handler.
   * \details ...
   */
  template<class T>
  class MemberFunctionEventHandler final : public EventHandler {
  public:
    /**
     * \brief 	The type of callback this event handler uses.
     * \details ...
     */
    using CallbackType = void (T::*)(Event*);

  public:
    /**
     * \brief		Constructs a new member function event handler from the given data.
     * \details ...
     * \param[in] callee The object to call the callback on.
     * \param[in] callback The function to execute on the callee.
     * \param[in] priority The priority of the event handler over other handlers.
     */
    explicit MemberFunctionEventHandler(T* callee, CallbackType callback, uint32_t priority) noexcept
      : EventHandler(priority)
      , mCallee(callee)
      , mCallback(callback)
    { }

  public:
    /**
     * \brief 		Compares this member function event handler to the other.
     * \details 	...
     * \param[in] other The object to compare to.
     * \returns 	True if all members are equal, otherwise false.
     */
    bool operator==(const MemberFunctionEventHandler& other) const noexcept {
      return mCallee == other.mCallee && mCallback == other.mCallback && mPriority == other.mPriority;
    }

    /**
     * \brief 		Executes the callback on the callee.
     * \details 	...
     * \param[in] event The event to be handled.
     */
    void notify(Event* event) const noexcept final {
      (mCallee->*mCallback)(event);
    }

  private:
    /**
     * \brief 	A pointer to the object to call the callback on.
     * \details ...
     */
    T* mCallee;

    /**
     * \brief 	The callback to execute during notification.
     * \details ...
     */
    CallbackType mCallback;
  };

  /**
   * \brief 	Handles the dispatching of events to handlers.
   * \details ...
   */
  class EventDispatcher final {
    /**
     * \brief 	A type definition for event handler instances.
     * \details ...
     */
    using EventHandler = std::unique_ptr<EventHandler>;

    /**
     * \brief 	A type definition for event instance.
     * \details ...
     */
    using Event = std::unique_ptr<Event>;

    /**
     * \brief 	A type definition for a member function callback.
     * \details ...
     * \tparam  T The type of the class object the member function is coming from.
     * */
    template<class T>
    using MemberFunctionCallback = typename MemberFunctionEventHandler<T>::CallbackType;

    /**
     * \brief 	A functor class designed to compare the priorities of event handlers and sort them.
     * \details ...
     */
    struct EventHandlerPrioritySorter final {
      /*!
       * \brief     Returns which handler has high priority.
       * \details 	...
       * \param[in] lhs The left hand operand.
       * \param[in] rhs The right hand operand.
       * \return    The handler with the higher priority.
       */
      bool operator()(const EventHandler& lhs, const EventHandler& rhs) const noexcept {
        return lhs->priority() < rhs->priority();
      }
    };

  public:
    /**
     * \brief 	Handles all the queued events.
     * \details ...
     */
    static void flush() noexcept {
      // Emit each event.
      for (auto& event : mEventQueue)
        emit(event);
      // Clear events.
      mEventQueue.clear();
    }

    /**
     * \brief			Emits the passed event to all handlers.
     * \details 	...
     * \param[in] event The event to emit.
     */
    static void emit(const Event& event) noexcept {
      // Emit to each handler.
      for (auto& handler : mHandlers) {
        // Make sure the event isn't handled.
        if (!event->isConsumed())
          handler->notify(event.get());
      }
    }

    /**
     * \brief 		Pushes a new event into the event queue.
     * \details 	...
     * \param[in] event The new event to push.
     */
    static void pushEvent(Event&& event) noexcept {
      mEventQueue.emplace_back(std::move(event));
    }

    /**
     * \brief 		Registers a member function event handler with the event dispatcher.
     * \details 	...
     * \tparam    T The type of the object with the member function.
     * \param[in] callee The object that will have its callback executed.
     * \param[in] callback The function to execute on the callee.
     * \param[in] priority The priority of the event handler.
     */
    template<class T>
    static void registerHandler(T* callee, MemberFunctionCallback<T> callback, std::uint32_t priority) noexcept {
      mHandlers.emplace_back(std::make_unique<MemberFunctionEventHandler<T>>(callee, callback, priority));
      std::sort(mHandlers.begin(), mHandlers.end(), EventHandlerPrioritySorter());
    }

    /**
     * \brief 		Unregisters a member fuunction event handler with the event dispatcher.
     * \details 	...
     * \tparam    T The type of the object with the member function.
     * \param[in] callee The object that will have its callback executed.
     * \param[in] callback The function to execute on the callee.
     * \param[in] priority The priority of the event handler.
     */
    template<class T>
    static void unregisterHandler(T* callee, MemberFunctionCallback<T> callback, std::uint32_t priority) noexcept {
      MemberFunctionEventHandler<T> toRemove(callee, callback, priority);
      for (auto itr = mHandlers.begin(); itr != mHandlers.end(); ) {
        // Lets make this really simple, we're just casting the EventHandler* to
        // MemberFunctionEventHandler* and then dereferencing that so we can compare it
        // to 'toRemove'.
        auto val = *dynamic_cast<MemberFunctionEventHandler<T>*>(itr->get());
        if (val == toRemove)
          itr = mHandlers.erase(itr);
        else
          ++itr;
      }
    }

  private:
    /**
     * \brief 	A list of registered event handlers.
     * \details ...
     */
    inline static std::vector<EventHandler> mHandlers;

    /**
     * \brief 	A queue for events to be pushed into for deferred handling.
     * \details To handle all the events, the queue should be flushed.
     */
    inline static std::vector<Event> mEventQueue;
  };

}

#endif /* __HEARTH_CORE_EVENT_DISPATCHER_HPP__ */
