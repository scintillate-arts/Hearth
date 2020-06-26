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
#include <cstring>
#include <stdexcept>
#include <hearth/graphics/resbuf.hpp>

namespace HAPI_NAMESPACE_NAME::gfx {

  ResourceBuffer::ResourceBuffer() noexcept
    : mPhysicalDevice(nullptr)
    , mLogicalDevice(nullptr)
    , mBufferHandle(nullptr)
    , mBufferMemory(nullptr)
  { }

  ResourceBuffer::ResourceBuffer(const CreateInfo& createInfo)
    : mPhysicalDevice(createInfo.physicalDevice)
    , mLogicalDevice(createInfo.logicalDevice)
    , mBufferHandle(nullptr)
    , mBufferMemory(nullptr)
  {
    initializeBuffer(createInfo.bufferSize, createInfo.bufferUsage);
    mapMemory(createInfo.bufferSize, createInfo.initialData);
  }

  ResourceBuffer::~ResourceBuffer() noexcept {
    // Wasn't created or was moved.
    if (mBufferHandle == nullptr)
      return;

    // Wait for device and delete data.
    vkDeviceWaitIdle(mLogicalDevice);
    vkDestroyBuffer(mLogicalDevice, mBufferHandle, nullptr);
    vkFreeMemory(mLogicalDevice, mBufferMemory, nullptr);
  }

  ResourceBuffer::ResourceBuffer(ResourceBuffer&& other) noexcept
    : mPhysicalDevice(std::move(other.mPhysicalDevice))
    , mLogicalDevice(std::move(other.mLogicalDevice))
    , mBufferHandle(std::move(other.mBufferHandle))
    , mBufferMemory(std::move(other.mBufferMemory))
  {
    // Ensures.
    other.mPhysicalDevice = nullptr;
    other.mLogicalDevice  = nullptr;
    other.mBufferHandle   = nullptr;
    other.mBufferMemory   = nullptr;
  }

  ResourceBuffer& ResourceBuffer::operator=(ResourceBuffer&& other) noexcept {
    std::swap(mPhysicalDevice, other.mPhysicalDevice);
    std::swap(mLogicalDevice,  other.mLogicalDevice);
    std::swap(mBufferHandle,   other.mBufferHandle);
    std::swap(mBufferMemory,   other.mBufferMemory);
    return *this;
  }

  VkBuffer ResourceBuffer::handle() const noexcept {
    return mBufferHandle;
  }

  // TODO: make more efficient for the GPU.
  void ResourceBuffer::initializeBuffer(std::size_t size, std::uint16_t usage) {
    // Provide buffer create info.
    VkBufferCreateInfo resbufCreateInfo;
    {
      resbufCreateInfo.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      resbufCreateInfo.pNext                 = nullptr;
      resbufCreateInfo.flags                 = 0;
      resbufCreateInfo.size                  = size;
      resbufCreateInfo.usage                 = usage;
      resbufCreateInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
      resbufCreateInfo.queueFamilyIndexCount = 0;
      resbufCreateInfo.pQueueFamilyIndices   = nullptr;
    }

    // Create resource buffer.
    VkResult result = vkCreateBuffer(mLogicalDevice, &resbufCreateInfo, nullptr, &mBufferHandle);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create resource buffer.");

    // Get requirements.
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(mLogicalDevice, mBufferHandle, &memRequirements);

    // Prepare these for memory properties.
    const auto& filter     = memRequirements.memoryTypeBits;
    const auto  properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    // Get memory type.
    std::uint32_t                    type;
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &memProperties);
    for (std::size_t index = 0; index < memProperties.memoryTypeCount; index++)
      if ((filter & (1 << index)) && (memProperties.memoryTypes[index].propertyFlags & properties) == properties)
        type = index;

    // Prepare to allocate memory.
    VkMemoryAllocateInfo allocInfo;
    {
      allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.pNext           = nullptr;
      allocInfo.allocationSize  = memRequirements.size;
      allocInfo.memoryTypeIndex = type;
    }

    // Proceed to allocate memory.
    result = vkAllocateMemory(mLogicalDevice, &allocInfo, nullptr, &mBufferMemory);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to allocate memory on GPU for resource buffer.");
  }

  void ResourceBuffer::mapMemory(std::size_t size, const void* data) {
    // Bind memory before mapping.
    vkBindBufferMemory(mLogicalDevice, mBufferHandle, mBufferMemory, 0);

    // Get data map.
    void* buffer;
    vkMapMemory(mLogicalDevice, mBufferMemory, 0, size, 0, &buffer);
      std::memcpy(buffer, data, size);
    vkUnmapMemory(mLogicalDevice, mBufferMemory);
  }

}
