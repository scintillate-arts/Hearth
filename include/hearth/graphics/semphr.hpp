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
  class Semaphore {
  public:
    //! \brief Explicitly defined default constructor.
    Semaphore() noexcept;

    /*!
     * \brief     Explicitly defined constructor, creates this object from the given information.
     * \param[in] logicalDevice The device that will create this object.
     */
    Semaphore(VkDevice logicalDevice);

    //! \brief Explicitly defined desstructor, makes sure this object gets destroyed properly.
   ~Semaphore() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object to move data from.
     */
    Semaphore(Semaphore&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    Semaphore& operator=(Semaphore&& other) noexcept;

  public:
    /*!
     * \brief  Gets the handle of this semaphore.
     * \return The vulkan semaphore this was created with.
     */
    VkSemaphore handle() const noexcept;

  private:
    //! \brief The device that created this semaphore.
    VkDevice mLogicalDevice;

    //! \brief The semaphore handle that vulkan will give us.
    VkSemaphore mSemaphore;
  };

}
