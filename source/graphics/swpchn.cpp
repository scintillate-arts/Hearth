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
#include <vector>
#include <hearth/graphics/swpchn.hpp>
#include "queuefamily.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

  // Details about the supported swap chain.
  struct SwapchainSupportDetails final {
    VkSurfaceCapabilitiesKHR        capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>   presentModes;
  };

  // Gets the details of what the swapchain can support.
  SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapchainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    std::uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount != 0) {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    std::uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
  }

  VkSurfaceFormatKHR chooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats, Format requestedFormat) {
    if (availableFormats.empty())
      throw std::runtime_error("No Vulkan surface formats available.");

    if (availableFormats.size() == 1 && availableFormats.front().format == VK_FORMAT_UNDEFINED)
      return VkSurfaceFormatKHR{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

    for (const auto& format: availableFormats)
      if (format.format == static_cast<VkFormat>(requestedFormat) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        return format;

    return availableFormats.front();
  }

  VkPresentModeKHR chooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availableModes, bool vsyncEnabled) {
    if (!vsyncEnabled) {
      for (const auto& mode: availableModes)
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR || mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
          return mode;
    }

    return VK_PRESENT_MODE_FIFO_KHR; // Should always be available.
  }

  VkExtent2D chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities, std::uint32_t width, std::uint32_t height) {
    if (capabilities.currentExtent.width == std::numeric_limits<std::uint32_t>::max()) {
      const auto finalWidth  = std::clamp(width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width);
      const auto finalHeight = std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
      return VkExtent2D{ finalWidth, finalHeight };
    }

    return capabilities.currentExtent;
  }

  SwapChain::SwapChain() noexcept
    : mSurfacePair{ nullptr, nullptr }
    , mImages{ nullptr, nullptr, nullptr }
    , mImageViews{ nullptr, nullptr, nullptr }
    , mPhysicalDevice(nullptr)
    , mLogicalDevice(nullptr)
    , mSwapChain(nullptr)
    , mImageAvailable(nullptr)
    , mRenderFinished(nullptr)
    , mExtent{ 0, 0 }
    , mNextImage(0)
    , mFormat(VK_FORMAT_UNDEFINED)
    , mBufferStrategy(BufferStrategy::DoubleBuffer)
    , mVsyncEnabled(false)
  { }

  SwapChain::SwapChain(const CreateInfo& createInfo)
    : mSurfacePair(createInfo.surfacePair)
    , mImages{ nullptr, nullptr, nullptr }
    , mImageViews{ nullptr, nullptr, nullptr }
    , mPhysicalDevice(createInfo.physicalDevice)
    , mLogicalDevice(createInfo.logicalDevice)
    , mSwapChain(nullptr)
    , mImageAvailable(nullptr)
    , mRenderFinished(nullptr)
    , mExtent{ 0, 0 }
    , mNextImage(0)
    , mFormat(VK_FORMAT_UNDEFINED)
    , mBufferStrategy(createInfo.bufferStrategy)
    , mVsyncEnabled(createInfo.vsyncEnabled)
  {
    initializeSwapchain(createInfo.imageResolution, createInfo.imageFormat);
    initializeImageViews();
    initializeSemaphores();
  }

  SwapChain::~SwapChain() noexcept {
    // Didn't get created or was moved.
    if (mSwapChain == nullptr)
      return;

    // Wait for device.
    vkDeviceWaitIdle(mLogicalDevice);

    // Delete semaphores.
    vkDestroySemaphore(mLogicalDevice, mImageAvailable, nullptr);
    vkDestroySemaphore(mLogicalDevice, mRenderFinished, nullptr);

    // Delete views and swapchain.
    for (auto imageView : mImageViews)
      vkDestroyImageView(mLogicalDevice, imageView, nullptr);
    vkDestroySwapchainKHR(mLogicalDevice, mSwapChain, nullptr);
  }

  SwapChain::SwapChain(SwapChain&& other) noexcept
    : mSurfacePair(std::move(other.mSurfacePair))
    , mImages(std::move(other.mImages))
    , mImageViews(std::move(other.mImageViews))
    , mPhysicalDevice(std::move(other.mPhysicalDevice))
    , mLogicalDevice(std::move(other.mLogicalDevice))
    , mSwapChain(std::move(other.mSwapChain))
    , mImageAvailable(std::move(other.mImageAvailable))
    , mRenderFinished(std::move(other.mRenderFinished))
    , mExtent(std::move(other.mExtent))
    , mNextImage(other.mNextImage)
    , mFormat(other.mFormat)
    , mBufferStrategy(other.mBufferStrategy)
    , mVsyncEnabled(other.mVsyncEnabled)
  {
    // Ensures.
    other.mSurfacePair    = std::pair<Window*, VkSurfaceKHR>{ nullptr, nullptr };
    other.mImages         = std::array<VkImage,     3>{ nullptr, nullptr, nullptr };
    other.mImageViews     = std::array<VkImageView, 3>{ nullptr, nullptr, nullptr };
    other.mPhysicalDevice = nullptr;
    other.mLogicalDevice  = nullptr;
    other.mSwapChain      = nullptr;
    other.mImageAvailable = nullptr;
    other.mRenderFinished = nullptr;
  }

  SwapChain& SwapChain::operator=(SwapChain&& other) noexcept {
    std::swap(mSurfacePair,    other.mSurfacePair);
    std::swap(mImages,         other.mImages);
    std::swap(mImageViews,     other.mImageViews);
    std::swap(mPhysicalDevice, other.mPhysicalDevice);
    std::swap(mLogicalDevice,  other.mLogicalDevice);
    std::swap(mSwapChain,      other.mSwapChain);
    std::swap(mImageAvailable, other.mImageAvailable);
    std::swap(mRenderFinished, other.mRenderFinished);
    std::swap(mExtent,         other.mExtent);
    std::swap(mNextImage,      other.mNextImage);
    std::swap(mFormat,         other.mFormat);
    std::swap(mBufferStrategy, other.mBufferStrategy);
    std::swap(mVsyncEnabled,   other.mVsyncEnabled);
    return *this;
  }

  const std::array<VkImageView, 3>& SwapChain::imageViews() const noexcept {
    return mImageViews;
  }

  std::array<VkImageView, 3>& SwapChain::imageViews() noexcept {
    return mImageViews;
  }

  VkSwapchainKHR SwapChain::handle() const noexcept {
    return mSwapChain;
  }

  glm::uvec2 SwapChain::imageResolution() const noexcept {
    return glm::uvec2{ mExtent.width, mExtent.height };
  }

  Format SwapChain::imageFormat() const noexcept {
    return static_cast<Format>(mFormat);
  }

  BufferStrategy SwapChain::bufferStrategy() const noexcept {
    return mBufferStrategy;
  }

  void SwapChain::present(VkQueue presentQueue) {
    VkResult result = vkAcquireNextImageKHR(mLogicalDevice, mSwapChain, UINT64_MAX, mImageAvailable, VK_NULL_HANDLE, &mNextImage);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      rebuildSwapChain(glm::uvec2{ mExtent.width, mExtent.height });
      return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
      throw std::runtime_error("Failed to acquire next image.");

    VkSemaphore          signalSemaphores[1] = { mRenderFinished };
    VkSemaphore          waitSemaphores[1]   = { mImageAvailable };
    VkPipelineStageFlags waitStages[1]       = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo;
    {
      submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.pNext                = nullptr;
      submitInfo.waitSemaphoreCount   = 1;
      submitInfo.pWaitSemaphores      = waitSemaphores;
      submitInfo.pWaitDstStageMask    = waitStages;
      submitInfo.commandBufferCount   = 0;
      submitInfo.pCommandBuffers      = nullptr;
      submitInfo.signalSemaphoreCount = 1;
      submitInfo.pSignalSemaphores    = signalSemaphores;
    }

    result = vkQueueSubmit(presentQueue, 1, &submitInfo, VK_NULL_HANDLE);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to submit semaphore to queue on present.");

    VkPresentInfoKHR presinfo;
    {
      presinfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
      presinfo.pNext              = nullptr;
      presinfo.waitSemaphoreCount = 1;
      presinfo.pWaitSemaphores    = signalSemaphores;
      presinfo.swapchainCount     = 1;
      presinfo.pSwapchains        = &mSwapChain;
      presinfo.pImageIndices      = &mNextImage;
      presinfo.pResults           = 0;
    }

    result = vkQueuePresentKHR(presentQueue, &presinfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
      rebuildSwapChain(glm::uvec2{ mExtent.width, mExtent.height });
    else if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to present.");
  }

  void SwapChain::reseat(const glm::uvec2& resolution) {
    rebuildSwapChain(resolution);
  }

  void SwapChain::initializeSwapchain(const glm::uvec2& resolution, Format requestedFormat) {
    SwapchainSupportDetails swapchainSupport   = querySwapchainSupport(mPhysicalDevice, mSurfacePair.second);
    VkSurfaceFormatKHR      surfaceFormat      = chooseSwapchainSurfaceFormat(swapchainSupport.formats, requestedFormat);
    VkPresentModeKHR        surfacePresentMode = chooseSwapchainPresentMode(swapchainSupport.presentModes, mVsyncEnabled);
    VkExtent2D              surfaceExtent      = chooseSwapchainExtent(swapchainSupport.capabilities, resolution.x, resolution.y);

    std::uint32_t imageCount  = swapchainSupport.capabilities.minImageCount + 1;
    std::uint32_t bufferCount = static_cast<std::uint32_t>(mBufferStrategy);
    if (swapchainSupport.capabilities.maxImageCount > 0)
      imageCount = std::clamp(bufferCount, swapchainSupport.capabilities.minImageCount, swapchainSupport.capabilities.maxImageCount);

    // Clamp if the max image count is infinite.
    imageCount = std::min(bufferCount, 3u);

    // Get queues.
    QueueFamilyIndices indices = getQueueFamilies(std::pair{ mPhysicalDevice, mSurfacePair.second });
    std::array<std::uint32_t, 2> queueFamiles{
      indices.graphics.value(),
      indices.present.value()
    };

    // Create swapchain create info.
    VkSwapchainCreateInfoKHR swapchainCreateInfo;
    {
      swapchainCreateInfo.sType                   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
      swapchainCreateInfo.pNext                   = nullptr;
      swapchainCreateInfo.flags                   = 0;
      swapchainCreateInfo.surface                 = mSurfacePair.second;
      swapchainCreateInfo.minImageCount           = imageCount;
      swapchainCreateInfo.imageFormat             = surfaceFormat.format;
      swapchainCreateInfo.imageColorSpace         = surfaceFormat.colorSpace;
      swapchainCreateInfo.imageExtent             = surfaceExtent;
      swapchainCreateInfo.imageArrayLayers        = 1;
      swapchainCreateInfo.imageUsage              = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

      if (queueFamiles[0] != queueFamiles[1]) {
        swapchainCreateInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = queueFamiles.size();
        swapchainCreateInfo.pQueueFamilyIndices   = queueFamiles.data();
      } else {
        swapchainCreateInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 1;
        swapchainCreateInfo.pQueueFamilyIndices   = queueFamiles.data();
      }

      swapchainCreateInfo.preTransform            = swapchainSupport.capabilities.currentTransform;
      swapchainCreateInfo.compositeAlpha          = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
      swapchainCreateInfo.presentMode             = surfacePresentMode;
      swapchainCreateInfo.clipped                 = VK_TRUE;
      swapchainCreateInfo.oldSwapchain            = nullptr;
    }

    // Attempt to create swap chain.
    VkResult result = vkCreateSwapchainKHR(mLogicalDevice, &swapchainCreateInfo, nullptr, &mSwapChain);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create swap chain.");

    // Obtain swapchain images.
    vkGetSwapchainImagesKHR(mLogicalDevice, mSwapChain, &imageCount, nullptr);
    vkGetSwapchainImagesKHR(mLogicalDevice, mSwapChain, &imageCount, mImages.data());

    mFormat = surfaceFormat.format;
    mExtent = surfaceExtent;
  }

  void SwapChain::initializeImageViews() {
    // The creation info for the image views.
    VkImageViewCreateInfo ivCreateInfo;
    {
      ivCreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      ivCreateInfo.pNext                           = nullptr;
      ivCreateInfo.flags                           = 0;
      ivCreateInfo.image                           = nullptr;
      ivCreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
      ivCreateInfo.format                          = mFormat;
      ivCreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
      ivCreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
      ivCreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
      ivCreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
      ivCreateInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
      ivCreateInfo.subresourceRange.baseMipLevel   = 0;
      ivCreateInfo.subresourceRange.levelCount     = 1;
      ivCreateInfo.subresourceRange.baseArrayLayer = 0;
      ivCreateInfo.subresourceRange.layerCount     = 1;
    }

    // Store result.
    VkResult result = VK_SUCCESS;
    for (std::size_t index = 0; index < static_cast<std::size_t>(mBufferStrategy); index++) {
      // Set image.
      ivCreateInfo.image = mImages[index];

      // Perform creation.
      result = vkCreateImageView(mLogicalDevice, &ivCreateInfo, nullptr, &mImageViews[index]);
      if (result != VK_SUCCESS)
        throw std::runtime_error("Failed to create image views.");
    }
  }

  void SwapChain::initializeSemaphores() {
    VkSemaphoreCreateInfo createInfo;
    {
      createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
      createInfo.pNext = nullptr;
      createInfo.flags = 0;
    }

    VkResult result = vkCreateSemaphore(mLogicalDevice, &createInfo, nullptr, &mImageAvailable);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create image available semaphore.");

    result = vkCreateSemaphore(mLogicalDevice, &createInfo, nullptr, &mRenderFinished);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create render finished semaphore");
  }

  void SwapChain::rebuildSwapChain(const glm::uvec2& resolution) {
    // Wait for device availability.
    vkDeviceWaitIdle(mLogicalDevice);

    // Delete image views.
    for (auto view : mImageViews)
      vkDestroyImageView(mLogicalDevice, view, nullptr);

    // Delete swapchain entirely.
    vkDestroySwapchainKHR(mLogicalDevice, mSwapChain, nullptr);

    // Recreate.
    initializeSwapchain(resolution, static_cast<Format>(mFormat));
    initializeImageViews();
  }

}
