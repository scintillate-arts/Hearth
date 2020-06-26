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
#include <array>
#include <utility>
#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include "../forward.hpp"
#include "../event.hpp"
#include "fence.hpp"
#include "format.hpp"
#include "semphr.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief The buffering strategy of a render context.
  enum struct BufferStrategy : std::uint8_t {
    SingleBuffer = 1,
    DoubleBuffer = 2,
    TripleBuffer = 3,
  };

  //! \brief Represents the swapchain as an object that which can be manipulated.
  class SwapChain final {
  public:
    //! \brief The information needed to create this swap chain.
    struct CreateInfo {
      //! \brief The surface the swapchain will be presenting images to.
      std::pair<Window*, VkSurfaceKHR> surfacePair;

      //! \brief The physical device the swapchain should be created from.
      VkPhysicalDevice physicalDevice;

      //! \brief The logical device the swapchain should be created from.
      VkDevice logicalDevice;

      //! \brief The resolution of the images of the swapchain, should match window resolution.
      glm::uvec2 imageResolution;

      //! \brief The requested image format for the swapchain.
      Format imageFormat;

      //! \brief The buffering strategy of the swapchain.
      BufferStrategy bufferStrategy;

      //! \brief Whether or not vsync should be enabled.
      bool vsyncEnabled;
    };

  public:
    //! \brief Explicitly defined default constructor.
    SwapChain() noexcept;

    /*!
     * \brief     Explicitly defined constructor, creates this object from the given information.
     * \param[in] creatInfo The information needed to create this object.
     */
    SwapChain(const CreateInfo& creatInfo);

    //! \brief Explicitly defined destructor, properly perpares this object for destruction.
   ~SwapChain() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    SwapChain(SwapChain&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object datat that will be moved.
     * \return    This object with the new data.
     */
    SwapChain& operator=(SwapChain&& other) noexcept;

  public:
    /*!
     * \brief  Gets the image views for this swapchain.
     * \return A const reference to the image views of this swapchain.
     */
    const std::array<VkImageView, 3>& imageViews() const noexcept;

    /*!
     * \brief  Gets the image views for this swapchain.
     * \return A non-const reference to the image view of this swapchain.
     */
    std::array<VkImageView, 3>& imageViews() noexcept;

    /*!
     * \brief  Gets the handle for this swapchain.
     * \return The handle given to this upon creation.
     */
    VkSwapchainKHR handle() const noexcept;

    /*!
     * \brief  The extent of the images of this swapchain.
     * \return The image extent.
     */
    glm::uvec2 imageResolution() const noexcept;

    /*!
     * \brief  The format of the images of this swapchain.
     * \return The image format.
     */
    Format imageFormat() const noexcept;

    /*!
     * \brief  The buffering strategy of this swapchain.
     * \return The buffer strategy this swapchain was created with.
     */
    BufferStrategy bufferStrategy() const noexcept;

    /*!
     * \brief     Presents the next image of the swapchain to the screen.
     * \param[in] presentQueue The queue with presentation support, to present the next image.
     */
    void present(VkQueue presentQueue);

  private:
    /*!
     * \brief     Initializes the swapchain proper, including images.
     * \param[in] resolution The resolution the swapchain images should be.
     * \param[in] requestedFormat The format the user has requested to use for the swapchain images.
     */
    void initializeSwapchain(const glm::uvec2& resolution, Format requestedFormat);

    //! \brief Initializes the swapchain image views.
    void initializeImageViews();

    //! \brief Initializes the semaphores used in image acquirement.
    void initializeSemaphores();

    /*!
     * \brief     Rebuilds the swapchain when something about it changes.
     * \param[in] resolution The new resolution of the swapchain images.
     */
    void rebuildSwapchain(const glm::uvec2& resolution);

  private:
    //! \brief The surface this swapchain will be presenting images to.
    std::pair<Window*, VkSurfaceKHR> mSurfacePair;

    //! \brief The images of the swapchain, that will be used to present rendered graphics to the screen.
    std::array<VkImage, 3> mImages;

    //! \brief An attachment of a framebuffer, defines the region of an image a renderpass will draw into.
    std::array<VkImageView, 3> mImageViews;

    //! \brief The physical device this swapchain should be created from.
    VkPhysicalDevice mPhysicalDevice;

    //! \brief The logical device that created this swapchain.
    VkDevice mLogicalDevice;

    //! \brief The swapchain handle given to us by vulkan.
    VkSwapchainKHR mSwapchain;

    //! \brief The semaphore signaled when an image is available.
    VkSemaphore mImageAvailable;

    //! \brief The semaphore signaled when rendering is done.
    VkSemaphore mRenderFinished;

    //! \brief The extent of the swapchain images.
    VkExtent2D mExtent;

    //! \brief The next available swapchain image.
    std::uint32_t mNextImage;

    //! \brief The format of the swapchain images.
    VkFormat mFormat;

    //! \brief The buffer strategy for this swapchain.
    BufferStrategy mBufferStrategy;

    //! \brief Whether or not vsync is enabled.
    bool mVsyncEnabled;
  };

}
