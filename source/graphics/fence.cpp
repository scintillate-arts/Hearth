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
#include <hearth/graphics/fence.hpp>

namespace HAPI_NAMESPACE_NAME::gfx {

  Fence::Fence() noexcept
    : mLogicalDevice(nullptr)
    , mFence(nullptr)
  { }

  Fence::Fence(VkDevice logicalDevice)
    : mLogicalDevice(logicalDevice)
    , mFence(nullptr)
  {
    VkFenceCreateInfo createInfo;
    {
      createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      createInfo.pNext = nullptr;
      createInfo.flags = 0;
    }

    VkResult result = vkCreateFence(mLogicalDevice, &createInfo, nullptr, &mFence);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create fence.");
  }

  Fence::~Fence() noexcept {
    // Wasn't created or was moved.
    if (mFence == nullptr)
      return;

    // Wait for device.
    vkDeviceWaitIdle(mLogicalDevice);

    // Delete.
    vkDestroyFence(mLogicalDevice, mFence, nullptr);
  }

  Fence::Fence(Fence&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mFence(std::move(other.mFence))
  {
    // Ensures.
    other.mLogicalDevice = nullptr;
    other.mFence         = nullptr;
  }

  Fence& Fence::operator=(Fence&& other) noexcept {
    std::swap(mLogicalDevice, other.mLogicalDevice);
    std::swap(mFence,         other.mFence);
    return *this;
  }

  VkFence Fence::handle() const noexcept {
    return mFence;
  }

  void Fence::reset() {
    VkResult result = vkResetFences(mLogicalDevice, 1, &mFence);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to reset fence.");
  }

  void Fence::wait(std::uint64_t timeout) {
    VkResult result = vkWaitForFences(mLogicalDevice, 1, &mFence, VK_TRUE, timeout);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to wait for fence.");
  }

}
