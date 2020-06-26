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
#include <optional>
#include <utility>
#include <vector>
#include <hearth/config.hpp>
#include <vulkan/vulkan.h>

namespace HAPI_NAMESPACE_NAME::gfx {

  //! \brief Responsible for providing the queue family information for a physical device, surface
  //!        pair.
  struct QueueFamilyIndices final {
    //! \brief The graphics queue family.
    std::optional<std::uint32_t> graphics;

    //! \brief The present queue family.
    std::optional<std::uint32_t> present;

    /*!
     * \brief  Checks if the queue families are complete.
     * \return True if all queue families have a value.
     */
    bool isComplete() const noexcept {
      return graphics.has_value() && present.has_value();
    }
  };

  /*!
   * \brief     Gets the queue families for the given physical device, surface pair.
   * \param[in] pdsPair The pair of physical device and surface for gettting queue families.
   * \return    The found queue families.
   */
  inline QueueFamilyIndices getQueueFamilies(const std::pair<VkPhysicalDevice, VkSurfaceKHR>& pdsPair) noexcept {
    QueueFamilyIndices indices;

    std::uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pdsPair.first, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> availableFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(pdsPair.first, &queueFamilyCount, availableFamilies.data());

    // Find suitable queue families.
    for (std::size_t index = 0; index < queueFamilyCount; index++) {
      // Get next queue family properties.
      const VkQueueFamilyProperties& queueFamily = availableFamilies[index];

      // Check if this queue family supports graphics.
      bool supportsGraphics = queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;
      if (supportsGraphics)
        indices.graphics = index;

      // Check presentation support.
      VkBool32 supportsPresent = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(pdsPair.first, index, pdsPair.second, &supportsPresent);
      if (supportsPresent)
        indices.present = index;

      if (indices.isComplete())
        break;
    }

    // Finally return.
    return indices;
  }

}
