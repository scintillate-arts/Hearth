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
#include <vulkan/vulkan.h>
#include "../forward.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief
  class Fence {
  public:
    //! \brief Explicitly defined default constructor.
    Fence() noexcept;

    /*!
     * \brief     Explicitly defined constructor, creates this object from the given information.
     * \param[in] logicalDevice The device that will create this object.
     */
    Fence(VkDevice logicalDevice);

    //! \brief Explicitly defined desstructor, makes sure this object gets destroyed properly.
   ~Fence() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object to move data from.
     */
    Fence(Fence&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    Fence& operator=(Fence&& other) noexcept;

  public:
    /*!
     * \brief  Gets the handle for this fence.
     * \return The handle that vulkan gave us when this object was created.
     */
    VkFence handle() const noexcept;

    //! \brief Resets this fence to be used again.
    void reset();

    /*!
     * \brief     Waits for this fence to be signaled.
     * \param[in] timeout The amount of time to wait, in nanoseconds.
     */
    void wait(std::uint64_t timeout);

  private:
    //! \brief The device that will create this fence.
    VkDevice mLogicalDevice;

    //! \brief The handle that vulkan will give us.
    VkFence mFence;
  };

}
