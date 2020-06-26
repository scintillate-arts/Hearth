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
#include <fstream>
#include <stdexcept>
#include <hearth/graphics/gfxpip.hpp>
#include <hearth/graphics/dscset.hpp>

namespace HAPI_NAMESPACE_NAME::gfx {

  PipelineLayout::PipelineLayout() noexcept
    : mLogicalDevice(nullptr)
    , mPipelineLayout(nullptr)
  { }

  PipelineLayout::PipelineLayout(const CreateInfo& createInfo)
    : mLogicalDevice(createInfo.logicalDevice)
    , mPipelineLayout(nullptr)
  {
    // Get layouts.
    std::vector<VkDescriptorSetLayout> layouts;
    for (auto layout : createInfo.descriptorLayouts)
      layouts.push_back(layout->handle());

    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    {
      pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.pNext                  = nullptr;
      pipelineLayoutInfo.flags                  = 0;
      pipelineLayoutInfo.setLayoutCount         = static_cast<std::uint32_t>(layouts.size());
      pipelineLayoutInfo.pSetLayouts            = layouts.data();
      pipelineLayoutInfo.pushConstantRangeCount = 0;
      pipelineLayoutInfo.pPushConstantRanges    = nullptr;
    }

    VkResult result = vkCreatePipelineLayout(mLogicalDevice, &pipelineLayoutInfo, nullptr, &mPipelineLayout);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create pipeline layout.");
  }

  PipelineLayout::~PipelineLayout() noexcept {
    // Wasn't created or was moved.
    if (mPipelineLayout == nullptr)
      return;

    // Delete.
    vkDestroyPipelineLayout(mLogicalDevice, mPipelineLayout, nullptr);
  }

  PipelineLayout::PipelineLayout(PipelineLayout&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mPipelineLayout(std::move(other.mPipelineLayout))
  {
    // Ensures.
    other.mLogicalDevice  = nullptr;
    other.mPipelineLayout = nullptr;
  }

  PipelineLayout& PipelineLayout::operator=(PipelineLayout&& other) noexcept {
    std::swap(mLogicalDevice,  other.mLogicalDevice);
    std::swap(mPipelineLayout, other.mPipelineLayout);
    return *this;
  }

  VkPipelineLayout PipelineLayout::handle() const noexcept {
    return mPipelineLayout;
  }

  Pipeline::Pipeline() noexcept
    : mLogicalDevice(nullptr)
    , mGraphicsPipeline(nullptr)
  { }

  Pipeline::Pipeline(const CreateInfo& createInfo)
    : mLogicalDevice(createInfo.logicalDevice)
    , mGraphicsPipeline(nullptr)
  {
    initializePipeline(createInfo);
  }

  Pipeline::~Pipeline() noexcept {
    // Wasn't created or was moved.
    if (mGraphicsPipeline == nullptr)
      return;

    // Wait for device.
    vkDeviceWaitIdle(mLogicalDevice);

    // Delete.
    vkDestroyPipeline(mLogicalDevice, mGraphicsPipeline, nullptr);
  }

  Pipeline::Pipeline(Pipeline&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mGraphicsPipeline(std::move(other.mGraphicsPipeline))
  {
    // Ensures.
    other.mLogicalDevice    = nullptr;
    other.mGraphicsPipeline = nullptr;
  }

  Pipeline& Pipeline::operator=(Pipeline&& other) noexcept {
    std::swap(mLogicalDevice,    other.mLogicalDevice);
    std::swap(mGraphicsPipeline, other.mGraphicsPipeline);
    return *this;
  }

  VkPipeline Pipeline::handle() const noexcept {
    return mGraphicsPipeline;
  }

  static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
  }

  VkShaderModule createShaderModule(VkDevice logicalDevice, const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
      throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
  }

  void Pipeline::initializePipeline(const CreateInfo& createInfo) {
    auto vertShaderCode = readFile("./resources/vert.spv");
    auto fragShaderCode = readFile("./resources/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(mLogicalDevice, vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(mLogicalDevice, fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo;
    {
      vertShaderStageInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      vertShaderStageInfo.pNext               = nullptr;
      vertShaderStageInfo.flags               = 0;
      vertShaderStageInfo.stage               = VK_SHADER_STAGE_VERTEX_BIT;
      vertShaderStageInfo.module              = vertShaderModule;
      vertShaderStageInfo.pName               = "main";
      vertShaderStageInfo.pSpecializationInfo = nullptr;
    }

    VkPipelineShaderStageCreateInfo fragShaderStageInfo;
    {
      fragShaderStageInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      fragShaderStageInfo.pNext               = nullptr;
      fragShaderStageInfo.flags               = 0;
      fragShaderStageInfo.stage               = VK_SHADER_STAGE_FRAGMENT_BIT;
      fragShaderStageInfo.module              = fragShaderModule;
      fragShaderStageInfo.pName               = "main";
      fragShaderStageInfo.pSpecializationInfo = nullptr;
    }

    VkPipelineShaderStageCreateInfo shaderStages[] = {
      vertShaderStageInfo, fragShaderStageInfo
    };

    std::vector<VkVertexInputBindingDescription> bindings;
    for (auto vertBinding : createInfo.vertexBindings) {
      VkVertexInputBindingDescription vertBindDesc;
      {
        vertBindDesc.binding   = vertBinding->binding;
        vertBindDesc.stride    = vertBinding->stride;
        vertBindDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
      }

      bindings.push_back(vertBindDesc);
    }

    std::vector<VkVertexInputAttributeDescription> attribs;
    for (auto vertAttrib : createInfo.vertexAttributes) {
      VkVertexInputAttributeDescription vertAttrDesc;
      {
        vertAttrDesc.location = vertAttrib->location;
        vertAttrDesc.binding  = vertAttrib->binding;
        vertAttrDesc.format   = static_cast<VkFormat>(vertAttrib->format);
        vertAttrDesc.offset   = vertAttrib->offset;
      }

      attribs.push_back(vertAttrDesc);
    }

    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    {
      vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      vertexInputInfo.pNext                           = nullptr;
      vertexInputInfo.flags                           = 0;
      vertexInputInfo.vertexBindingDescriptionCount   = static_cast<std::size_t>(bindings.size());
      vertexInputInfo.pVertexBindingDescriptions      = bindings.data();
      vertexInputInfo.vertexAttributeDescriptionCount = static_cast<std::size_t>(attribs.size());
      vertexInputInfo.pVertexAttributeDescriptions    = attribs.data();
    }

    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    {
      inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
      inputAssembly.pNext                  = nullptr;
      inputAssembly.flags                  = 0;
      inputAssembly.topology               = static_cast<VkPrimitiveTopology>(createInfo.topology);
      inputAssembly.primitiveRestartEnable = VK_FALSE;
    }

    std::vector<VkViewport> viewports;
    for (auto& viewport : createInfo.viewports) {
      VkViewport vp;
      {
        vp.x        = viewport->origin.x;
        vp.y        = viewport->origin.y;
        vp.width    = static_cast<float>(viewport->extent.x);
        vp.height   = static_cast<float>(viewport->extent.y);
        vp.minDepth = viewport->minDepth;
        vp.maxDepth = viewport->maxDepth;
      }

      viewports.push_back(vp);
    }

    if (viewports.empty())
      throw std::runtime_error("Must provide at least one viewport.");

    std::vector<VkRect2D> scissors;
    for (auto& scissor : createInfo.scissors) {
      VkRect2D sc;
      {
        sc.offset = { scissor->offset.x, scissor->offset.y };
        sc.extent = { scissor->extent.x, scissor->extent.y };
      }

      scissors.push_back(sc);
    }

    if (scissors.empty())
      throw std::runtime_error("Must provide at least one scissor.");

    VkPipelineViewportStateCreateInfo viewportState;
    {
      viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      viewportState.pNext         = nullptr;
      viewportState.flags         = 0;
      viewportState.viewportCount = static_cast<std::uint32_t>(viewports.size());
      viewportState.pViewports    = viewports.data();
      viewportState.scissorCount  = static_cast<std::uint32_t>(scissors.size());
      viewportState.pScissors     = scissors.data();
    }

    VkPipelineRasterizationStateCreateInfo rasterizer;
    {
      rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
      rasterizer.pNext                   = nullptr;
      rasterizer.flags                   = 0;
      rasterizer.depthClampEnable        = VK_FALSE;
      rasterizer.rasterizerDiscardEnable = VK_FALSE;
      rasterizer.polygonMode             = static_cast<VkPolygonMode>(createInfo.polygonMode);
      rasterizer.cullMode                = static_cast<VkCullModeFlags>(createInfo.cullMode);
      rasterizer.frontFace               = static_cast<VkFrontFace>(createInfo.frontFace);
      rasterizer.depthBiasEnable         = VK_FALSE;
      rasterizer.depthBiasConstantFactor = 0.0f;
      rasterizer.depthBiasClamp          = 0.0f;
      rasterizer.depthBiasSlopeFactor    = 0.0f;
      rasterizer.lineWidth               = createInfo.lineWidth;
    }

    VkPipelineMultisampleStateCreateInfo multisampling;
    {
      multisampling.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      multisampling.pNext                 = nullptr;
      multisampling.flags                 = 0;
      multisampling.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
      multisampling.sampleShadingEnable   = VK_FALSE;
      multisampling.minSampleShading      = 0.0f;
      multisampling.pSampleMask           = 0;
      multisampling.alphaToCoverageEnable = VK_FALSE;
      multisampling.alphaToOneEnable      = VK_FALSE;
    }

    // Make sure we get a color blending state.
    if (createInfo.colorBlending == nullptr)
      throw std::runtime_error("Must have non-null color blending state.");

    std::vector<VkPipelineColorBlendAttachmentState> colorAttachments;
    VkPipelineColorBlendAttachmentState attachment;
    for (auto& attach : createInfo.colorBlending->attachments) {
      attachment.blendEnable         = attach->blendEnabled;
      attachment.srcColorBlendFactor = static_cast<VkBlendFactor>(attach->srcColorFactor);
      attachment.dstColorBlendFactor = static_cast<VkBlendFactor>(attach->dstColorFactor);
      attachment.colorBlendOp        = static_cast<VkBlendOp>(attach->colorOp);
      attachment.srcAlphaBlendFactor = static_cast<VkBlendFactor>(attach->srcAlphaFactor);
      attachment.dstAlphaBlendFactor = static_cast<VkBlendFactor>(attach->dstAlphaFactor);
      attachment.alphaBlendOp        = static_cast<VkBlendOp>(attach->alphaOp);
      attachment.colorWriteMask      = attach->colorWriteMask;
      colorAttachments.push_back(attachment);
    }

    // Provide blend state create info.
    VkPipelineColorBlendStateCreateInfo colorBlend;
    {
      colorBlend.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      colorBlend.pNext             = nullptr;
      colorBlend.flags             = 0;
      colorBlend.logicOpEnable     = createInfo.colorBlending->logicOpEnabled;
      colorBlend.logicOp           = static_cast<VkLogicOp>(createInfo.colorBlending->logicOp);
      colorBlend.attachmentCount   = static_cast<std::uint32_t>(colorAttachments.size());
      colorBlend.pAttachments      = colorAttachments.data();
      colorBlend.blendConstants[0] = createInfo.colorBlending->blendConstants[0];
      colorBlend.blendConstants[1] = createInfo.colorBlending->blendConstants[1];
      colorBlend.blendConstants[2] = createInfo.colorBlending->blendConstants[2];
      colorBlend.blendConstants[3] = createInfo.colorBlending->blendConstants[3];
    }

    // Expects.
    if (createInfo.layout == nullptr)
      throw std::runtime_error("Cannot create graphics pipeline with null layout.");

    VkGraphicsPipelineCreateInfo pipelineCreateInfo;
    {
      pipelineCreateInfo.sType                = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      pipelineCreateInfo.pNext                = nullptr;
      pipelineCreateInfo.flags                = 0;
      pipelineCreateInfo.stageCount           = 2;
      pipelineCreateInfo.pStages              = shaderStages;
      pipelineCreateInfo.pVertexInputState    = &vertexInputInfo;
      pipelineCreateInfo.pInputAssemblyState  = &inputAssembly;
      pipelineCreateInfo.pTessellationState   = nullptr;
      pipelineCreateInfo.pViewportState       = &viewportState;
      pipelineCreateInfo.pRasterizationState  = &rasterizer;
      pipelineCreateInfo.pMultisampleState    = &multisampling;
      pipelineCreateInfo.pDepthStencilState   = nullptr;
      pipelineCreateInfo.pColorBlendState     = &colorBlend;
      pipelineCreateInfo.pDynamicState        = nullptr;
      pipelineCreateInfo.layout               = createInfo.layout->handle();
      pipelineCreateInfo.renderPass           = createInfo.renderPass;
      pipelineCreateInfo.subpass              = createInfo.subpass;
      pipelineCreateInfo.basePipelineHandle   = nullptr;
      pipelineCreateInfo.basePipelineIndex    = 0;
    }

    VkResult result = vkCreateGraphicsPipelines(mLogicalDevice, nullptr, 1, &pipelineCreateInfo, nullptr, &mGraphicsPipeline);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create graphics pipeline.");

    vkDestroyShaderModule(mLogicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(mLogicalDevice, vertShaderModule, nullptr);
  }

}
