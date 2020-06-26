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
#include <hearth/graphics/dscset.hpp>
#include <hearth/graphics/resbuf.hpp>

namespace HAPI_NAMESPACE_NAME::gfx {

  DescriptorPool::DescriptorPool() noexcept
    : mLogicalDevice(nullptr)
    , mDescriptorPool(nullptr)
  { }

  DescriptorPool::DescriptorPool(const CreateInfo& createInfo)
    : mLogicalDevice(createInfo.logicalDevice)
    , mDescriptorPool(nullptr)
  {
    // Provide descriptor pool sizes.
    std::vector<VkDescriptorPoolSize> poolSizing;
    for (auto sizeInfo : createInfo.sizeInformations) {
      VkDescriptorPoolSize poolSize;
      {
        poolSize.type            = static_cast<VkDescriptorType>(sizeInfo->descriptorType);
        poolSize.descriptorCount = sizeInfo->descriptorCount;
      }

      poolSizing.push_back(poolSize);
    }

    // Provide descriptor pool create info.
    VkDescriptorPoolCreateInfo dscpllCreateInfo;
    {
      dscpllCreateInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      dscpllCreateInfo.pNext         = nullptr;
      dscpllCreateInfo.flags         = 0;
      dscpllCreateInfo.poolSizeCount = static_cast<std::uint32_t>(poolSizing.size());
      dscpllCreateInfo.pPoolSizes    = poolSizing.data();
      dscpllCreateInfo.maxSets       = createInfo.maxSets;
    }

    // Create descriptor pool.
    VkResult result = vkCreateDescriptorPool(mLogicalDevice, &dscpllCreateInfo, nullptr, &mDescriptorPool);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create descriptor pool.");
  }

  DescriptorPool::~DescriptorPool() noexcept {
    // Wasn't created or was moved.
    if (mDescriptorPool == nullptr)
      return;

    // Delete.
    vkDestroyDescriptorPool(mLogicalDevice, mDescriptorPool, nullptr);
  }

  DescriptorPool::DescriptorPool(DescriptorPool&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mDescriptorPool(std::move(other.mDescriptorPool))
  {
    // Ensures.
    other.mLogicalDevice  = nullptr;
    other.mDescriptorPool = nullptr;
  }

  DescriptorPool& DescriptorPool::operator=(DescriptorPool&& other) noexcept {
    std::swap(mLogicalDevice,  other.mLogicalDevice);
    std::swap(mDescriptorPool, other.mDescriptorPool);
    return *this;
  }

  VkDescriptorPool DescriptorPool::handle() const noexcept {
    return mDescriptorPool;
  }

  DescriptorSetLayout::DescriptorSetLayout() noexcept
    : mLogicalDevice(nullptr)
    , mDescriptorLayout(nullptr)
  { }

  DescriptorSetLayout::DescriptorSetLayout(const CreateInfo& createInfo)
    : mLogicalDevice(createInfo.logicalDevice)
    , mDescriptorLayout(nullptr)
  {
    // Provide descriptor set layout bindings.
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for (auto binding : createInfo.bindings) {
      VkDescriptorSetLayoutBinding dslBinding;
      {
        dslBinding.binding            = binding->binding;
        dslBinding.descriptorCount    = binding->descriptorCount;
        dslBinding.descriptorType     = static_cast<VkDescriptorType>(binding->descriptorType);
        dslBinding.stageFlags         = binding->stages;
        dslBinding.pImmutableSamplers = nullptr;
      }

      bindings.push_back(dslBinding);
    }

    // Provide descriptor set layout create info.
    VkDescriptorSetLayoutCreateInfo dslCreateInfo;
    {
      dslCreateInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
      dslCreateInfo.pNext        = nullptr;
      dslCreateInfo.flags        = 0;
      dslCreateInfo.bindingCount = static_cast<std::uint32_t>(bindings.size());
      dslCreateInfo.pBindings    = bindings.data();
    }

    // Create descriptor set layout.
    VkResult result = vkCreateDescriptorSetLayout(mLogicalDevice, &dslCreateInfo, nullptr, &mDescriptorLayout);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create descriptor set layout.");
  }

  DescriptorSetLayout::~DescriptorSetLayout() noexcept {
    // Wasn't created or was moved.
    if (mDescriptorLayout == nullptr)
      return;

    // Delete.
    vkDestroyDescriptorSetLayout(mLogicalDevice, mDescriptorLayout, nullptr);
  }

  DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mDescriptorLayout(std::move(other.mDescriptorLayout))
  {
    // Ensures.
    other.mLogicalDevice    = nullptr;
    other.mDescriptorLayout = nullptr;
  }

  DescriptorSetLayout& DescriptorSetLayout::operator=(DescriptorSetLayout&& other) noexcept {
    std::swap(mLogicalDevice,    other.mLogicalDevice);
    std::swap(mDescriptorLayout, other.mDescriptorLayout);
    return *this;
  }

  VkDescriptorSetLayout DescriptorSetLayout::handle() const noexcept {
    return mDescriptorLayout;
  }

  DescriptorSet::DescriptorSet() noexcept
    : mLogicalDevice(nullptr)
    , mDescriptorPool(nullptr)
    , mDescriptorSet(nullptr)
  { }

  DescriptorSet::DescriptorSet(const CreateInfo& createInfo)
    : mLogicalDevice(createInfo.logicalDevice)
    , mDescriptorPool(nullptr)
    , mDescriptorSet(nullptr)
  {
    // Expects.
    if (createInfo.descriptorPool == nullptr)
      throw std::runtime_error("Cannot create descriptor set from null descriptor pool.");

    // Expects.
    if (createInfo.descriptorLayout == nullptr)
      throw std::runtime_error("Cannot create descriptor set from null descriptor set layout.");

    // Set descriptor pool.
    mDescriptorPool = createInfo.descriptorPool->handle();

    // Prefetch descriptor layout because compiler complains about &(createInfo.descriptorLayout->handle());
    const auto layout = createInfo.descriptorLayout->handle();

    // Provide descriptor set allocation info.
    VkDescriptorSetAllocateInfo allocInfo;
    {
      allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.pNext              = nullptr;
      allocInfo.descriptorPool     = mDescriptorPool;
      allocInfo.descriptorSetCount = 1;
      allocInfo.pSetLayouts        = &layout;
    }

    // Allocate descriptor set.
    vkAllocateDescriptorSets(mLogicalDevice, &allocInfo, &mDescriptorSet);

    // Preupdate descriptor set.
    updateBuffers(createInfo.bufferInfos, createInfo.descriptorType);
  }

  DescriptorSet::~DescriptorSet() noexcept {
  }

  DescriptorSet::DescriptorSet(DescriptorSet&& other) noexcept
    : mLogicalDevice(std::move(other.mLogicalDevice))
    , mDescriptorPool(std::move(other.mDescriptorPool))
    , mDescriptorSet(std::move(other.mDescriptorSet))
  {
    // Ensures.
    other.mLogicalDevice = nullptr;
    other.mDescriptorSet = nullptr;
  }

  DescriptorSet& DescriptorSet::operator=(DescriptorSet&& other) noexcept {
    std::swap(mLogicalDevice,  other.mLogicalDevice);
    std::swap(mDescriptorPool, other.mDescriptorPool);
    std::swap(mDescriptorSet,  other.mDescriptorSet);
    return *this;
  }

  void DescriptorSet::updateBuffers(const std::vector<const BufferInfo*>& bufferInfos, DescriptorType descriptorType) {
    for (auto bufferInfo : bufferInfos) {
      VkDescriptorBufferInfo descBuffInfo;
      {
        descBuffInfo.buffer = bufferInfo->buffer->handle();
        descBuffInfo.offset = bufferInfo->bufferOffset;
        descBuffInfo.range  = bufferInfo->bufferSize;
      }

      VkWriteDescriptorSet writeDescSet;
      {
        writeDescSet.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescSet.pNext            = nullptr;
        writeDescSet.dstSet           = mDescriptorSet;
        writeDescSet.dstBinding       = bufferInfo->binding;
        writeDescSet.dstArrayElement  = 0;
        writeDescSet.descriptorCount  = 1;
        writeDescSet.descriptorType   = static_cast<VkDescriptorType>(descriptorType);
        writeDescSet.pImageInfo       = nullptr;
        writeDescSet.pBufferInfo      = &descBuffInfo;
        writeDescSet.pTexelBufferView = nullptr;
      }

      vkUpdateDescriptorSets(mLogicalDevice, 1, &writeDescSet, 0, nullptr);
    }
  }

  VkDescriptorSet DescriptorSet::handle() const noexcept {
    return mDescriptorSet;
  }

}
