# Hearth Engine

A C++ based game engine used by the *'Scintillate Arts'* organization to develop games for multiple
platforms, of which will be Linux, Windows, and MacOS. Currently we only are focused on Windows.
There are no current plans for supporting mobile yet.

---

The game engine itself is simplistic, and will typically require a bit of set up to use in a
commercial application. It is designed to allow a developer to bypass the internal design to some
extent, so that a developer who is not happy with the implementation to design their own and have
Hearth Engine use that. It is possible for a developer to instead, add to the framework in their own
manner and build it specifically for their purposes rather than dynamically replacing built in
systems.

__To build the Hearth Engine library:__

* Clone the repository through HTTPS or SSH protocol.
* Run the build script with any combination of the following flags:
  * 'd' Specifies a debug build.
  * 'r' Specifies a release build.
  * 'o' Specifies a optimized debug build.
  * 'p' Specifies for the library to enable profiling. This is more of a debugging feature, so we
  recommend only enabling it in a debugging build.
  * 'b' Enables benchmark testing.
  * 't' Enables source testing.
  * 'q' Is used to ignore commonly annoying compiler warnings, usually ones that show up a lot.

For example a debug build with testing, profiling, and quiet compiling would have the flag sequence:
`-dtpq` or potentially `-pqdt`. The order of the flags do not matter, just that they are followed
initially by the `-` and that they are valid flags. The build script will not do anything with flags
it doesn't know, it will simply continue to process all flags.

It should be noted that compile the library and tests, you'll need GLM installed on your path as
`GLM_PATH` and VulkanSDK installed as well. These are the only two external libraries so far. We'd
include them in the repo, but the repo would just become bloated.
