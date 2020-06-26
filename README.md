# Hearth Engine

A C++ based game engine used by the *'Scintillate Arts'* organization to develop games for multiple platforms, of which will be: Linux and Windows. Currently we are only focused on Windows. There are no current plans for supporting mobile yet.

---

The game engine itself is simplistic, and will typically require a bit of set up to use in a commercial application. It is designed to allow a developer to bypass the internal design to some extent, so that a developer who is not happy with the implementation can design their own and have Hearth Engine use that. **This is a work in progress as the base functionality isn't all the way implemented itself, at the time of writing this.**

## Building Hearth Engine

__What you need:__

* GCC 9.2 or earlier
  * We recommend using Pacman on [MSYS2](https://www.msys2.org/) to get GCC 9.2 or earlier then set it on your path to use from Git bash or Cmder or whatever terminal emulator you like the most.
  * We use some C++20 features (or are upgrading to) for better readability and performance as well as better const semantics. Of which `aggregate initializer names` are one of those features. We also plan to use the `3-way comparison operator` to speed up production. If possible we will also use concepts, but in all likelihood we'll wait for the release of C++20 before using them.
* CMAKE 3.9 or earlier
* [VulkanSDK 1.1.126](https://vulkan.lunarg.com/sdk/home#windows) or earlier
  * We use Vulkan for our default rendering engine, and do not plan to support any other graphics libraries thanks to Vulkan's very good cross platform support. It is also in our opinion, on par if not better than DirectX for Windows, and DirectX is not cross platform.
* [GLM 0.9.9.5](https://github.com/g-truc/glm/releases/tag/0.9.9.5) or earlier
  * This is the math library we are currently using, though it may change in the future, or we may write our own version to fix the inconsitency in the naming scheming and formatting. We may also do this for standard library objects for execution performance and memory performance reasons.

__To build Hearth Engine:__

* Clone the repository through HTTPS or SSH protocol.
* Run the build script with any combination of the following flags:
  * 'd' Specifies a debug build.
  * 'r' Specifies a release build.
  * 'o' Specifies a optimized debug build.
  * 'p' Specifies for the library to enable profiling. This is more of a debugging feature, so we recommend only enabling it in a debugging build.
  * 'b' Enables benchmark testing.
  * 't' Enables source testing.
  * 'q' Is used to ignore commonly annoying compiler warnings, usually ones that show up a lot.

For example a debug build with testing, profiling, and quiet compiling would have the flag sequence: `-dtpq` or potentially `-pqdt`. The order of the flags do not matter, just that they are followed initially by the `-` and that they are valid flags. The build script will not do anything with flags it doesn't know, it will simply continue to process all flags.

## What can you do for Hearth

Any and all support is welcome, since the project is still in early development, we're focused on rendering classes and the rendering system in general. It has a long way to go, so any support finding and removing bugs are welcome. If you have recommendations for a particular direction Hearth should take for rendering, please make an issue about it, or a pull request demonstrating the differences. We encourage everyone to work on a different branch than `master` or whatever Microsoft plans to change the default branch to.
