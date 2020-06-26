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
#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <hearth/event.hpp>

namespace HAPI_NAMESPACE_NAME {

  //! \brief Responsible for handling emitted events.
  class EventHandler {
  public:
    /*!
     * \brief     Constructs this event handler from the given priority.
     * \param[in] priority The priority of this event handler.
     */
    explicit EventHandler(std::uint32_t priority) noexcept
      : mPriority(priority)
    {}

  public:
    /*!
     * \brief  Gets the priority of this event handler.
     * \return The priority of this event handler.
     */
    std::uint32_t priority() const noexcept {
      return mPriority;
    }

  public:
    /*!
     * \brief     Notifies this event handler of the emitted event.
     * \param[in] evnt The event that was emitted.
     */
    virtual void notify(Event& evnt) const noexcept = 0;

  private:
    //! \brief The priority of this handler.
    std::uint32_t mPriority = 0;
  };

  //! \brief A member function specific event handler.
  template<typename Type>
  class MemberEventHandler final : public EventHandler {
    //! \brief The type of the callback this event handler uses.
    using Callback = void(Type::*)(Event&);

  public:
    /*!
     * \brief     Constructs this event handler with the member information.
     * \param[in] callee A pointer to the class type that will handle events.
     * \param[in] callback A function of the class type that will handle events.
     * \param[in] priority The priority of this event handler over the others.
     */
    MemberEventHandler(Type* callee, Callback callback, uint32_t priority) noexcept
      : EventHandler(priority)
      , mCallee(callee)
      , mCallback(callback)
    {}

  public:
    //! \copydoc EventHandler::notify()
    void notify(Event& evnt) const noexcept final {
      (mCallee->*mCallback)(evnt);
    }

  private:
    //! \brief The callee, whose callback will be executed.
    Type* mCallee;

    //! \brief The callback that will be executed.
    Callback mCallback;
  };

  //! \brief Sorts event handlers by their priority.
    struct EventHandlerPrioritySorter final {
      /*!
       * \brief     Returns which handler has high priority.
       * \param[in] lhs The left hand operand.
       * \param[in] rhs The right hand operand.
       * \return    The handler with the higher priority.
       */
      bool operator()(
        const std::unique_ptr<EventHandler>& lhs,
        const std::unique_ptr<EventHandler>& rhs
      ) const noexcept {
        return lhs->priority() < rhs->priority();
      }
    };

  //! \brief Receives emitted events and notifies handlers of it.
  class EventBus final {
    /*!
     * \brief  The type of the member callback.
     * \tparam Type A class type to get the member function off of.
     */
    template<typename Type>
    using MemberCallback = void(Type::*)(Event&);

  public:
    /*!
     * \brief     Registers a new handler for events that are emitted.
     * \tparam    Type The type of the member function owner.
     * \param[in] callee The instance of the type that owns the function.
     * \param[in] callback The member function.
     * \param[in] priority The priority of this handler over the others.
     */
    template<typename Type>
    static void registerHandler(Type* callee, MemberCallback<Type> callback, std::uint32_t priority) noexcept {
      mHandlers.emplace_back(std::make_unique<MemberEventHandler<Type>>(callee, callback, priority));
      std::sort(mHandlers.begin(), mHandlers.end(), EventHandlerPrioritySorter{});
    }

    /*!
     * \brief     Receives an event to pass to the handlers.
     * \param[in] evnt The event that was emitted.
     */
    static void emit(Event&& evnt) noexcept {
      for (auto& handler : mHandlers)
        if (!evnt.isConsumed())
          handler->notify(evnt);
    }

  private:
    //! \brief A list of the registered event handlers.
    inline static std::vector<std::unique_ptr<EventHandler>> mHandlers;
  };

}
