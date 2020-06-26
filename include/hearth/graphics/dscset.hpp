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
#include <vector>
#include <vulkan/vulkan.h>
#include "../forward.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief Describes the different types of descriptor sets a pipeline can have.
  enum struct DescriptorType : std::uint8_t {
    Sampler              = 0,
    CombinedSampler      = 1,
    SampledImage         = 2,
    StorageImage         = 3,
    UniformTexelBuffer   = 4,
    StorageTexelBuffer   = 5,
    UniformBuffer        = 6,
    StorageBuffer        = 7,
    UniformBufferDynamic = 8,
    StorageBufferDynamic = 9,
    InputAttachment      = 10,
  };

  //! \brief Represents and object that which can create descriptor sets.
  class DescriptorPool {
  public:
    //! \brief Describes information about the size of a portion of the descriptor pool.
    struct SizeInfo {
      //! \brief The number for descriptors the pool can create of the given type.
      std::uint32_t descriptorCount;

      //! \brief The type of descriptor the pool can create.
      DescriptorType descriptorType;
    };

    //! \brief The information needed to create this
    struct CreateInfo {
      //! \brief Information about how large to make the descriptor pool.
      std::vector<const SizeInfo*> sizeInformations;

      //! \brief The logical device that will create the descriptor pool.
      VkDevice logicalDevice;

      //! \brief The maximum number of sets the descriptor pool can have.
      std::uint32_t maxSets;
    };

  public:
    //! \brief Explicitly defined default constructor.
    DescriptorPool() noexcept;

    /*!
     * \brief     Explicitly defined constructor, create this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    DescriptorPool(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~DescriptorPool() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    DescriptorPool(DescriptorPool&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    DescriptorPool& operator=(DescriptorPool&& other) noexcept;

  public:
    /*!
     * \brief  Gets the handle for this descriptor pool.
     * \return The handle this descriptor pool was given by vulkan.
     */
    VkDescriptorPool handle() const noexcept;

  private:
    //! \brief The logical device that will create this descriptor pool.
    VkDevice mLogicalDevice;

    //! \brief The handle to this descriptor pool, given to us by vulkan.
    VkDescriptorPool mDescriptorPool;
  };

  //! \brief Describes a descriptor set layout for a graphics pipeline or descriptor set proper.
  class DescriptorSetLayout {
  public:
    //! \brief A how the descriptor set should be bound in the graphics pipeline.
    struct Binding {
      //! \brief The binding number of this entry, corresponds to a resource of the same binding.
      std::uint32_t binding;

      //! \brief The number of descriptors contained in the binding.
      std::uint32_t descriptorCount;

      //! \brief The shader stages the descriptor is available in.
      std::uint32_t stages;

      //! \brief The type of descriptors are being used.
      DescriptorType descriptorType;
    };

    //! \brief The information needed to create this descriptor set.
    struct CreateInfo {
      //! \brief The bindings for the descriptor set layout.
      std::vector<const Binding*> bindings;

      //! \brief The logical device the descriptor set layout will be created from.
      VkDevice logicalDevice;
    };

  public:
    //! \brief Explicitly defined default constructor.
    DescriptorSetLayout() noexcept;

    /*!
     * \brief     Explicitly defined constructor, create this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    DescriptorSetLayout(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~DescriptorSetLayout() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    DescriptorSetLayout(DescriptorSetLayout&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    DescriptorSetLayout& operator=(DescriptorSetLayout&& other) noexcept;

  public:
    /*!
     * \brief  Gets the handle for this descriptor set layout.
     * \return The handle this object was given by vulkan when it was created.
     */
    VkDescriptorSetLayout handle() const noexcept;

  private:
    //! \brief The logical device that this descriptor set layout will be created from.
    VkDevice mLogicalDevice;

    //! \brief The handle to the descriptor set layout, given to us by vulkan.
    VkDescriptorSetLayout mDescriptorLayout;
  };

  //! \brief Represents a set of descriptors that can be created.
  class DescriptorSet {
  public:
    //! \brief Describes information about the buffer this descriptor set represents.
    struct BufferInfo {
      //! \brief The buffer the descriptor set represents.
      const ResourceBuffer* buffer;

      //! \brief The offset in bytes from the start of the buffer.
      std::size_t bufferOffset;

      //! \brief The size in bytes of the buffer.
      std::size_t bufferSize;

      //! \brief The binding of the descriptor within the set.
      std::uint32_t binding;
    };

    //! \brief The information needed to create this descriptor set.
    struct CreateInfo {
      //! \brief A list of the descriptor buffer informations, helps write to the descriptor set.
      std::vector<const BufferInfo*> bufferInfos;

      //! \brief The pool to use to create the descriptor set.
      const DescriptorPool* descriptorPool;

      //! \brief The descriptor set layout to use.
      const DescriptorSetLayout* descriptorLayout;

      //! \brief The logical device that will create the descriptor set.
      VkDevice logicalDevice;

      //! \brief The type of descriptors the descriptor set stores.
      DescriptorType descriptorType;
    };

  public:
    //! \brief Explicitly defined default constructor.
    DescriptorSet() noexcept;

    /*!
     * \brief     Explicitly defined constructor, create this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    DescriptorSet(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~DescriptorSet() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    DescriptorSet(DescriptorSet&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    DescriptorSet& operator=(DescriptorSet&& other) noexcept;

  public:
    /*!
     * \brief     Updates the buffers for this descriptor set.
     * \param[in] bufferInfos Information about the buffers to update.
     * \param[in] descriptorType The type of descriptor that is being updated.
     */
    void updateBuffers(const std::vector<const BufferInfo*>& bufferInfos, DescriptorType descriptorType);

    /*!
     * \brief  Gets the handle for this descriptor set.
     * \return The handle this object was given by vulkan when it was created.
     */
    VkDescriptorSet handle() const noexcept;

  private:
    //! \brief The logical device that will create this descriptor set.
    VkDevice mLogicalDevice;

    //! \brief The descriptor pool this object was created from.
    VkDescriptorPool mDescriptorPool;

    //! \brief The handle to this descriptor set, given to us by vulkan.
    VkDescriptorSet mDescriptorSet;
  };

}
