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
#include "../config.hpp"
#include "format.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief Describes the 3 load operations for an attachment description.
  enum struct AttachmentLoadOp : std::uint8_t {
    Load, Clear, DontCare
  };

  //! \brief Describes the 2 store operations for an attachment description.
  enum struct AttachmentStoreOp : std::uint8_t {
    Store, DontCare
  };

  //! \brief Describes the layout of a given image at the beginning and end of a subpass.
  enum struct ImageLayout {
    Undefined                             = 0,
    General                               = 1,
    ColorAttachmentOptimal                = 2,
    DepthStencilAttachmentOptimal         = 3,
    DepthStencilReadOnlyOptimal           = 4,
    ShaderReadOnlyOptimal                 = 5,
    TransferSrcOptimal                    = 6,
    TransferDstOptimal                    = 7,
    Preinitialized                        = 8,
    DepthReadOnlyStencilAttachmentOptimal = 1000117000,
    DepthAttachmentStencilReadOnlyOptimal = 1000117001,
    PresentSrc                            = 1000001002,
    SharedPresent                         = 1000111000
  };

  //! \brief Describes the different pipeline bind points for a given subpass.
  enum struct PipelineBindPoint : std::uint8_t {
    Graphics, Compute, // Raytracing
  };

  //! \brief Describes the different stages of the graphics pipeline.
  enum PipelineStages : std::uint32_t{
    PipelineStageTopOfPipeBit             = 0x00000001,
    PipelineStageDrawIndirectBit          = 0x00000002,
    PipelineStageVertexInputBit           = 0x00000004,
    PipelineStageVertexShaderBit          = 0x00000008,
    PipelineStageTessCtrlShaderBit        = 0x00000010,
    PipelineStageTessEvalShaderBit        = 0x00000020,
    PipelineStageGeometryShaderBit        = 0x00000040,
    PipelineStageFragmentShaderBit        = 0x00000080,
    PipelineStageEarlyFragmentTestsBit    = 0x00000100,
    PipelineStageLateFragmentTestsBit     = 0x00000200,
    PipelineStageColorAttachmentOutputBit = 0x00000400,
    PipelineStageComputeShaderBit         = 0x00000800,
    PipelineStageTransferBit              = 0x00001000,
    PipelineStageBottomOfPipeBit          = 0x00002000,
    PipelineStageHostBit                  = 0x00004000,
    PipelineStageAllGraphicsBit           = 0x00008000,
    PipelineStageAllCommandsBit           = 0x00010000,
  };

  //! \brief Describes the image view of a framebuffer.
  struct AttachmentDescription {
    //! \brief The format that was used to create the swapchain.
    Format format;

    //! \brief The load operation for this attachment description.
    AttachmentLoadOp loadOp;

    //! \brief The store operation for this attachment description.
    AttachmentStoreOp storeOp;

    //! \brief The load operation for the stencil bits.
    AttachmentLoadOp stencilLoadOp;

    //! \brief The store operation for the stencil bits.
    AttachmentStoreOp stencilStoreOp;

    //! \brief The initial layout of the image view when the renderpass starts.
    ImageLayout initialLayout;

    //! \brief The final layout of the image view when the renderpass ends.
    ImageLayout finalLayout;
  };

  struct AttachmentReference {
    //! \brief The index of the attachment we are referencing.
    std::uint32_t index;

    //! \brief The layout of that attachment.
    ImageLayout layout;
  };

  //! \brief Describes a subpass of a single renderpass.
  struct SubpassDescription {
    /*!
     * \brief  A list of numerical references to the input attachments.
     * \remark Must be the indices of the attachments in the attachments vector of the renderpass
     *         create info.
     */
    std::vector<AttachmentReference> inputAttachmentRefs;

    /*!
     * \brief  A list of numerical references to the color attachments.
     * \remark Must be the indices of the attachments in the attachments vector of the renderpass
     *         create info.
     */
    std::vector<AttachmentReference> colorAttachmentRefs;

    /*!
     * \brief  A numerical reference to the resolve attachment.
     * \remark Must be the index of the attachment in the attachments vector of the renderpasss
     *         create info.
     */
    AttachmentReference resolveAttachmentRef;

    /*!
     * \brief  A numerical reference to the depth/stencil attachment.
     * \remark Must be the index of the attachment in the attachments vector of the renderpass
     *         create info.
     */
    AttachmentReference depthStencilAttachmentRef;

    //! \brief The binding point for this subpass.
    PipelineBindPoint pipelineBindPoint;
  };

  //! \brief
  class RenderPass final {
  public:
    //! \brief The information needed to create a renderpass.
    struct CreateInfo {
      //! \brief The attachments for this renderpass.
      std::vector<const AttachmentDescription*> attachments;

      //! \brief The subpasses for this renderpass.
      std::vector<const SubpassDescription*> subpasses;

      //! \brief The logical device that will create the renderpass.
      VkDevice logicalDevice;
    };

  public:
    //! \brief Explicitly defined default constructor.
    RenderPass() noexcept;

    /*!
     * \brief     Explicitly defined constructor, creates this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    RenderPass(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~RenderPass() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    RenderPass(RenderPass&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object datat that will be moved.
     * \return    This object with the new data.
     */
    RenderPass& operator=(RenderPass&& other) noexcept;

  public:
    /*!
     * \brief  Gets the handle this renderpass was given.
     * \return The VkRenderPass this renderpass object was created with.
     */
    VkRenderPass handle() const noexcept;

  private:
    /*!
     * \brief     Initializes this renderpass object.
     * \param[in] createInfo The information needed to create this renderpass.
     */
    void initializeRenderPass(const CreateInfo& createInfo);

  private:
    //! \brief The logical device this renderpass will be created with.
    VkDevice mLogicalDevice;

    //! \brief The renderpass handle given to us by vulkan.
    VkRenderPass mRenderPass;
  };

}
