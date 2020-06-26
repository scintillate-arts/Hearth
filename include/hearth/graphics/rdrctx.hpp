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
#include <cstdint>
#include <string_view>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include "../forward.hpp"
#include "cmdbuf.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief Represents an object that can perform rendering operations on a window of an Application.
  class RenderContext final {
  public:
    //! \brief The information the render context will be created with.
    struct CreateInfo {
      //! \brief The name of the application.
      std::string_view appName;

      //! \brief The window the render context will render to.
      Window* surface;

      //! \brief The version of the application.
      std::uint32_t appVersion;
    };

  public:
    //! \brief Explicitly defined default constructor.
    RenderContext() noexcept;

    /*!
     * \brief     Explicitly defined constructor, creates this object from the given information.
     * \param[in] createInfo The information needed to create this object.
     */
    RenderContext(const CreateInfo& createInfo);

    //! \brief Explicitly defined destructor, properly prepares this object for destruction.
   ~RenderContext() noexcept;

    /*!
     * \brief     Explicitly defined move constructor, allows moving data of another object into
     *            this one.
     * \param[in] other The object data that will be moved.
     */
    RenderContext(RenderContext&& other) noexcept;

    /*!
     * \brief     Explicitly defined move assignment operator, allows moving data of another object
     *            into this one.
     * \param[in] other The object data that will be moved.
     * \return    This object with the new object data.
     */
    RenderContext& operator=(RenderContext&& other) noexcept;

  public:
    /*!
     * \brief  Gets the vulkan instance this render context created on construction.
     * \return The vkinstance this render context tracks.
     */
    VkInstance environment() const noexcept;

    // TODO: move these.
    VkSurfaceKHR surface() const noexcept { return mSurface; }
    VkPhysicalDevice physicalDevice() const noexcept { return mPhysicalDevice; }
    VkDevice logicalDevice() const noexcept { return mLogicalDevice; }
    VkQueue graphicsQueue() const noexcept { return mGraphicsQueuePair.first; }
    VkQueue presentQueue() const noexcept { return mPresentQueuePair.first; }
    std::uint32_t graphicsQueueIndex() const noexcept { return mGraphicsQueuePair.second; }
    std::uint32_t presentQueueIndex() const noexcept { return mPresentQueuePair.second; }

  private:
    /*!
     * \brief     Initializes the vulkan instance.
     * \param[in] appName The name of the app the instance is being created for.
     * \param[in] appVersion The version of the app the instance is being created for.
     */
    void initializeInstance(std::string_view appName, std::uint32_t appVersion);

    /*!
     * \brief     Initializes the vulkan surface.
     * \param[in] surface The window that we plan to give to vulkan.
     */
    void initializeSurface(Window* surface);

    //! \brief Performs operations to choose a suitable physical device to render with.
    void pickPhysicalDevice();

    //! \brief Initailizes the logical device for this render context.
    void initializeLogicalDevice();

  private:
    //! \brief The queue graphics commands will be sent to.
    std::pair<VkQueue, std::uint32_t> mGraphicsQueuePair;

    //! \brief The queue present commands will be sent to.
    std::pair<VkQueue, std::uint32_t> mPresentQueuePair;

    //! \brief The vulkan instance that will allow us to interact with the driver.
    VkInstance mInstance;

    //! \brief The surface vulkan will render to.
    VkSurfaceKHR mSurface;

    //! \brief The physical device that we found to be suitable for the needs of the application.
    VkPhysicalDevice mPhysicalDevice;

    //! \brief The logical device that we will be using to get memory and other things from the GPU.
    VkDevice mLogicalDevice;

  #if defined(HAPI_DEBUG)
    //! \brief Provides methods of debugging for the instance.
    VkDebugUtilsMessengerEXT mDebugMessenger;
  #endif
  };

}
