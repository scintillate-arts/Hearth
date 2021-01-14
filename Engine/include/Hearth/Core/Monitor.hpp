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
/**
 * \file  Monitor.hpp
 * \brief This file contains the definition for the Monitor class and its members.
 */
#ifndef __INC_HEARTH_CORE_MONITOR_HPP__
#define __INC_HEARTH_CORE_MONITOR_HPP__ 1
#include <cstdint>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Forward.hpp"

namespace Hearth::Core {
/**
 * \struct  VideoMode
 * \brief   Represents the video mode of a monitor.
 * \details Monitors typically come with multiple video modes that can be configured for optimal
 *          data presentation or setting the vertical sync speed of the monitor.
 */
/**
 * \fn        VideoMode::operator==(const VideoMode&)
 * \brief     Compares this video mode object to another one.
 * \details   Where a normal lexographical comparer would just compare the bits of the structs,
 *            we opt to actually compare it's members.
 * \param[in] other The other video mode to compare to.
 * \return    True if the two have equal members.
 */
/**
 * \fn        VideoMode::operator!=(const VideoMode&)
 * \brief     Compares this video mode object to another one.
 * \details   Where a normal lexographical comparer would just compare the bits of the structs,
 *            we opt to actually compare it's members.
 * \param[in] other The other video mode to compare to.
 * \return    True if the two do not have equal members.
 */
/**
 * \property VideoMode::resolution
 * \brief    The resolution of the video mode, in pixels.
 * \details  This is usually the same as the size of the monitor, but it can be different if the
 *           monitor allows up or down scaling operations.
 */
/**
 * \property VideoMode::redBits
 * \brief    The number of red bits this video mode supports.
 * \details  This is typically going to be 8, but it may not be if the monitor is configured to
 *           use different linear, gamma, or other color space algorithms.
 */
/**
 * \property VideoMode::greenBits
 * \brief    The number of green bits this video mode supports.
 * \details  This is typically going to be 8, but it may not be if the monitor is configured to
 *           use different linear, gamma, or other color space algorithms.
 */
/**
 * \property VideoMode::blueBits
 * \brief    The number of blue bits this video mode supports.
 * \details  This is typically going to be 8, but it may not be if the monitor is configured to
 *           use different linear, gamma, or other color space algorithms.
 */
/**
 * \property VideoMode::refreshRate
 * \brief    The refresh rate of this video mode.
 * \details  This may be a few different values over all the video modes available on a monitor.
 *           It may very if the monitor supports variable refresh rates, or multiple refresh
 *           rates.
 */
/**
 * \typedef VideoModeView
 * \brief   A type representing the view of a video mode object.
*/
/**
 * \fn      Monitor::Monitor()
 * \brief   Default constructs this object.
 * \details We don't need to manage anything directly, so we don't need to do anything.
 */
/**
 * \fn      Monitor::~Monitor()
 * \brief   Default destructs this object.
 * \details Since we don't manage anything directly, we can let the runtime clean this up.
 */
/**
 * \fn        Monitor::Monitor(const Monitor&)
 * \brief     Default copy constructs this object.
 * \details   Copies all the members of the other object into this object lexographically.
 * \param[in] other The monitor to copy data from.
 */
/**
 * \fn        Monitor::Monitor(Monitor&&)
 * \brief     Default move constructs this object.
 * \details   Moves all the members of the other object into this object lexographically.
 * \param[in] other The monitor to move data from.
 */
/**
 * \fn        Monitor::operator=(const Monitor&)
 * \brief     Default copy assignment operator.
 * \details   Copies all the members of the other object into this object lexographically.
 * \param[in] other The monitor to copy data from.
 * \returns   This object with the copied data.
 */
/**
 * \fn        Monitor::operator=(Monitor&&)
 * \brief     Default move assignment operator.
 * \details   Swaps all the members of the other object with this object lexographically.
 * \param[in] other The monitor to swap data with.
 * \returns   This object with the swapped data.
 */
/**
 * \fn      Monitor::isPrimary()
 * \brief   Checks whether or not this is the primary monitor.
 * \details The primary monitor is the first monitor object created when the engine starts up.
 *          By checking this against that one, we can verify it's validity as the primary.
 * \returns True if this is the primary monitor, false otherwise.
 */
/**
 * \fn      Monitor::deviceName()
 * \brief   Gets the device name of this monitor.
 * \details This name is retrieved from the operating system and seems to be operating system
 *          dependent and specific.
 * \returns The device name of this monitor as specified by the operating system.
 */
/**
 * \fn      Monitor::adapterName()
 * \brief   Gets the adapter name for this monitor.
 * \details This name is retried from the operating system, it's usually the name of the
 *          graphics card the monitor is connected to.
 * \returns The context string of this monitor.
 */
/**
 * \fn      Monitor::name()
 * \brief   Gets the actual name of the monitor.
 * \details This name is retrieved once again, from the operating system, and seems to be the
 *          actual name of the monitor as the operating system knows it.
 * \return  The name of the monitor.
 */
/**
 * \fn      Monitor::sizeInMillimeters()
 * \brief   Gets the size of this monitor in millimeters.
 * \details Operating systems seem to be able to get us some interesting information about the
 *          monitors attached to the machine. This is likely because it's provided, somewhat,
 *          standard in drivers for monitors. Regardless, we are able to get the size of a
 *          monitor in millimeters and this seems useful for some applications. So we provide a
 *          way to get it.
 * \returns The size of this monitor in millimeters as specified by the operating system.
 */
/**
 * \fn      Monitor::sizeInPixels()
 * \brief   Gets the size of this monitor in pixels.
 * \details This property of a monitor is more reasonable to be present. The operating system
 *          itself must know how large the monitor is in pixels to perform graphics operations.
 *          So thankfully, we are able to get that information.
 * \returns The size of the monitor in pixels as specified by the operating system.
 */
/**
 * \fn      Monitor::screenPosition()
 * \brief   Gets the left-top screen position of the monitor.
 * \details On most operating systems, monitors can be arranged in a sort of virtual screen
 *          space. It's useful being able to know the position of a monitor in that space, so we
 *          can do some specific operations, such as fullscreen.
 * \returns The location of this monitor's left-top corner in virtual screen space.
 */
/**
 * \fn      Monitor::videoMode()
 * \brief   Gets the current video mode that is set for this monitor.
 * \details The video mode that the monitor is currently using, this object cannot be modified.
 * \returns The current video mode for this monitor.
 */
/**
 * \fn      Monitor::availableModes()
 * \brief   Gets all the available video modes for this monitor.
 * \details All the video modes for a monitor are polled when the monitor is created. They
 *          become stored on the monitor and are easily pollable.
 * \returns A vector containing all the available video modes of this monitor.
 */
/**
 * \fn      Monitor::primary()
 * \brief   The primary monitor object.
 * \details When the engine starts up, all the available monitors on a system will be created.
 *          The primary is the very first of those created.
 * \returns The primary monitor.
 */
/**
 * \fn      Monitor::allAvailable()
 * \brief   Gets all the monitors connected to a system.
 * \details When the engine starts up, all the available monitors on a system will be created.
 *          This function will return the vector that they are stored in. The vector cannot and
 *          should not be modified.
 * \returns All the monitors that are available.
 */

  struct VideoMode final {
    constexpr
    bool operator==(const VideoMode& other) const noexcept {
      return resolution  == other.resolution &&
             redBits     == other.redBits    &&
             greenBits   == other.greenBits  &&
             blueBits    == other.blueBits   &&
             refreshRate == other.refreshRate;
    }

    constexpr
    bool operator!=(const VideoMode& other) const noexcept {
      return !operator==(other);
    }

    glm::uvec2    resolution;
    std::uint8_t  redBits;
    std::uint8_t  greenBits;
    std::uint8_t  blueBits;
    std::uint16_t refreshRate;
  };

  using VideoModeView = Memory::View<VideoMode>;

  class Monitor final {
    // This friend class is responsible for initializing the monitors of a platform.
    friend class MonitorInitializer;

  private:
    Monitor() noexcept = default;

  public:
    ~Monitor() noexcept = default;
    Monitor(const Monitor& other) noexcept = default;
    Monitor(Monitor&& other) noexcept = default;
    Monitor& operator=(const Monitor& other) noexcept = default;
    Monitor& operator=(Monitor&& other) noexcept = default;

  public:
    bool isPrimary() const noexcept;
    WideStringView deviceName() const noexcept;
    WideStringView adapterName() const noexcept;
    WideStringView name() const noexcept;
    glm::uvec2 sizeInMillimeters() const noexcept;
    glm::uvec2 sizeInPixels() const noexcept;
    glm::ivec2 screenPosition() const noexcept;
    const VideoMode& videoMode() const noexcept;
    const std::vector<VideoMode>& availableModes() const noexcept;

  public:
    static const Monitor& primary() noexcept;
    static const std::vector<Monitor>& allAvailable() noexcept;

  private:
    std::vector<VideoMode> m_availableModes;
    VideoModeView          m_currentMode;
    WideString             m_deviceName;
    WideString             m_adapterName;
    WideString             m_monitorName;
    glm::uvec2             m_sizeInMillimeters;
    glm::uvec2             m_sizeInPixels;
    glm::ivec2             m_screenPosition;
  };

}

#endif /* __INC_HEARTH_CORE_MONITOR_HPP__ */
