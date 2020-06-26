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
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>
#include <hearth/version.hpp>
#include <hearth/graphics/rdrctx.hpp>
#if defined(HAPI_WINDOWS_OS)
#  include "../win32/winapi.hpp"
#  include "../win32/window.hpp"
#  include <vulkan/vulkan_win32.h>
#endif
#include "queuefamily.hpp"

namespace HAPI_NAMESPACE_NAME::gfx {

#if defined(HAPI_DEBUG)
  constexpr std::size_t gRequiredInstanceExtensionsCount = 3;
#else
  constexpr std::size_t gRequiredInstanceExtensionsCount = 2;
#endif

  // The required device extensions.
  constexpr std::array<const char*, gRequiredInstanceExtensionsCount> gRequiredInstanceExtensions {
    VK_KHR_SURFACE_EXTENSION_NAME,

  #if defined(HAPI_WINDOWS_OS)
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
  #endif

  #if defined(HAPI_DEBUG)
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
  #endif
  };

  // The required device extensions.
  constexpr std::array<const char*, 1> gRequiredDeviceExtensions {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

#if defined(HAPI_DEBUG)
  constexpr std::array<const char*, 1> gValidationLayers {
    "VK_LAYER_KHRONOS_validation"
  };

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    [[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT        messageType,
               const VkDebugUtilsMessengerCallbackDataEXT*  pCallbackData,
    [[maybe_unused]] void*                                  pUserData
  ) noexcept {
    std::cerr << "Vulkan Validation Layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
  }

  VkResult createDebugUtilsMessengerEXT(
          VkInstance                          instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks*              pAllocator,
          VkDebugUtilsMessengerEXT*           pDebugMessenger
  ) {
    // Attempt to load function on first call.
    static auto _vk__func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT")
    );

    // Double check that the function was loaded.
    if (_vk__func == nullptr)
      throw std::runtime_error("Unable to load vkCreateDebugUtilsMessengerEXT");

    // Call function.
    return (*_vk__func)(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }

  void destroyDebugUtilsMessengerEXT(
          VkInstance               instance,
          VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks*   pAllocator
  ) {
    // Attempt to load function on first call.
    static auto _vk__func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT")
    );

    // Double check that the function was loaded.
    if (_vk__func == nullptr)
      throw std::runtime_error("Unable to load vkDestroyDebugUtilsMessengerEXT");

    // Call function.
    (*_vk__func)(instance, debugMessenger, pAllocator);
  }
#endif

  static bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) noexcept {
    std::uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string_view> requiredExtensions(gRequiredDeviceExtensions.begin(), gRequiredDeviceExtensions.end());
    for (const auto& extension: availableExtensions)
      requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
  }

  static bool deviceIsSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) noexcept {
    QueueFamilyIndices indices             = getQueueFamilies(std::pair{ physicalDevice, surface });
    bool               extensionsSupported = checkDeviceExtensionSupport(physicalDevice);
    return indices.isComplete() && extensionsSupported;
  }

  RenderContext::RenderContext() noexcept
    : mGraphicsQueuePair(std::pair{ nullptr, 0 })
    , mPresentQueuePair(std::pair{ nullptr, 0 })
    , mInstance(nullptr)
    , mSurface(nullptr)
    , mPhysicalDevice(nullptr)
    , mLogicalDevice(nullptr)
  #if defined(HAPI_DEBUG)
    , mDebugMessenger(nullptr)
  #endif
  { }

  RenderContext::RenderContext(const CreateInfo& createInfo)
    : mGraphicsQueuePair(std::pair{ nullptr, 0 })
    , mPresentQueuePair(std::pair{ nullptr, 0 })
    , mInstance(nullptr)
    , mSurface(nullptr)
    , mPhysicalDevice(nullptr)
    , mLogicalDevice(nullptr)
  #if defined(HAPI_DEBUG)
    , mDebugMessenger(nullptr)
  #endif
  {
    initializeInstance(createInfo.appName, createInfo.appVersion);
    initializeSurface(createInfo.surface);
    pickPhysicalDevice();
    initializeLogicalDevice();
  }

  RenderContext::~RenderContext() noexcept {
    // Don't process this because it was either swapped or it failed construction.
    if (mInstance == nullptr)
      return;

    // Wait for device.
    vkDeviceWaitIdle(mLogicalDevice);
    vkDestroyDevice(mLogicalDevice, nullptr);
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);

  #if defined(HAPI_DEBUG)
    destroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
  #endif

    vkDestroyInstance(mInstance, nullptr);
  }

  RenderContext::RenderContext(RenderContext&& other) noexcept
    : mGraphicsQueuePair(std::move(other.mGraphicsQueuePair))
    , mPresentQueuePair(std::move(other.mPresentQueuePair))
    , mInstance(std::move(other.mInstance))
    , mSurface(std::move(other.mSurface))
    , mPhysicalDevice(std::move(other.mPhysicalDevice))
    , mLogicalDevice(std::move(other.mLogicalDevice))
  #if defined(HAPI_DEBUG)
    , mDebugMessenger(std::move(other.mDebugMessenger))
  #endif
  {
    // Ensuring.
    other.mGraphicsQueuePair = std::pair{ nullptr, 0 };
    other.mPresentQueuePair  = std::pair{ nullptr, 0 };
    other.mInstance          = nullptr;
    other.mSurface           = nullptr;
    other.mPhysicalDevice    = nullptr;
    other.mLogicalDevice     = nullptr;
  #if defined(HAPI_DEBUG)
    other.mDebugMessenger    = nullptr;
  #endif
  }

  RenderContext& RenderContext::operator=(RenderContext&& other) noexcept {
    std::swap(mGraphicsQueuePair, other.mGraphicsQueuePair);
    std::swap(mPresentQueuePair,  other.mPresentQueuePair);
    std::swap(mInstance,          other.mInstance);
    std::swap(mSurface,           other.mSurface);
    std::swap(mPhysicalDevice,    other.mPhysicalDevice);
    std::swap(mLogicalDevice,     other.mLogicalDevice);
  #if defined(HAPI_DEBUG)
    std::swap(mDebugMessenger, other.mDebugMessenger);
  #endif
    return *this;
  }

  VkInstance RenderContext::environment() const noexcept {
    return mInstance;
  }

  void RenderContext::initializeInstance(std::string_view appName, std::uint32_t appVersion) {
    // Get the application information.
    VkApplicationInfo appInfo;
    {
      appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pNext              = nullptr;
      appInfo.pApplicationName   = appName.data();
      appInfo.applicationVersion = appVersion;
      appInfo.pEngineName        = "Hearth Engine";
      appInfo.engineVersion      = Version::current;
      appInfo.apiVersion         = VK_API_VERSION_1_1;
    }

    // Check extension properties.
    std::uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());

    // Check that the instance can support surfaces.
    bool generalSurfaceSupport  = false;
    bool platformSurfaceSupport = false;
    for (std::size_t index = 0; index < extensionCount; index++) {
      // Get extension name.
      const char* extensionName = extensionProperties[index].extensionName;

      // Check general surface support.
      if (std::strcmp(extensionName, gRequiredInstanceExtensions[0]) == 0)
        generalSurfaceSupport = true;
      else if (std::strcmp(extensionName, gRequiredInstanceExtensions[1]) == 0)
        platformSurfaceSupport = true;
    }

    // Check that surfaces are supported.
    if (!generalSurfaceSupport || !platformSurfaceSupport)
      throw std::runtime_error("Surfaces are not supported by vulkan driver!");

  #if defined(HAPI_DEBUG)
    VkDebugUtilsMessengerCreateInfoEXT dbMessengerCreateInfo;
    {
      dbMessengerCreateInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      dbMessengerCreateInfo.pNext           = nullptr;
      dbMessengerCreateInfo.flags           = 0;
      dbMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                              VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                              VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      dbMessengerCreateInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      dbMessengerCreateInfo.pfnUserCallback = &debugCallback;
      dbMessengerCreateInfo.pUserData       = nullptr;
    }
  #endif

    // Get instance create info.
    VkInstanceCreateInfo instCreateInfo;
    {
      instCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    #if defined(HAPI_DEBUG)
      instCreateInfo.pNext                   = static_cast<void*>(&dbMessengerCreateInfo);
    #endif
      instCreateInfo.flags                   = 0;
      instCreateInfo.pApplicationInfo        = &appInfo;
    #if defined(HAPI_DEBUG)
      instCreateInfo.enabledLayerCount       = static_cast<std::uint32_t>(gValidationLayers.size());
      instCreateInfo.ppEnabledLayerNames     = gValidationLayers.data();
    #else
      instCreateInfo.enabledLayerCount       = 0;
      instCreateInfo.ppEnabledLayerNames     = nullptr;
    #endif
      instCreateInfo.enabledExtensionCount   = static_cast<std::uint32_t>(gRequiredInstanceExtensionsCount);
      instCreateInfo.ppEnabledExtensionNames = gRequiredInstanceExtensions.data();
    }

    // Try to create vulkan instance.
    VkResult result = vkCreateInstance(&instCreateInfo, nullptr, &mInstance);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create vulkan instance.");

  #if defined(HAPI_DEBUG)
    result = createDebugUtilsMessengerEXT(mInstance, &dbMessengerCreateInfo, nullptr, &mDebugMessenger);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create debug messenger.");
  #endif
  }

  void RenderContext::initializeSurface(Window* surface) {
    // Make sure surface isn't null.
    if (surface == nullptr)
      throw std::runtime_error("Surface cannot be null when creating a render context.");

    VkResult result;
  #if defined(HAPI_WINDOWS_OS)
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
    {
      surfaceCreateInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
      surfaceCreateInfo.pNext     = nullptr;
      surfaceCreateInfo.flags     = 0;
      surfaceCreateInfo.hinstance = GetModuleHandleW(nullptr);
      surfaceCreateInfo.hwnd      = static_cast<HWND>(static_cast<native::Win32Window*>(surface)->handle());
    }

    result = vkCreateWin32SurfaceKHR(mInstance, &surfaceCreateInfo, nullptr, &mSurface);
  #endif

    // Check if surface creation succeeded.
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create vulkan surface.");
  }

  void RenderContext::pickPhysicalDevice() {
    std::uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr);

    std::vector<VkPhysicalDevice> availableDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, availableDevices.data());

    // Check if any of the devices are suitable for our needs.
    for (std::size_t index = 0; index < physicalDeviceCount; index++) {
      // Get device.
      const auto& device = availableDevices[index];

      // Check if device is suitable.
      if (deviceIsSuitable(device, mSurface))
        mPhysicalDevice = device;
    }

    // Check that we did find a suitable device.
    if (mPhysicalDevice == VK_NULL_HANDLE)
      throw std::runtime_error("Failed to find a suitable GPU.");
  }

  void RenderContext::initializeLogicalDevice() {
    // Get queue family indices for our physical device.
    QueueFamilyIndices indices = getQueueFamilies(std::pair{ mPhysicalDevice, mSurface });

    // A place to store our Queue creation info. We can support up to 2 currently.
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<std::uint32_t> uniqueFamilies = {
      indices.graphics.value(),
      indices.present.value()
    };

    // Prepare population.
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo qCreateInfo;
    {
      qCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      qCreateInfo.pNext            = nullptr;
      qCreateInfo.flags            = 0;
      qCreateInfo.pQueuePriorities = &queuePriority;
    }

    // Populate queueCreateInfos.
    for (auto queueFamily: uniqueFamilies) {
      qCreateInfo.queueFamilyIndex = queueFamily;
      qCreateInfo.queueCount       = 1;
      queueCreateInfos.emplace_back(qCreateInfo);
    }

    // Device features.
    VkPhysicalDeviceFeatures deviceFeatures { };

    // Our device create info.
    VkDeviceCreateInfo deviceCreateInfo;
    {
      deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
      deviceCreateInfo.pNext                   = nullptr;
      deviceCreateInfo.flags                   = 0;
      deviceCreateInfo.queueCreateInfoCount    = static_cast<std::uint32_t>(queueCreateInfos.size());
      deviceCreateInfo.pQueueCreateInfos       = queueCreateInfos.data();
    #if defined(HAPI_DEBUG)
      deviceCreateInfo.enabledLayerCount       = static_cast<std::uint32_t>(gValidationLayers.size());
      deviceCreateInfo.ppEnabledLayerNames     = gValidationLayers.data();
    #else
      deviceCreateInfo.enabledLayerCount       = 0;
      deviceCreateInfo.ppEnabledLayerNames     = nullptr;
    #endif /* HAPI_DEBUG */
      deviceCreateInfo.enabledExtensionCount   = static_cast<std::uint32_t>(gRequiredDeviceExtensions.size());
      deviceCreateInfo.ppEnabledExtensionNames = gRequiredDeviceExtensions.data();
      deviceCreateInfo.pEnabledFeatures        = &deviceFeatures;
    }

    // Attempt to create logical device.
    VkResult result = vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice);
    if (result != VK_SUCCESS)
      throw std::runtime_error("Failed to create logical device.");

    // Get device queues. Graphics is always in queue 0.
    vkGetDeviceQueue(mLogicalDevice, indices.graphics.value(), 0, &mGraphicsQueuePair.first);
    vkGetDeviceQueue(mLogicalDevice, indices.present.value(), 0, &mPresentQueuePair.first);

    // Set indices.
    mGraphicsQueuePair.second = indices.graphics.value();
    mPresentQueuePair.second  = indices.present.value();
  }

}
