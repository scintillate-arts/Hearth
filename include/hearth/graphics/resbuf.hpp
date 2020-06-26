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
#include <cstdint>
#include <vulkan/vulkan.h>
#include "../forward.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief Represents a buffer of memory on the GPU that we can map CPU memory to.
  class ResourceBuffer {
  public:
    //! \brief Describes the type of usage a given resource buffer is.
    enum BufferUsage : std::uint16_t {
      UsageTransferSrcBit        = 0x00000001,
      UsageTransferDstBit        = 0x00000002,
      UsageUniformTexelBufferBit = 0x00000004,
      UsageStorageTexelBufferBit = 0x00000008,
      UsageUniformBufferBit      = 0x00000010,
      UsageStorageBufferBit      = 0x00000020,
      UsageIndexBufferBit        = 0x00000040,
      UsageVertexBufferBit       = 0x00000080,
      UsageIndirectBufferBit     = 0x00000100,
    };

    //! \brief The information needed to create this resource buffer.
    struct CreateInfo {
      //! \brief The physical device that we will be getting memory from.
      VkPhysicalDevice physicalDevice;

      //! \brief The logical device that will create the buffer.
      VkDevice logicalDevice;

      //! \brief The size of the buffer.
      std::size_t bufferSize;

      //! \brief The initial data to store in the resource buffer.
      const void* initialData;

      //! \brief The usage of the buffer.
      std::uint16_t bufferUsage;
    };

  public:
    //! \brief Explicitly defined default constructor.
    ResourceBuffer() noexcept;

    /*!
     * \brief     Explicitly defined constructor, create this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    ResourceBuffer(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~ResourceBuffer() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    ResourceBuffer(ResourceBuffer&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    ResourceBuffer& operator=(ResourceBuffer&& other) noexcept;

  public:
    /*!
     * \brief  Gets the buffer handle for this resource buffer.
     * \return The vulkan buffer given to this object on creation.
     */
    VkBuffer handle() const noexcept;

  private:
    /*!
     * \brief     Initializes this resource buffer.
     * \param[in] size The size of the resource buffer.
     * \param[in] usage The way this resource buffer will be used.
     */
    void initializeBuffer(std::size_t size, std::uint16_t usage);

    /*!
     * \brief     Maps the given data to this resource buffer's memory.
     * \param[in] size The size of the resource buffer.
     * \param[in] data The data to map to this resource buffer.
     */
    void mapMemory(std::size_t size, const void* data);

  private:
    //! \brief The physical device we will allocate memory from.
    VkPhysicalDevice mPhysicalDevice;

    //! \brief The device this resource buffer will be created from.
    VkDevice mLogicalDevice;

    //! \brief The handle to this resource buffer.
    VkBuffer mBufferHandle;

    //! \brief The memory this buffer stores.
    VkDeviceMemory mBufferMemory;
  };

}
