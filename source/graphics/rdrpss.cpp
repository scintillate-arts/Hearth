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
#include <hearth/graphics/rdrpss.hpp>

namespace HAPI_NAMESPACE_NAME::gfx {

  RenderPass::RenderPass() noexcept
    : mLogicalDevice(nullptr)
    , mRenderPass(nullptr)
  { }

  RenderPass::RenderPass(const CreateInfo& createInfo)
    : mLogicalDevice(createInfo.logicalDevice)
    , mRenderPass(nullptr)
  {
    initializeRenderPass(createInfo);
  }

  RenderPass::~RenderPass() noexcept {
    // Wasn't created or was moved.
    if (mRenderPass == nullptr)
      return;

    // Wait for device.
    vkDeviceWaitIdle(mLogicalDevice);

    // Delete.
    vkDestroyRenderPass(mLogicalDevice, mRenderPass, nullptr);
  }

  RenderPass::RenderPass(RenderPass&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mRenderPass(std::move(other.mRenderPass))
  {
    // Ensures.
    other.mLogicalDevice = nullptr;
    other.mRenderPass    = nullptr;
  }

  RenderPass& RenderPass::operator=(RenderPass&& other) noexcept {
    std::swap(mLogicalDevice, other.mLogicalDevice);
    std::swap(mRenderPass,    other.mRenderPass);
    return *this;
  }

  VkRenderPass RenderPass::handle() const noexcept {
    return mRenderPass;
  }

  void RenderPass::initializeRenderPass(const CreateInfo& createInfo) {
    // A place to store the attachments.
    std::vector<VkAttachmentDescription> attachments;
    for (auto attachment : createInfo.attachments) {
      VkAttachmentDescription attachDesc;
      {
        attachDesc.flags          = 0;
        attachDesc.format         = static_cast<VkFormat>(attachment->format);
        attachDesc.samples        = VK_SAMPLE_COUNT_1_BIT;
        attachDesc.loadOp         = static_cast<VkAttachmentLoadOp>(attachment->loadOp);
        attachDesc.storeOp        = static_cast<VkAttachmentStoreOp>(attachment->storeOp);
        attachDesc.stencilLoadOp  = static_cast<VkAttachmentLoadOp>(attachment->stencilLoadOp);
        attachDesc.stencilStoreOp = static_cast<VkAttachmentStoreOp>(attachment->stencilStoreOp);
        attachDesc.initialLayout  = static_cast<VkImageLayout>(attachment->initialLayout);
        attachDesc.finalLayout    = static_cast<VkImageLayout>(attachment->finalLayout);
      }

      // Move the attachment description into the known attachments.
      attachments.emplace_back(attachDesc);
    }

    // A place to store the subpasses.
    VkAttachmentReference              attachRef;
    std::vector<VkAttachmentReference> inputs;
    std::vector<VkAttachmentReference> colors;
    std::vector<VkSubpassDescription>  subpasses;
    std::size_t                        inputIndex = 0, colorIndex = 0;
    for (auto subpass : createInfo.subpasses) {
      // Get the index of the last.
      std::size_t inputCount = 0, colorCount = 0;

      // Get input references.
      for (auto& ref : subpass->inputAttachmentRefs) {
        attachRef.attachment = ref.index;
        attachRef.layout     = static_cast<VkImageLayout>(ref.layout);
        inputs.push_back(attachRef);
        inputCount++;
      }

      // Get color references.
      for (auto& ref : subpass->colorAttachmentRefs) {
        attachRef.attachment = ref.index;
        attachRef.layout     = static_cast<VkImageLayout>(ref.layout);
        colors.push_back(attachRef);
        colorCount++;
      }

      // To be used.
      VkAttachmentReference resolve;
      VkAttachmentReference depthStencil;

      // Prepare subpass information.
      VkSubpassDescription subpassDesc;
      {
        subpassDesc.flags                   = 0;
        subpassDesc.pipelineBindPoint       = static_cast<VkPipelineBindPoint>(subpass->pipelineBindPoint);
        subpassDesc.inputAttachmentCount    = static_cast<std::uint32_t>(inputCount);
        subpassDesc.pInputAttachments       = &inputs[inputIndex];
        subpassDesc.colorAttachmentCount    = static_cast<std::uint32_t>(colorCount);
        subpassDesc.pColorAttachments       = &colors[colorIndex];
        subpassDesc.pResolveAttachments     = nullptr;
        subpassDesc.pDepthStencilAttachment = nullptr;
        subpassDesc.preserveAttachmentCount = 0;
        subpassDesc.pPreserveAttachments    = nullptr;
      }

      // Move the description into the known subpasses.
      subpasses.emplace_back(subpassDesc);
      inputIndex = inputs.size() - 1;
      colorIndex = colors.size() - 1;
    }

    // Provide renderpass create info.
    VkRenderPassCreateInfo rdrpssCreateInfo;
    {
      rdrpssCreateInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      rdrpssCreateInfo.pNext           = nullptr;
      rdrpssCreateInfo.flags           = 0;
      rdrpssCreateInfo.attachmentCount = static_cast<std::uint32_t>(attachments.size());
      rdrpssCreateInfo.pAttachments    = attachments.data();
      rdrpssCreateInfo.subpassCount    = static_cast<std::uint32_t>(subpasses.size());
      rdrpssCreateInfo.pSubpasses      = subpasses.data();
      rdrpssCreateInfo.dependencyCount = 0;
      rdrpssCreateInfo.pDependencies   = nullptr;
    }

    // Try and create renderpass.
    VkResult result = vkCreateRenderPass(mLogicalDevice, &rdrpssCreateInfo, nullptr, &mRenderPass);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create render pass.");
  }

}
