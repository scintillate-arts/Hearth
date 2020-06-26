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
#include <hearth/graphics/cmdbuf.hpp>

namespace HAPI_NAMESPACE_NAME::gfx {

  CommandPool::CommandPool() noexcept
    : mLogicalDevice(nullptr)
    , mCommandPool(nullptr)
  { }

  CommandPool::CommandPool(const CreateInfo& createInfo)
    : mLogicalDevice(createInfo.logicalDevice)
    , mCommandPool(nullptr)
  {
    VkCommandPoolCreateInfo poolInfo;
    {
      poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.pNext            = nullptr;
      poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
      poolInfo.queueFamilyIndex = createInfo.queueIndex;
    }

    if (vkCreateCommandPool(mLogicalDevice, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
      throw std::runtime_error("Failed to create command pool.");
  }

  CommandPool::~CommandPool() noexcept {
    // Wasn't created or was moved.
    if (mCommandPool == nullptr)
      return;

    // Delete.
    vkDestroyCommandPool(mLogicalDevice, mCommandPool, nullptr);
  }

  CommandPool::CommandPool(CommandPool&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mCommandPool(std::move(other.mCommandPool))
  {
    // Ensures.
    other.mLogicalDevice = nullptr;
    other.mCommandPool   = nullptr;
  }

  CommandPool& CommandPool::operator=(CommandPool&& other) noexcept {
    std::swap(mLogicalDevice, other.mLogicalDevice);
    std::swap(mCommandPool,   other.mCommandPool);
    return *this;
  }

  VkCommandPool CommandPool::handle() const noexcept {
    return mCommandPool;
  }

  CommandBuffer::CommandBuffer() noexcept
    : mLogicalDevice(nullptr)
    , mCommandBuffer(nullptr)
    , mRecordingFence(nullptr)
  {
  }

  CommandBuffer::CommandBuffer(const CreateInfo& createInfo)
    : mLogicalDevice(createInfo.logicalDevice)
    , mCommandBuffer(nullptr)
    , mRecordingFence(nullptr)
  {
    // Except.
    if (createInfo.commandPool == nullptr)
      throw std::runtime_error("Cannot create command buffer from null command pool.");

    // Provide allocation information.
    VkCommandBufferAllocateInfo allocInfo;
    {
      allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.pNext              = nullptr;
      allocInfo.commandPool        = createInfo.commandPool->handle();
      allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandBufferCount = 1;
    }

    // Allocate the command buffer.
    VkResult result = vkAllocateCommandBuffers(mLogicalDevice, &allocInfo, &mCommandBuffer);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create command buffer.");

    // Provide fence creation info.
    VkFenceCreateInfo fenceCreateInfo;
    {
      fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceCreateInfo.pNext = nullptr;
      fenceCreateInfo.flags = 0;
    }

    // Create fence.
    result = vkCreateFence(mLogicalDevice, &fenceCreateInfo, nullptr, &mRecordingFence);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create recording fence.");
  }

  CommandBuffer::~CommandBuffer() noexcept {
    // Wasn't created or was moved.
    if (mCommandBuffer == nullptr)
      return;

    // Wait for device then destroy fence.
    vkDeviceWaitIdle(mLogicalDevice);
    vkDestroyFence(mLogicalDevice, mRecordingFence, nullptr);
  }

  CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mCommandBuffer(std::move(other.mCommandBuffer))
    , mRecordingFence(std::move(other.mRecordingFence))
  {
    other.mLogicalDevice  = nullptr;
    other.mCommandBuffer  = nullptr;
    other.mRecordingFence = nullptr;
  }

  CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept {
    std::swap(mLogicalDevice,  other.mLogicalDevice);
    std::swap(mCommandBuffer,  other.mCommandBuffer);
    std::swap(mRecordingFence, other.mRecordingFence);
    return *this;
  }

  void CommandBuffer::begin() {
    // Wait for fence.
    vkWaitForFences(mLogicalDevice, 1, &mRecordingFence, VK_TRUE, UINT64_MAX);
    vkResetFences(mLogicalDevice, 1, &mRecordingFence);

    VkCommandBufferBeginInfo cmdBeginInfo;
    {
      cmdBeginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      cmdBeginInfo.pNext            = nullptr;
      cmdBeginInfo.flags            = 0;
      cmdBeginInfo.pInheritanceInfo = nullptr;
    }

    VkResult result = vkBeginCommandBuffer(mCommandBuffer, &cmdBeginInfo);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to begin command buffer recording.");
  }

  void CommandBuffer::end() {
    VkResult result = vkEndCommandBuffer(mCommandBuffer);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to end command buffer recording.");
  }

  void CommandBuffer::submit(VkQueue queue) {
    // Provide submit info.
    VkSubmitInfo submitInfo;
    {
      submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.pNext                = nullptr;
      submitInfo.waitSemaphoreCount   = 0;
      submitInfo.pWaitSemaphores      = nullptr;
      submitInfo.pWaitDstStageMask    = nullptr;
      submitInfo.commandBufferCount   = 1;
      submitInfo.pCommandBuffers      = &mCommandBuffer;
      submitInfo.signalSemaphoreCount = 0;
      submitInfo.pSignalSemaphores    = nullptr;
    }

    // Try and submit queue.
    VkResult result = vkQueueSubmit(queue, 1, &submitInfo, mRecordingFence);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to submit command buffer to queue.");
  }

  void CommandBuffer::updateBuffer(const ResourceBuffer* buffer, std::size_t offset, const void* data, std::size_t dataSize) {
    // Expects.
    if (buffer == nullptr)
      throw std::runtime_error("Cannot update null resource buffer.");

    // Update.
    vkCmdUpdateBuffer(mCommandBuffer, buffer->handle(), offset, dataSize, data);
  }

  void CommandBuffer::bindVertexBuffer(const ResourceBuffer* vertexBuffer) {
    // Expects.
    if (vertexBuffer == nullptr)
      throw std::runtime_error("Cannot bind null resource buffer to vertex buffer");

    // Provide binding information.
    VkBuffer     vertexBuffers[1] { vertexBuffer->handle() };
    VkDeviceSize offsets[1]       { 0 };
    vkCmdBindVertexBuffers(mCommandBuffer, 0, 1, vertexBuffers, offsets);
  }

  void CommandBuffer::bindIndexBuffer(const ResourceBuffer* indexBuffer) {
    // Expects.
    if (indexBuffer == nullptr)
      throw std::runtime_error("Cannot bind null resource buffer to index buffer.");

    // Bind.
    vkCmdBindIndexBuffer(mCommandBuffer, indexBuffer->handle(), 0, VK_INDEX_TYPE_UINT32);
  }

  void CommandBuffer::bindPipeline(const Pipeline* pipeline, PipelineBindPoint bindPoint) {
    // Expects.
    if (pipeline == nullptr)
      throw std::runtime_error("Cannot bind null Pipeline.");

    // Bind.
    vkCmdBindPipeline(mCommandBuffer, static_cast<VkPipelineBindPoint>(bindPoint), pipeline->handle());
  }

  void CommandBuffer::bindDescriptorSet(const DescriptorSet* descriptorSet, const PipelineLayout* layout) {
    // Expects.
    if (descriptorSet == nullptr)
      throw std::runtime_error("Cannot bind null descriptor set.");

    // Expects.
    if (layout == nullptr)
      throw std::runtime_error("Cannot bind descriptor set with null pipeline layout.");

    // Perform bind.
    auto dscset = descriptorSet->handle();
    vkCmdBindDescriptorSets(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout->handle(), 0, 1, &dscset, 0, nullptr);
  }

  void CommandBuffer::draw(std::uint32_t vertCount, std::uint32_t firstVertex) {
    vkCmdDraw(mCommandBuffer, vertCount, 1, firstVertex, 0);
  }

  void CommandBuffer::drawIndexed(std::uint32_t indCount, std::uint32_t firstIndex, std::uint32_t vertOffset) {
    vkCmdDrawIndexed(mCommandBuffer, indCount, 1, firstIndex, vertOffset, 0);
  }

  void CommandBuffer::beginRenderPass(const BeginRenderPassInfo& brpi) {
    // Expects.
    if (brpi.renderPass == nullptr)
      throw std::runtime_error("Cannot start renderpass on null RenderPass.");

    // Expects.
    if (brpi.frameBuffer == nullptr)
      throw std::runtime_error("Cannot start renderpass on null FrameBuffer.");

    // The current clear color until changed.
    const VkClearValue clearColor{ 0.0f, 0.0f, 0.0f, 1.0f };

    VkRenderPassBeginInfo rdrpssBeginInfo;
    {
      rdrpssBeginInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      rdrpssBeginInfo.pNext             = nullptr;
      rdrpssBeginInfo.renderPass        = brpi.renderPass->handle();
      rdrpssBeginInfo.framebuffer       = brpi.frameBuffer->handle();
      rdrpssBeginInfo.renderArea.offset = VkOffset2D { 0, 0 };
      rdrpssBeginInfo.renderArea.extent = VkExtent2D { brpi.renderAreaExtent.x, brpi.renderAreaExtent.y };
      rdrpssBeginInfo.clearValueCount   = 1;
      rdrpssBeginInfo.pClearValues      = &clearColor;
    }

    vkCmdBeginRenderPass(mCommandBuffer, &rdrpssBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
  }

  void CommandBuffer::endRenderPass() {
    vkCmdEndRenderPass(mCommandBuffer);
  }

}
