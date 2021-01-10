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
#include <Hearth/Config.hpp>
#include <Hearth/Core/Monitor.hpp>
#if HEARTH_WINDOWS_PLATFORM
#include "WinAPI.hpp"

namespace Hearth {

  // The available monitors on the platform.
  static std::vector<Monitor> sPlatformConnectedMonitors;

  // Constructs each of the monitors for this platform.
  //! \todo Adapt this to perform polling, so that in the window procedure we can handle display connects and disconnects.
  //! \todo Check for disconnected displays.
  class MonitorInitializer {
  public:
    // Initiates monitor construction.
    MonitorInitializer() {
      createMonitors();
    }

    // Not sure if this needs to do anything yet.
    ~MonitorInitializer() noexcept = default;

  private:
    // Distributes the proper bpp.
    inline static void splitbpp(std::uint8_t bpp, std::uint8_t& red, std::uint8_t& green, std::uint8_t& blue) noexcept {
      std::uint8_t delta;

      // Only 3 channels available.
      if (bpp == 32)
        bpp = 24;

      // Convert.
      red = green = blue = bpp / 3;
      delta = bpp - (red * 3);
      if (delta >= 1)
        green = green + 1;
      if (delta == 2)
        red = red + 1;
    }

    // Enumerates and assigns the video modes for the given monitor.
    inline static void enumerateVideoModes(const DISPLAY_DEVICEW& adapter, const DISPLAY_DEVICEW& display, Monitor& monitor) noexcept {
      // Fill in all video modes for this monitor.
      auto vmIndex            = 0u;
      auto vidMode            = VideoMode{};
      auto displayMode        = DEVMODEW{};
           displayMode.dmSize = sizeof(DEVMODEW);
      BOOL pruned             = adapter.StateFlags & DISPLAY_DEVICE_MODESPRUNED;
      while (EnumDisplaySettingsExW(adapter.DeviceName, vmIndex++, &displayMode, 0) == TRUE) {
        // Skip modes with less than 15 BPP.
        if (displayMode.dmBitsPerPel < 15)
          continue;

        // Fill out video mode.
        vidMode.resolution  = glm::uvec2{ displayMode.dmPelsWidth, displayMode.dmPelsHeight };
        vidMode.refreshRate = displayMode.dmDisplayFrequency;
        splitbpp(displayMode.dmBitsPerPel, vidMode.redBits, vidMode.greenBits, vidMode.blueBits);

        // See if the video mode is already contained.
        auto itr = std::find(monitor.mAvailableModes.begin(), monitor.mAvailableModes.end(), vidMode);
        if (itr != monitor.mAvailableModes.end())
          continue;

        // Make sure this mode can actually be used.
        if (pruned) {
          if (ChangeDisplaySettingsExW(monitor.mDeviceName.c_str(), &displayMode, nullptr, CDS_TEST, 0) != DISP_CHANGE_SUCCESSFUL)
            continue;
        }

        monitor.mAvailableModes.emplace_back(vidMode);
      }
    }

    // Enumerate the displays.
    inline static void enumerateDisplays(const DISPLAY_DEVICEW& adapter) noexcept {
      // Proceed to get information about the displays.
      auto displayIndex           = 0u;
      auto displayInfo            = DISPLAY_DEVICEW{};
           displayInfo.cb         = sizeof(DISPLAY_DEVICEW);
      auto displaySettings        = DEVMODEW{};
           displaySettings.dmSize = sizeof(DEVMODEW);
      while (EnumDisplayDevicesW(adapter.DeviceName, displayIndex++, &displayInfo, 0) == TRUE) {
        // Continue passed these.
        if (!(displayInfo.StateFlags & DISPLAY_DEVICE_ACTIVE))
          continue;

        // Create new monitor.
        Monitor monitor;
                monitor.mDeviceName  = adapter.DeviceName;
                monitor.mAdapterName = adapter.DeviceString;
                monitor.mMonitorName = displayInfo.DeviceString;

        // This device context will help us get device capabilities.
        // When we specify the driver and device name this way, it will actually give us a device
        // context that is compatible and probably references the original. This allows us to use it
        // in place of the original in functions like GetDeviceCaps.
        HDC dc = CreateDCW(L"DISPLAY", adapter.DeviceName, nullptr, nullptr);

        // Get monitor settings.
        EnumDisplaySettingsExW(adapter.DeviceName, ENUM_CURRENT_SETTINGS, &displaySettings, EDS_ROTATEDMODE);
        monitor.mSizeInPixels   = glm::uvec2{ displaySettings.dmPelsWidth,  displaySettings.dmPelsHeight };
        monitor.mScreenPosition = glm::ivec2{ displaySettings.dmPosition.x, displaySettings.dmPosition.y };
        if (IsWindows8Point1OrGreater())
          monitor.mSizeInMillimeters = glm::uvec2{ GetDeviceCaps(dc, HORZSIZE), GetDeviceCaps(dc, VERTSIZE) };
        else
          monitor.mSizeInMillimeters = glm::uvec2{
            displaySettings.dmPelsWidth  * 25.4f / GetDeviceCaps(dc, LOGPIXELSX),
            displaySettings.dmPelsHeight * 25.4f / GetDeviceCaps(dc, LOGPIXELSY)
          };

        // Delete the device context that we created.
        DeleteDC(dc);

        // Enumerate video modes.
        enumerateVideoModes(adapter, displayInfo, monitor);

        // TODO: handle disconnected monitors.
        // Add monitor to connected monitors.
        sPlatformConnectedMonitors.emplace_back(monitor);
      }
    }

    // Enumerate the adapters.
    inline static void enumerateAdapters() noexcept {
      // Proceed to get information about the adapters.
      auto adapterIndex   = 0u;
      auto adapterInfo    = DISPLAY_DEVICEW{};
           adapterInfo.cb = sizeof(DISPLAY_DEVICEW);
      bool isPrimary      = false;
      while (EnumDisplayDevicesW(nullptr, adapterIndex++, &adapterInfo, 0) == TRUE) {
        // Continue passed these.
        if (!(adapterInfo.StateFlags & DISPLAY_DEVICE_ACTIVE))
          continue;

        // If this is the primary device, insert first.
        if (adapterInfo.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
          isPrimary = true;

        // Enumerate the displays.
        enumerateDisplays(adapterInfo);
      }
    }

    // Proceeds with creating monitors.
    inline static void createMonitors() noexcept {
      // Kick everything off.
      enumerateAdapters();
    }
  };

  // The monitor initializer.
  static MonitorInitializer sMonitorInitializer;

  const Monitor& Monitor::primary() noexcept {
    return sPlatformConnectedMonitors[0];
  }

  const std::vector<Monitor>& Monitor::allAvailable() noexcept {
    return sPlatformConnectedMonitors;
  }

  bool Monitor::isPrimary() const noexcept {
    return this == &sPlatformConnectedMonitors[0];
  }

  std::wstring_view Monitor::deviceName() const noexcept {
    return mDeviceName;
  }

  std::wstring_view Monitor::adapterName() const noexcept {
    return mAdapterName;
  }

  std::wstring_view Monitor::name() const noexcept {
    return mMonitorName;
  }

  glm::uvec2 Monitor::sizeInMillimeters() const noexcept {
    return mSizeInMillimeters;
  }

  glm::uvec2 Monitor::sizeInPixels() const noexcept {
    return mSizeInPixels;
  }

  glm::ivec2 Monitor::screenPosition() const noexcept {
    return mScreenPosition;
  }

  const VideoMode& Monitor::videoMode() const noexcept {
    return *mCurrentMode;
  }

  const std::vector<VideoMode>& Monitor::availableModes() const noexcept {
    return mAvailableModes;
  }

}

#endif /* HEARTH_WINDOWS_PLATFORM */