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

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief
  class FrameBuffer final {
  public:
    //! \brief The information needed to create a framebuffer.
    struct CreateInfo {
      //! \brief The attachments for the framebuffer.
      std::vector<VkImageView> attachments;

      //! \brief The resolution of the framebuffer.
      glm::uvec2 resolution;

      //! \brief The logical device the framebuffer will be created with.
      VkDevice logicalDevice;

      //! \brief The renderpass type that the framebuffer will be able to accept.
      VkRenderPass renderPass;
    };

  public:
    //! \brief Explicitly defined default constructor.
    FrameBuffer() noexcept;

    /*!
     * \brief     Explicitly defined constructor, creates this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    FrameBuffer(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~FrameBuffer() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    FrameBuffer(FrameBuffer&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object datat that will be moved.
     * \return    This object with the new data.
     */
    FrameBuffer& operator=(FrameBuffer&& other) noexcept;
  public:
    /*!
     * \brief  Gets the handle this framebuffer was given.
     * \return The VkFramebuffer this framebuffer object was created with.
     */
    VkFramebuffer handle() const noexcept;

  private:
    /*!
     * \brief     Initializes this framebuffer object.
     * \param[in] createInfo The information needed to create this framebuffer.
     */
    void initializeFrameBuffer(const CreateInfo& createInfo);

  private:
    //! \brief The logical device that this framebuffer will be created with.
    VkDevice mLogicalDevice;

    //! \brief The framebuffer handle given to us by vulkan.
    VkFramebuffer mFrameBuffer;
  };

}
