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
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include "../forward.hpp"
#include "frmbuf.hpp"
#include "gfxpip.hpp"
#include "rdrpss.hpp"
#include "resbuf.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief Describes information for starting a renderpass.
  struct BeginRenderPassInfo {
    //! \brief The renderpass we will be starting.
    const RenderPass* renderPass;

    //! \brief The framebuffer we will be rendering to.
    const FrameBuffer* frameBuffer;

    //! \brief The render area extent.
    glm::uvec2 renderAreaExtent;
  };

  //! \brief Represents the object that allows command buffers to be allocated.
  class CommandPool {
  public:
    //! \brief The information needed to create a command pool.
    struct CreateInfo {
      //! \brief The logical device the command pool will be created from.
      VkDevice logicalDevice;

      //! \brief The queue index the command pool will allocate command buffers on.
      std::uint32_t queueIndex;
    };

  public:
    //! \brief Explicitly defined default constructor.
    CommandPool() noexcept;

    /*!
     * \brief     Explicitly defined constructor, creates this object from the given information.
     * \param[in] createInfo The information to create this object.
     */
    CommandPool(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~CommandPool() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object to move data from.
     */
    CommandPool(CommandPool&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    CommandPool& operator=(CommandPool&& other) noexcept;

  public:
    /*!
     * \brief  The handle to this command pool.
     * \return The handle that this command pool was given by vulkan at creation.
     */
    VkCommandPool handle() const noexcept;

  private:
    //! \brief The logical device that created this command pool.
    VkDevice mLogicalDevice;

    //! \brief The handle to this command pool, given to us by vulkan.
    VkCommandPool mCommandPool;
  };

  //! \brief
  class CommandBuffer {
  public:
    //! \brief The information needed to create a command buffer.
    struct CreateInfo {
      //! \brief The command pool to create the command buffer from.
      const CommandPool* commandPool;

      //! \brief The logical device the command buffer will be created with.
      VkDevice logicalDevice;
    };

  public:
    //! \brief Explicitly defined default constructor.
    CommandBuffer() noexcept;

    /*!
     * \brief     Explicitly defined constructor, creates this object from the given information.
     * \param[in] createInfo The information to create this object.
     */
    CommandBuffer(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~CommandBuffer() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object to move data from.
     */
    CommandBuffer(CommandBuffer&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    CommandBuffer& operator=(CommandBuffer&& other) noexcept;

  public:
    //! \brief Tells the command buffer to start recording.
    void begin();

    //! \brief Tells the command buffer to end recording.
    void end();

    /*!
     * \brief     Submits this command buffer for execution.
     * \param[in] queue The queue to submit this command buffer to.
     */
    void submit(VkQueue queue);

    /*!
     * \brief     Updates the data of a buffer.
     * \param[in] buffer The buffer to update.
     * \param[in] offset How far into the buffer to update
     * \param[in] data The new data.
     * \param[in] dataSize How much data there is.
     */
    void updateBuffer(const ResourceBuffer* buffer, std::size_t offset, const void* data, std::size_t dataSize);

    /*!
     * \brief     Updates the viewport of a bound graphics pipeline.
     * \param[in] viewport The new viewport.
     */
    void updateViewport(const Viewport& viewport);

    /*!
     * \brief     Updates the scissor of a bound graphics pipeline.
     * \param[in] scissor The new scissor.
     */
    void updateScissor(const Scissor& scissor);

    /*!
     * \brief     Binds the given vertex buffer.
     * \param[in] vertexBuffer The vertex buffer to bind.
     */
    void bindVertexBuffer(const ResourceBuffer* vertexBuffer);

    /*!
     * \brief     Binds the given index buffer.
     * \param[in] indexBuffer The index buffer to bind.
     */
    void bindIndexBuffer(const ResourceBuffer* indexBuffer);

    /*!
     * \brief     Binds the given pipeline the given bind point.
     * \param[in] pipeline The pipeline to bind.
     * \param[in] bindPoint The point to bind to.
     */
    void bindPipeline(const Pipeline* pipeline, PipelineBindPoint bindPoint);

    /*!
     * \brief     Binds the given descriptor set.
     * \param[in] descriptorSet The descriptor set that should be bound.
     * \param[in] layout The layout of the pipeline.
     */
    void bindDescriptorSet(const DescriptorSet* descriptorSet, const PipelineLayout* layout);

    /*!
     * \brief     Draws polygons from the given bound vertex buffers based on the pipeline.
     * \param[in] vertCount The number of vertices to draw.
     * \param[in] firstVertex The index of the first vertex to draw.
     */
    void draw(std::uint32_t vertCount, std::uint32_t firstVertex);


    /*!
     * \brief     Draws polygons from the given bound index buffers based on the pipeline.
     * \param[in] indCount The number of indices to draw.
     * \param[in] firstIndex The index of the first index to draw.
     * \param[in] vertOffset The offset of the first vertex to draw.
     */
    void drawIndexed(std::uint32_t indCount, std::uint32_t firstIndex, std::uint32_t vertOffset);

    /*!
     * \brief     Begins render pass recording with the given render context.
     * \param[in] brpi The information needed to start a renderpass.
     */
    void beginRenderPass(const BeginRenderPassInfo& brpi);

    //! \brief Ends render pass recording.
    void endRenderPass();

  private:
    //! \brief The logical device that this command buffer was created from.
    VkDevice mLogicalDevice;

    //! \brief The command pool this buffer was created from.
    VkCommandPool mCommandPool;

    //! \brief The command buffer this object represents.
    VkCommandBuffer mCommandBuffer;

    //! \brief The fence that will help synchronize the recording of this command buffer.
    VkFence mRecordingFence;
  };

}
