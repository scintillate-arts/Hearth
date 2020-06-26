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
#include <stdexcept>
#include <hearth/graphics/semphr.hpp>

namespace HAPI_NAMESPACE_NAME::gfx {

  Semaphore::Semaphore() noexcept
    : mLogicalDevice(nullptr)
    , mSemaphore(nullptr)
  { }

  Semaphore::Semaphore(VkDevice logicalDevice)
    : mLogicalDevice(logicalDevice)
    , mSemaphore(nullptr)
  {
    VkSemaphoreCreateInfo createInfo;
    {
      createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
      createInfo.pNext = nullptr;
      createInfo.flags = 0;
    }

    VkResult result = vkCreateSemaphore(mLogicalDevice, &createInfo, nullptr, &mSemaphore);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create Semaphore.");
  }

  Semaphore::~Semaphore() noexcept {
    // Wasn't created or was moved.
    if (mSemaphore == nullptr)
      return;

    // Wait for device.
    vkDeviceWaitIdle(mLogicalDevice);

    // Delete.
    vkDestroySemaphore(mLogicalDevice, mSemaphore, nullptr);
  }

  Semaphore::Semaphore(Semaphore&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mSemaphore(std::move(other.mSemaphore))
  {
    // Ensures.
    other.mLogicalDevice = nullptr;
    other.mSemaphore         = nullptr;
  }

  Semaphore& Semaphore::operator=(Semaphore&& other) noexcept {
    std::swap(mLogicalDevice, other.mLogicalDevice);
    std::swap(mSemaphore,         other.mSemaphore);
    return *this;
  }

  VkSemaphore Semaphore::handle() const noexcept {
    return mSemaphore;
  }

}
