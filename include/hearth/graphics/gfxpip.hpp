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
#include <vector>
#include <glm/glm.hpp>
#include "dscset.hpp"
#include "format.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief The types of available topology.
  enum struct TopologyType : std::uint8_t {
    PointList                  = 0,
    LineList                   = 1,
    LineStrip                  = 2,
    TriangleList               = 3,
    TriangleStrip              = 4,
    TriangleFan                = 5,
    LineListWithAdjacency      = 6,
    LineStripWithAdjacency     = 7,
    TriangleListWithAdjacency  = 8,
    TriangleStripWithAdjacency = 9,
    PatchList                  = 10,
  };

  //! \brief The different modes in which a pipeline can fill polygons.
  enum struct PolygonMode : std::uint8_t {
    Fill, Line, Point,
  };

  //! \brief The different culling modes for faces of polygons.
  enum struct FaceCullMode : std::uint8_t {
    None, Front, Back, Both
  };

  //! \brief The different front face generation modes.
  enum struct FrontFace : std::uint8_t {
    CounterClockwise, Clockwise
  };

  //! \brief Describes the blend factors for color blending.
  enum struct BlendFactor : std::uint8_t {
    Zero                = 0,
    One                 = 1,
    SrcColor            = 2,
    OneMinusSrcColor    = 3,
    DstColor            = 4,
    OneMinusDstColor    = 5,
    SrcAlpha            = 6,
    OneMinusSrcAlpha    = 7,
    DstAlpha            = 8,
    OneMinusDstAlpha    = 9,
    ConstColor          = 10,
    OneMinusConstColor  = 11,
    ConstAlpha          = 12,
    OneMinusConstAlpha  = 13,
    SrcAlphaSaturate    = 14,
    Src1Color           = 15,
    OneMinusSrc1Color   = 16,
    Src1Alpha           = 17,
    OneMinusSrc1Alpha   = 18,
  };

  //! \brief Describes the blend operations for color blending.
  enum struct BlendOp : std::uint8_t {
    Add             = 0,
    Subtract        = 1,
    ReverseSubtract = 2,
    Minimum         = 3,
    Maximum         = 4,
  };

  //! \brief Describes the logical operations to perform on a color blend state.
  enum struct LogicOp : std::uint8_t {
    Clear        = 0,
    And          = 1,
    AndReverse   = 2,
    Copy         = 3,
    AndInverted  = 4,
    NoOp         = 5,
    Xor          = 6,
    Or           = 7,
    Nor          = 8,
    Equivalent   = 9,
    Invert       = 10,
    OrReverse    = 11,
    CopyInverted = 12,
    OrInverted   = 13,
    Nand         = 14,
    Set          = 15,
  };

  //! \brief Describes different color components as a mask.
  enum ColorComponentFlags {
    ColorComponentRBit = 0x0001,
    ColorComponentGBit = 0x0002,
    ColorComponentBBit = 0x0004,
    ColorComponentABit = 0x0008,
  };

  //! \brief The shader stages of a pipeline.
  enum ShaderStages : std::uint8_t {
    ShaderStageVertexBit      = 0x00000001,
    ShaderStageTessCtrlBit    = 0x00000002,
    ShaderStageTessEvalBit    = 0x00000004,
    ShaderStageGeometryBit    = 0x00000008,
    ShaderStageFragmentBit    = 0x00000010,
    ShaderStageComputeBit     = 0x00000020,
    ShaderStageAllGraphicsBit = 0x0000001F,
  };

  //! \brief Represents the region of the framebuffer that a rendered image will be placed.
  struct Viewport {
    //! \brief The origin of this viewport.
    glm::ivec2 origin;

    //! \brief The size or extent of this viewport.
    glm::ivec2 extent;

    //! \brief The min depth of this viewport.
    float minDepth;

    //! \brief The max depth of this viewport.
    float maxDepth;
  };

  //! \brief Represents the visible area of a viewport.
  struct Scissor {
    //! \brief The offset of this scissor in relation to the viewport.
    glm::ivec2 offset;

    //! \brief The size or extent of this scissor.
    glm::uvec2 extent;
  };

  //! \brief Describes how a vertex buffer is bound to a pipeline.
  struct BindingDescription {
    //! \brief The binding number this structure represents.
    std::uint32_t binding;

    //! \brief The distance between two consecutive elements in a vertex buffer, in bytes.
    std::uint32_t stride;
  };

  //! \brief Describes how each attribute of a vertex buffer is lane out.
  struct AttributeDescription {
    //! \brief The location the vertex attribute is bound to.
    std::uint32_t location;

    //! \brief The binding number which this attribute takes it's data from.
    std::uint32_t binding;

    //! \brief The format of the data at that attribute.
    Format format;

    //! \brief The offset of the attribute in the data.
    std::uint32_t offset;
  };

  //! \brief Represents the color blending attachments for a color blending state.
  struct ColorBlendAttachment {
    //! \brief The blend factor for the source color.
    BlendFactor srcColorFactor;

    //! \brief The blend factor for the destination color.
    BlendFactor dstColorFactor;

    //! \brief The color blend operation.
    BlendOp colorOp;

    //! \brief The blend factor for the source alpha color.
    BlendFactor srcAlphaFactor;

    //! \brief The blend factor for the destination alpha color.
    BlendFactor dstAlphaFactor;

    //! \brief The alpha color blend operation.
    BlendOp alphaOp;

    //! \brief The mask for blending.
    std::uint8_t colorWriteMask;

    //! \brief Whether or not blending is enabled for this attachment.
    bool blendEnabled;
  };

  //! \brief Represents the color blending state for the graphics pipeline.
  struct ColorBlendState {
    //! \brief The attachments for color blending operations.
    std::vector<const ColorBlendAttachment*> attachments;

    //! \brief The blending constants for this color blend state.
    float blendConstants[4];

    //! \brief The logic operation to perform when the logic op is enabled.
    LogicOp logicOp;

    //! \brief Whether or not the logic operation should be enabled.
    bool logicOpEnabled;
  };

  //! \brief Describes the layout of a pipeline so that shader stages can receive exterior memory.
  class PipelineLayout {
  public:
    //! \brief The information needed to create this pipeline layout.
    struct CreateInfo {
      //! \brief The a list of the descriptor set layouts for the pipeline layout.
      std::vector<const DescriptorSetLayout*> descriptorLayouts;

      //! \brief The logical device the pipeline layout will be created from.
      VkDevice logicalDevice;
    };

  public:
    //! \brief Explicitly defined default constructor.
    PipelineLayout() noexcept;

    /*!
     * \brief     Explicitly defined constructor, create this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    PipelineLayout(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~PipelineLayout() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    PipelineLayout(PipelineLayout&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    PipelineLayout& operator=(PipelineLayout&& other) noexcept;

  public:
    /*!
     * \brief  Gets handle to this pipeline layout.
     * \return The vulkan pipeline layout object this pipeline layout was created with.
     */
    VkPipelineLayout handle() const noexcept;

  private:
    //! \brief The logical device this pipeline layout was created from.
    VkDevice mLogicalDevice;

    //! \brief The handle to the pipeline layout, given to us by vulkan.
    VkPipelineLayout mPipelineLayout;
  };

  //! \brief Represents the entire graphics pipeline.
  class Pipeline {
  public:
    //! \brief The information needed to create this graphics pipeline.
    struct CreateInfo {
      //! \brief The binding descriptions for a vertex buffer that is compatible with this pipeline.
      std::vector<const BindingDescription*> vertexBindings;

      //! \brief The attribute descriptions for a vertex buffer that is compatible with this pipeline.
      std::vector<const AttributeDescription*> vertexAttributes;

      //! \brief The states of color blending the pipeline should apply.
      const ColorBlendState* colorBlending;

      //! \brief The layout for the pipeline.
      const PipelineLayout* layout;

      //! \brief The base pipeline to create the pipeline from.
      Pipeline* base;

      //! \brief The logical device that will the pipeline.
      VkDevice logicalDevice;

      //! \brief The renderpass the pipeline should be compatible with.
      VkRenderPass renderPass;

      //! \brief The subpass the pipeline should be bound to.
      std::uint32_t subpass;

      //! \brief The width of lines generated with PolygonMode::Line.
      float lineWidth;

      //! \brief The type of topology the pipeline will generate.
      TopologyType topology;

      //! \brief The polygon mode the pipeline will work with.
      PolygonMode polygonMode;

      //! \brief The culling mode for the pipeline.
      FaceCullMode cullMode;

      //! \brief The front face generation mode for the pipeline.
      FrontFace frontFace;
    };

  public:
    //! \brief Explicitly defined default constructor.
    Pipeline() noexcept;

    /*!
     * \brief     Explicitly defined constructor, create this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    Pipeline(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~Pipeline() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    Pipeline(Pipeline&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    Pipeline& operator=(Pipeline&& other) noexcept;

  public:
    /*!
     * \brief  Gets handle to this graphics pipeline.
     * \return The vulkan pipeline object this graphics pipeline was created with.
     */
    VkPipeline handle() const noexcept;

  private:
    /*!
     * \brief     Initializes this graphics pipeline.
     * \param[in] createInfo The information needed to create this graphics pipeline.
     */
    void initializePipeline(const CreateInfo& createInfo);

  private:
    //! \brief The logical device that will create this pipeline.
    VkDevice mLogicalDevice;

    //! \brief The pipeline handle that vulkan will give us.
    VkPipeline mGraphicsPipeline;
  };

}
