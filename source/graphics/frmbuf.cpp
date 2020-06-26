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
#include <hearth/graphics/frmbuf.hpp>

namespace HAPI_NAMESPACE_NAME::gfx {

  FrameBuffer::FrameBuffer() noexcept
    : mLogicalDevice(nullptr)
    , mFrameBuffer(nullptr)
  { }

  FrameBuffer::FrameBuffer(const CreateInfo& createInfo)
    : mLogicalDevice(createInfo.logicalDevice)
    , mFrameBuffer(nullptr)
  {
    initializeFrameBuffer(createInfo);
  }

  FrameBuffer::~FrameBuffer() noexcept {
    // Wasn't created or was moved.
    if (mFrameBuffer == nullptr)
      return;

    // Wait for device.
    vkDeviceWaitIdle(mLogicalDevice);

    // Delete.
    vkDestroyFramebuffer(mLogicalDevice, mFrameBuffer, nullptr);
  }

  FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mFrameBuffer(std::move(other.mFrameBuffer))
  {
    // Ensures.
    other.mLogicalDevice = nullptr;
    other.mFrameBuffer   = nullptr;
  }

  FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept {
    std::swap(mLogicalDevice, other.mLogicalDevice);
    std::swap(mFrameBuffer,   other.mFrameBuffer);
    return *this;
  }

  VkFramebuffer FrameBuffer::handle() const noexcept {
    return mFrameBuffer;
  }

  void FrameBuffer::initializeFrameBuffer(const CreateInfo& createInfo) {
    // Provide framebuffer create info.
    VkFramebufferCreateInfo fbCreateInfo;
    {
      fbCreateInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      fbCreateInfo.pNext           = nullptr;
      fbCreateInfo.flags           = 0;
      fbCreateInfo.renderPass      = createInfo.renderPass;
      fbCreateInfo.attachmentCount = static_cast<std::uint32_t>(createInfo.attachments.size());
      fbCreateInfo.pAttachments    = createInfo.attachments.data();
      fbCreateInfo.width           = createInfo.resolution.x;
      fbCreateInfo.height          = createInfo.resolution.y;
      fbCreateInfo.layers          = 1;
    }

    VkResult result = vkCreateFramebuffer(mLogicalDevice, &fbCreateInfo, nullptr, &mFrameBuffer);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create framebuffer.");
  }

}
