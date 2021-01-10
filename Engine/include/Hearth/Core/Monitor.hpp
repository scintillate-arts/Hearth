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
 * \file
 * \brief
 * \details
 */
#ifndef __INC_HEARTH_CORE_MONITOR_HPP__
#define __INC_HEARTH_CORE_MONITOR_HPP__ 1
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <glm/glm.hpp>

namespace Hearth {

  /**
   * \brief   Represents the video mode of a monitor.
   * \details Monitors typically come with multiple video modes that can be configured for optimal
   *          data presentation or setting the vertical sync speed of the monitor.
   */
  struct VideoMode final {
    /**
     * \brief     Compares this video mode object to another one.
     * \details   Where a normal lexographical comparer would just compare the bits of the structs,
     *            we opt to actually compare it's members.
     * \param[in] other The other video mode to compare to.
     * \return    True if the two have equal members.
     */
    bool operator==(const VideoMode& other) const noexcept {
      return resolution  == other.resolution &&
             redBits     == other.redBits    &&
             greenBits   == other.greenBits  &&
             blueBits    == other.blueBits   &&
             refreshRate == other.refreshRate;
    }

    /**
     * \brief     Compares this video mode object to another one.
     * \details   Where a normal lexographical comparer would just compare the bits of the structs,
     *            we opt to actually compare it's members.
     * \param[in] other The other video mode to compare to.
     * \return    True if the two do not have equal members.
     */
    bool operator!=(const VideoMode& other) const noexcept {
      return !operator==(other);
    }

    /**
     * \brief   The resolution of the video mode, in pixels.
     * \details This is usually the same as the size of the monitor, but it can be different if the
     *          monitor allows up or down scaling operations.
     */
    glm::uvec2 resolution;

    /**
     * \brief   The number of red bits this video mode supports.
     * \details This is typically going to be 8, but it may not be if the monitor is configured to
     *          use different linear, gamma, or other color space algorithms.
     */
    std::uint8_t redBits;

    /**
     * \brief   The number of green bits this video mode supports.
     * \details This is typically going to be 8, but it may not be if the monitor is configured to
     *          use different linear, gamma, or other color space algorithms.
     */
    std::uint8_t greenBits;

    /**
     * \brief   The number of blue bits this video mode supports.
     * \details This is typically going to be 8, but it may not be if the monitor is configured to
     *          use different linear, gamma, or other color space algorithms.
     */
    std::uint8_t blueBits;

    /**
     * \brief   The refresh rate of this video mode.
     * \details This may be a few different values over all the video modes available on a monitor.
     *          It may very if the monitor supports variable refresh rates, or multiple refresh
     *          rates.
     */
    std::uint16_t refreshRate;
  };

  /**
   * \brief   Represents a monitor connected to the computer.
   * \details Monitors are very important to a game engine. Monitors can sometime have technologies
   *          in them that allows us to optimize their usage, or graphics drivers can interoperate
   *          with them in manners that provide some optimization if we enable it. They also are
   *          useful when determining window sizes, positions, fullscreen state, etc.
   * \todo    Write tests to confirm monitor functions work as expected.
   */
  class Monitor final {
    // This friend class is responsible for initializing the monitors of a platform.
    friend class MonitorInitializer;

  private:
    /**
     * \brief   Default constructs this object.
     * \details We don't need to manage anything directly, so we don't need to do anything.
     */
    Monitor() noexcept = default;

  public:
    /**
     * \brief   Default destructs this object.
     * \details Since we don't manage anything directly, we can let the runtime clean this up.
     */
    ~Monitor() noexcept = default;

    /**
     * \brief     Default copy constructs this object.
     * \details   Copies all the members of the other object into this object lexographically.
     * \param[in] other The monitor to copy data from.
     */
    Monitor(const Monitor& other) noexcept = default;

    /**
     * \brief     Default move constructs this object.
     * \details   Moves all the members of the other object into this object lexographically.
     * \param[in] other The monitor to move data from.
     */
    Monitor(Monitor&& other) noexcept = default;

    /**
     * \brief     Default copy assignment operator.
     * \details   Copies all the members of the other object into this object lexographically.
     * \param[in] other The monitor to copy data from.
     * \returns   This object with the copied data.
     */
    Monitor& operator=(const Monitor& other) noexcept = default;

    /**
     * \brief     Default move assignment operator.
     * \details   Swaps all the members of the other object with this object lexographically.
     * \param[in] other The monitor to swap data with.
     * \returns   This object with the swapped data.
     */
    Monitor& operator=(Monitor&& other) noexcept = default;

  public:
    /**
     * \brief   The primary monitor object.
     * \details When the engine starts up, all the available monitors on a system will be created.
     *          The primary is the very first of those created.
     * \returns The primary monitor.
     */
    static const Monitor& primary() noexcept;

    /**
     * \brief   Gets all the monitors connected to a system.
     * \details When the engine starts up, all the available monitors on a system will be created.
     *          This function will return the vector that they are stored in. The vector cannot and
     *          should not be modified.
     * \returns All the monitors that are available.
     */
    static const std::vector<Monitor>& allAvailable() noexcept;

  public:
    /**
     * \brief   Checks whether or not this is the primary monitor.
     * \details The primary monitor is the first monitor object created when the engine starts up.
     *          By checking this against that one, we can verify it's validity as the primary.
     * \returns True if this is the primary monitor, false otherwise.
     */
    bool isPrimary() const noexcept;

    /**
     * \brief   Gets the device name of this monitor.
     * \details This name is retrieved from the operating system and seems to be operating system
     *          dependent and specific.
     * \returns The device name of this monitor as specified by the operating system.
     */
    std::wstring_view deviceName() const noexcept;

    /**
     * \brief   Gets the adapter name for this monitor.
     * \details This name is retried from the operating system, it's usually the name of the
     *          graphics card the monitor is connected to.
     * \returns The context string of this monitor.
     */
    std::wstring_view adapterName() const noexcept;

    /**
     * \brief   Gets the actual name of the monitor.
     * \details This name is retrieved once again, from the operating system, and seems to be the
     *          actual name of the monitor as the operating system knows it.
     * \return  The name of the monitor.
     */
    std::wstring_view name() const noexcept;

    /**
     * \brief   Gets the size of this monitor in millimeters.
     * \details Operating systems seem to be able to get us some interesting information about the
     *          monitors attached to the machine. This is likely because it's provided, somewhat,
     *          standard in drivers for monitors. Regardless, we are able to get the size of a
     *          monitor in millimeters and this seems useful for some applications. So we provide a
     *          way to get it.
     * \returns The size of this monitor in millimeters as specified by the operating system.
     */
    glm::uvec2 sizeInMillimeters() const noexcept;

    /**
     * \brief   Gets the size of this monitor in pixels.
     * \details This property of a monitor is more reasonable to be present. The operating system
     *          itself must know how large the monitor is in pixels to perform graphics operations.
     *          So thankfully, we are able to get that information.
     * \returns The size of the monitor in pixels as specified by the operating system.
     */
    glm::uvec2 sizeInPixels() const noexcept;

    /**
     * \brief   Gets the left-top screen position of the monitor.
     * \details On most operating systems, monitors can be arranged in a sort of virtual screen
     *          space. It's useful being able to know the position of a monitor in that space, so we
     *          can do some specific operations, such as fullscreen.
     * \returns The location of this monitor's left-top corner in virtual screen space.
     */
    glm::ivec2 screenPosition() const noexcept;

    /**
     * \brief   Gets the current video mode that is set for this monitor.
     * \details The video mode that the monitor is currently using, this object cannot be modified.
     * \returns The current video mode for this monitor.
     */
    const VideoMode& videoMode() const noexcept;

    /**
     * \brief   Gets all the available video modes for this monitor.
     * \details All the video modes for a monitor are polled when the monitor is created. They
     *          become stored on the monitor and are easily pollable.
     * \returns A vector containing all the available video modes of this monitor.
     */
    const std::vector<VideoMode>& availableModes() const noexcept;

  private:
    /**
     * \brief   A vector listing all the available modes on this monitor.
     * \details This will be populated when the monitor gets constructed.
     */
    std::vector<VideoMode> mAvailableModes;

    /**
     * \brief   A pointer to the current video mode this monitor is using.
     * \details We track this for convenience, since we need to be able to return it and swap it out.
     *
     * FIXME: This is currently broken. It cannot deference the proper memory.
     */
    const VideoMode* mCurrentMode;

    /**
     * \brief   The device name for this monitor.
     * \details This value is operating system dependent.
     */
    std::wstring mDeviceName;

    /**
     * \brief   The adapter name for the display device.
     * \details This is typically the name of the graphics card this monitor is attached to.
     */
    std::wstring mAdapterName;

    /**
     * \brief   The name of the monitor itself.
     * \details This name is obtained through the operating system. It likely comes from the monitor
     *          drive on the operating system. It will describe the actual name of the monitor.
     */
    std::wstring mMonitorName;

    /**
     * \brief   The size of this monitor in millimeters.
     * \details This is provided by the operating system, likely from a driver. It can be used to
     *          perform some mathematical operations that we can't forsee but guarantee will happen.
     *          (People are weird)
     */
    glm::uvec2 mSizeInMillimeters;

    /**
     * \brief   The size of this monitor in pixels.
     * \details This is a very important value. It will help with making the window fullscreen or
     *          maximized, or borderless-fullscreen.
     */
    glm::uvec2 mSizeInPixels;

    /**
     * \brief   The left-top position of the monitor in the virtual screen space.
     * \details When there are multiple monitors on a system, their orientation and arrangement make
     *          up the virtual screen space. Screen space is measured from the left-top. A monitor
     *          at (0,0) screen coordinates is typically considered the primary monitor.
     */
    glm::ivec2 mScreenPosition;
  };

}

#endif /* __INC_HEARTH_CORE_MONITOR_HPP__ */
