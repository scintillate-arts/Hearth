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
 * \file    Config.hpp
 * \brief   ...
 * \details ...
 */
#ifndef __HEARTH_CORE_CONFIG_HPP__
#define __HEARTH_CORE_CONFIG_HPP__ 1

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
//                                    Platform Detection                                         //
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

// Check if CygWin is available.
#if defined(__CYGWIN__)
#  define HEARTH_CYGWIN_PLATFORM 1
#  define HEARTH_DESKTOP 1
#endif

// Check if MinGW is available.
#if defined(__MINGW32__) || defined(__MINGW64__)
#  define HEARTH_MINGW_PLATFORM 1
#  define HEARTH_DESKTOP 1
#endif

// Check operating system and processor.
#if defined(HEARTH_ANDROID_PLATFORM) || defined(__ANDROID__)
#  undef HEARTH_ANDROID_PLATFORM
#  define HEARTH_ANDROID_PLATFORM 1
#  define HEARTH_LINUX_PLATFORM 1
#  define HEARTH_UNIX_PLATFORM 1
#  define HEARTH_POSIX_PLATFORM 1
#  define HEARTH_PLATFORM_NAME "Android"
#  define HEARTH_MOBILE 1
#  if defined(__arm__)
#    define HEARTH_ARM32_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Android on ARM"
#  elif defined(__aarch64__)
#    define HEARTH_ARM64_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Android on ARM64"
#  elif defined(__i386__)
#    define HEARTH_X86_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Android on x86"
#  elif defined(__x86_64__)
#    define HEARTH_X64_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Android on x64"
#  else
#    error "Unknown Processor"
#  endif
#  if !defined(HEARTH_BIG_ENDIAN) && !defined(HEARTH_LITTLE_ENDIAN)
#    define HEARTH_LITTLE_ENDIAN 1
#  endif
#elif defined(HEARTH_APPLE_PLATFORM) || defined(__APPLE__) && __APPLE__
#  include <TargetConditionals.h>
#  undef HEARTH_APPLE_PLATFORMS
#  define HEARTH_APPLE_PLATFORM 1
#  define HEARTH_POSIX_PLATFORM 1
#  if defined(HEARTH_IPHONE_PLATFORM) || defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
#    undef HEARTH_IPHONE_PLATFORM
#    define HEARTH_IPHONE_PLATFORM 1
#    define HEARTH_PLATFORM_NAME "iPhone"
#    define HEARTH_POSIX_THREADS_AVAILABLE 1
#    define HEARTH_LITTLE_ENDIAN 1
#    define HEARTH_MOBILE 1
#    if defined(__arm__)
#      define HEARTH_ARM32_PROCESSOR 1
#      define HEARTH_PLATFORM_DESC "iPhone on ARM"
#    elif defined(__aarch64__) || defined(__AARCH64)
#      define HEARTH_ARM64_PROCESSOR 1
#      define HEARTH_PLATFORM_DESC "iPhone on ARM64"
#    elif defined(__i386__)
#      define HEARTH_IPHONE_SIMULATOR_PLATFORM 1
#      define HEARTH_X86_PROCESSOR 1
#      define HEARTH_PLATFORM_DESC "iPhone simulator on x86"
#    elif defined(__x86_64__) || defined(__amd64)
#      define HEARTH_IPHONE_SIMULATOR_PLATFORM 1
#      define HEARTH_X64_PROCESSOR 1
#      define HEARTH_PLATFORM_DESC "iPhone simulator on x64"
#    else
#      error "Unknown Processor"
#    endif
#  elif defined(HEARTH_OSX_PLATFORM) || defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
#    undef HEARTH_OSX_PLATFORM
#    define HEARTH_OSX_PLATFORM 1
#    define HEARTH_UNIX_PLATFORM 1
#    define HEARTH_PLATFORM_NAME "OSX"
#    define HEARTH_DESKTOP 1
#    if defined(__i386__) || defined(__intel__)
#      define HEARTH_X86_PROCESSOR 1
#      define HEARTH_LITTLE_ENDIAN 1
#      define HEARTH_PLATFORM_DESC "OSX on x86"
#    elif defined(__x86_64__) || defined(__amd64)
#      define HEARTH_X64_PROCESSOR 1
#      define HEARTH_LITTLE_ENDIAN 1
#      define HEARTH_PLATFORM_DESC "OSX on x64"
#    elif defined(__arm__)
#      define HEARTH_ARM32_PROCESSOR 1
#      define HEARTH_LITTLE_ENDIAN 1
#      define HEARTH_PLATFORM_DESC "OSX on ARM"
#    elif defined(__aarch64__) || defined(__AARCH64)
#      define HEARTH_ARM64_PROCESSOR 1
#      define HEARTH_LITTLE_ENDIAN 1
#      define HEARTH_PLATFORM_DESC "OSX on ARM64"
#    elif defined(__POWERPC64__) || defined(__powerpc64__)
#      define HEARTH_POWERPC64_PROCESSOR 1
#      define HEARTH_BIG_ENDIAN 1
#      define HEARTH_PLATFORM_DESC "OSX on PowerPC 64"
#    elif defined(__POWERPC__) || defined(__powerpc__)
#      define HEARTH_POWERPC32_PROCESSOR 1
#      define HEARTH_BIG_ENDIAN 1
#      define HEARTH_PLATFORM_DESC "OSX on PowerPC"
#    else
#      error "Unknown Processor"
#    endif
#  else
#    error "Unknown Apple platform"
#  endif
#elif defined(HEARTH_LINUX_PLATFORM) || (defined(__linux) || defined(__linux__))
#  undef HEARTH_LINUX_PLATFORM
#  define HEARTH_LINUX_PLATFORM 1
#  define HEARTH_UNIX_PLATFORM 1
#  define HEARTH_POSIX_PLATFORM 1
#  define HEARTH_PLATFORM_NAME "Linux"
#  define HEARTH_DESKTOP 1
#  if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
#    define HEARTH_X86_PROCESSOR 1
#    define HEARTH_LITTLE_ENDIAN 1
#    define HEARTH_PLATFORM_DESC "Linux on x86"
#  elif defined(__ARM_ARCH_7A__) || defined(__ARM_EABI__)
#    define HEARTH_ARM32_PROCESSOR 1
#    define HEARTH_LITTLE_ENDIAN 1
#    define HEARTH_PLATFORM_DESC "Linux on ARM 6/7 32-bits"
#  elif defined(__aarch64__) || defined(__AARCH64)
#    define HEARTH_ARM64_PROCESSOR 1
#    define HEARTH_LITTLE_ENDIAN 1
#    define HEARTH_PLATFORM_DESC "Linux on ARM64"
#  elif defined(__x86_64__)
#    define HEARTH_X64_PROCESSOR 1
#    define HEARTH_LITTLE_ENDIAN 1
#    define HEARTH_PLATFOMR_DESC "Linux on x64"
#  elif defined(__powerpc64__)
#    define HEARTH_POWERPC64_PROCESSOR 1
#    define HEARTH_BIG_ENDIAN 1
#    define HEARTH_PLATFORM_DESC "Linux on PowerPC 64"
#  elif defined(__powerpc__)
#    define HEARTH_POWERPC32_PROCESSOR 1
#    define HEARTH_BIG_ENDIAN 1
#    define HEARTH_PLATFORM_DESC "Linux on PowerPC"
#  else
#    error "Unknown Processor"
#  endif
#elif defined(HEARTH_BSD_PLATFORM) || (defined(__BSD__) || defined(__FreeBSD__))
#  undef HEARTH_BSD_PLATFORM
#  define HEARTH_BSD_PLATFORM 1
#  define HEARTH_UNIX_PLATFORM 1
#  define HEARTH_POSIX_PLATFORM 1
#  define HEARTH_PLATFORM_NAME "BSD Unix"
#  if defined(__i386__) || defined(__intel__)
#    define HEARTH_X86_PROCESSOR 1
#    define HEARTH_LITTLE_ENDIAN 1
#    define HEARTH_PLATFORM_DESC "BSD on x86"
#  elif defined(__x86_64__)
#    define HEARTH_X64_PROCESSOR 1
#    define HEARTH_LITTLE_ENDIAN 1
#    define HEARTH_PLATFORM_DESC "BSD on x64"
#  elif defined(__powerpc64__)
#    define HEARTH_POWERPC64_PROCESSOR 1
#    define HEARTH_BIG_ENDIAN 1
#    define HEARTH_PLATFORM_DESC "BSD on PowerPC 64"
#  elif defined(__powerpc__)
#    define HEARTH_POWERPC32_PROCESSOR 1
#    define HEARTH_BIG_ENDIAN 1
#    define HEARTH_PLATFORM_DESC "BSD on PowerPC"
#  else
#    error "Unknown Processor"
#  endif
#  if !defined(HEARTH_FREEBSD_PLATFORM) && defined(__FreeBSD__)
#    define HEARTH_FREEBSD_PLATFORM 1
#  endif
#elif defined(HEARTH_WINDOWS_PHONE_PLATFORM)
#  undef HEARTH_WINDOWS_PHONE_PLATFORM
#  define HEARTH_WINDOWS_PHONE_PLATFORM 1
#  define HEARTH_MICROSOFT_PLATFORM 1
#  define HEARTH_PLATFORM_NAME "Windows Phone"
#  define HEARTH_LITTLE_ENDIAN 1
#  define HEARTH_MOBILE 1
#  if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
#    define HEARTH_X64_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Windows Phone on x64"
#  elif defined(_M_IX86) || defined(_X86_)
#    define HEARTH_X86_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Windows Phone on x86"
#  elif defined(_M_ARM)
#    define HEARTH_ARM32_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Windows Phone on ARM"
#  else
#    error "Unknown Processor"
#  endif
#  define HEARTH_WINAPI_FAMILY_APP 1
#  define HEARTH_WINAPI_FAMILY_DESKTOP_APP 2
#  define HEARTH_WINAPI_FAMILY_PHONE_APP 3
#  if defined(WINAPI_FAMILY)
#    include <winapifamily.h>
#    if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#      define HEARTH_WINAPI_FAMILY
#    else
#      error "Unsupported WINAPI_FAMILY for Windows Phone"
#    endif
#  else
#    error "WINAPI_FAMILY should always be defined on Windows Phone"
#  endif
#  define HEARTH_WINAPI_FAMILY_PARTITION(partition) (partition)
#  if HEARTH_WINAPI_FAMILY == HEARTH_WINAPI_FAMILY_PHONE_APP
#    define HEARTH_WINAPI_PARTITION_CORE 1
#    define HEARTH_WINAPI_PARTITION_PHONE 1
#    define HEARTH_WINAPI_PARTITION_APP 1
#  else
#    error "Unsupported WINAPI_FAMILY for Windows Phone"
#  endif
#elif (defined(HEARTH_WINDOWS_PLATFORM) || (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__))) && !defined(CS_UNDEFINED_STRING)
#  undef HEARTH_WINDOWS_PLATFORM
#  define HEARTH_MICROSOFT_PLATFORM 1
#  define HEARTH_WINDOWS_PLATFORM 1
#  define HEARTH_PLATFORM_NAME "Windows"
#  define HEARTH_LITTLE_ENDIAN 1
#  define HEARTH_DESKTOP 1
#  if defined(_WIN64) || defined(__WIN64__)
#    define HEARTH_WIN64_PLATFORM 1
#  else
#    define HEARTH_WIN32_PLATFORM 1
#  endif
#  if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
#    define HEARTH_X64_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Windows on x64"
#  elif defined(_M_IX86) || defined(_X86_)
#    define HEARTH_X86_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Windows on x86"
#  elif defined(_M_IA64) || defined(_IA64_)
#    define HEARTH_IA64_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Windows on IA-64"
#  elif defined(_M_ARM)
#    define HEARTH_ARM32_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Windows on ARM"
#  elif defined(_M_ARM64)
#    define HEARTH_ARM64_PROCESSOR 1
#    define HEARTH_PLATFORM_DESC "Windows on ARM64"
#  else
#    error "Unknown Processor"
#  endif
#  define HEARTH_WINAPI_FAMILY_APP 1000
#  define HEARTH_WINAPI_FAMILY_DESKTOP_APP 1001
#  define HEARTH_WINAPI_FAMILY_GAMES 1002
#  if defined(WINAPI_FAMILY)
#    if defined(_MSC_VER)
#      pragma warning(push, 0)
#    endif
#    include <winapifamily.h>
#    if defined(_MSC_VER)
#      pragma warning(pop)
#    endif
#    if defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#      define HEARTH_WINAPI_FAMILY HEARTH_WINAPI_FAMILY_DESKTOP_APP
#    elif defined(WINAPI_FAMILY_APP) && WINAPI_FAMILY == WINAPI_FAMILY_APP
#      define HEARTH_WINAPI_FAMILY HEARTH_WINAPI_FAMILY_APP
#    elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
#      define HEARTH_WINAPI_FAMILY HEARTH_WINAPI_FAMILY_GAMES
#    else
#      error "Unsupported WINAPI_FAMILY"
#    endif
#  endif
#  define HEARTH_WINAPI_FAMILY_PARTITION(partition) (partition)
#  define HEARTH_WINAPI_PARTITION_DESKTOP 1
#  define HEARTH_WINAPI_PARTITION_APP 1
#  define HEARTH_WINAPI_PARTITION_GAMES (HEARTH_WINAPI_FAMILY == HEARTH_WINAPI_FAMILY_GAMES)
#  if defined(__cplusplus_winrt)
#    define HEARTH_WINRT_PLATFORM 1
#  endif
#else
#  error "Unknown Platform"
#endif

// Make sure this gets defined.
#if !defined(HEARTH_ARM_PROCESSOR)
#  if defined(HEARTH_ARM32_PROCESSOR) || defined(HEARTH_ARM64_PROCESSOR) || defined(HEARTH_ARM7_PROCESSOR)
#    define HEARTH_ARM_PROCESSOR 1
#  endif
#endif

// Make sure this gets defined.
#if !defined(HEARTH_POWERPC_PROCESSOR)
#  if defined(HEARTH_POWERPC64_PROCESSOR) || defined(HEARTH_POWERPC32_PROCESSOR)
#    define HEARTH_POWERPC_PROCESSOR 1
#  endif
#endif

// Check pointer size.
#if !defined(HEARTH_PROCESSOR_PTR_SIZE)
#  if defined(__WORDSIZE)
#    define HEARTH_PROCESSOR_PTR_SIZE ((__WORDSIZE) / 8)
#  elif defined(_WIN64) || defined(__LP64__) || defined(_M_IA64) || defined(__ia64__) || defined(__arch64__) || defined(__aarch64__) || defined(__mips64__) || defined(__64BIT__) || defined(__Ptr_Is_64)
#    define HEARTH_PROCESSOR_PTR_SIZE 8
#  elif defined(__CC_ARM) && (__sizeof_ptr == 8)
#    define HEARTH_PROCESSOR_PTR_SIZE 8
#  else
#    define HEARTH_PROCESSOR_PTR_SIZE 4
#  endif
#endif

// Check word size.
#if !defined(HEARTH_PROCESSOR_WORD_SIZE)
#  define HEARTH_PROCESSOR_WORD_SIZE HEARTH_PROCESSOR_PTR_SIZE
#endif

// Check min allocation alignment.
#if !defined(HEARTH_PROCESSOR_MIN_MALLOC_ALIGNMENT)
#  if defined(HEARTH_APPLE_PLATFORM)
#    define HEARTH_PROCESSOR_MIN_MALLOC_ALIGNMENT 16
#  elif defined(HEARTH_ANDROID_ALIGNMENT) && defined(HEARTH_ARM_PROCESSOR)
#    define HEARTH_PROCESSOR_MIN_MALLOC_ALIGNMENT 8
#  elif defined(HEARTH_ANDROID_ALIGNMENT) && defined(HEARTH_X86_PROCESSOR)
#    define HEARTH_PROCESSOR_MIN_MALLOC_ALIGNMENT 8
#  else
#    define HEARTH_PROCESSOR_MIN_MALLOC_ALIGNMENT (HEARTH_PROCESSOR_PTR_SIZE * 2)
#  endif
#endif

// Can the processor read and write built-in types that aren't naturally aligned?
#if !defined(HEARTH_PROCESSOR_MISALIGNED_SUPPORT_LEVEL)
#  if defined(HEARTH_X64_PROCESSOR)
#    define HEARTH_PROCESSOR_MISALIGNED_SUPPORT_LEVEL 2
#  else
#    define HEARTH_PROCESSOR_MISALIGNED_SUPPORT_LEVEL 0
#  endif
#endif

// Disable Windows API partition if not defined or on another platform.
#if !defined(HEARTH_WINAPI_FAMILY_PARTITION)
#  define HEARTH_WINAPI_FAMILY_PARTITION(partition) (0)
#endif

// Best guess at cache line sizes at compile time.
#if !defined(HEARTH_PROCESSOR_CACHE_LINE_SIZE)
#  if defined(HEARTH_X86_PROCESSOR)
#    define HEARTH_PROCESSOR_CACHE_LINE_SIZE 32
#  elif defined(HEARTH_X64_PROCESSOR)
#    define HEARTH_PROCESSOR_CACHE_LINE_SIZE 64
#  elif defined(HEARTH_ARM32_PROCESSOR)
#    define HEARTH_PROCESSOR_CACHE_LINE_SIZE 32
#  elif defined(HEARTH_ARM64_PROCESSOR)
#    define HEARTH_PROCESSOR_CACHE_LINE_SIZE 64
#  elif (HEARTH_PROCESSOR_WORD_SIZE == 4)
#    define HEARTH_PROCESSOR_CACHE_LINE_SIZE 32
#  else
#    define HEARTH_PROCESSOR_CACHE_LINE_SIZE 64
#  endif
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
//                                    Compiler Detection                                         //
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
// Check for this.
#if !defined(INTERNAL_STRINGIZE)
#  define INTERNAL_STRINGIZE(x) INTERNAL_PRIMITIVE_STRINGIZE(x)
#endif

// Check for this too.
#if !defined(INTERNAL_PRIMITIVE_STRINGIZE)
#  define INTERNAL_PRIMITIVE_STRINGIZE(x) #x
#endif

// Used to check for feature on Clang.
#if !defined(HEARTH_COMPILER_HAS_FEATURE)
#  if defined(__clang__)
#    define HEARTH_COMPILER_HAS_FEATURE(x) __has_feature(x)
#  else
#    define HEARTH_COMPILER_HAS_FEATURE(x) 0
#  endif
#endif

// Used to check for builtin on Clang.
#if !defined(HEARTH_COMPILER_HAS_BUILTIN)
#  if defined(__clang__)
#    define HEARTH_COMPILER_HAS_BUILTIN(x) __has_builtin(x)
#  else
#    define HEARTH_COMPILER_HAS_BUILTIN(x) 0
#  endif
#endif

// EDG compiler front-end, used by other compilers such as SN.
#if defined(__EDG_VERSION__)
#  define HEARTH_EDG_COMPILER 1
#  if defined(_MSC_VER)
#    define HEARTH_EDG_VC_MODE 1
#  endif
#  if defined(__GNUC__)
#    define HEARTH_EDG_GCC_MODE 1
#  endif
#endif

// Check for WinRTCX.
#if !defined(HEARTH_WINRTCX_ENABLED) && defined(__cpluplus_winrt)
#  define HEARTH_WINRTCX_ENABLED 1
#endif

// Check for C++11 support.
#if !defined(HEARTH_CPP11_ENABLED) && defined(__cplusplus)
#  if (__cplusplus >= 201103L)
#    define HEARTH_CPP11_ENABLED 1
#  elif defined(__GNUC__) && defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define HEARTH_CPP11_ENABLED 1
#  elif defined(_MSV_VER) && (_MSC_VER >= 1600)
#    define HEARTH_CPP11_ENABLED 1
#  endif
#endif

// Check for C++14 support.
#if !defined(HEARTH_CPP14_ENABLED) && defined(__cplusplus)
#  if (__cplusplus >= 201402L)
#    define HEARTH_CPP14_ENABLED 1
#  elif defined(_MSC_VER) && (_MSC_VER >= 1900)
#    define HEARTH_CPP14_ENABLED 1
#  endif
#endif

// Check for C++17 support.
#if !defined(HEARTH_CPP17_ENABLED) && defined(__cplusplus)
#  if (__cplusplus >= 201703L)
#    define HEARTH_CPP17_ENABLED 1
#  elif defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)
#    define HEARTH_CPP17_ENABLED 1
#  endif
#endif

// Check for C++20 support.
#if !defined(HEARTH_CPP20_ENABLED) && defined(__cplusplus)
#  if (__cplusplus >= 202002L)
#    define HEARTH_CPP20_ENABLED 1
#  elif defined(_MSVC_LANG) && (_MSVC_LANG >= 202002L)
#    define HEARTH_CPP20_ENABLED 1
#  endif
#endif

// Don't allow a compiler that doesn't support C++14 or earlier.
// #if !defined(HEARTH_CPP17_ENABLED)
// #  error "We request that you kindly at least use a compile which supports, and has enabled, C++17."
// #endif

// Detect different compilers.
#if defined(__ARMCC_VERSION)
#  define HEARTH_RVCT_COMPILER 1
#  define HEARTH_ARM_COMPILER 1
#  define HEARTH_COMPILER_VERSION __ARMCC_VERSION
#  define HEARTH_COMPILER_NAME "RVCT"
#elif defined(__clang__) && !defined(_MSC_VER)
#  define HEARTH_CLANG_COMPILER 1
#  define HEARTH_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)
#  define HEARTH_COMPILER_NAME "Clang"
#  define HEARTH_COMPILER_DESC "Clang " __clang_version__
#elif defined(__GNUC__)
#  define HEARTH_GNUC_COMPILER 1
#  define HEARTH_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#  define HEARTH_COMPILER_NAME "GCC"
#  define HEARTH_COMPILER_DESC "GCC compiler, version " INTERNAL_STRINIZE(__GNUC__) "." INTERNAL_STRINGIZE(__GNUC_MINOR__)
#  if (__GNUC__ == 2) && (__GNUC_MINOR__ < 95)
#    define HEARTH_NO_MEMBER_TEMPLATES 1
#  endif
#  if (__GNUC__ == 2) && (__GNUC_MINOR__ < 97)
#    define HEARTH_NO_MEMBER_TEMPLATE_FRIENDS 1
#  endif
#  if (__GNUC__ == 3) && ((__GNUC_MINOR == 1) || (__GNUC_MINOR__ == 2))
#    define HEARTH_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS 1
#  endif
#elif defined(__BORLANDC__)
#  define HEARTH_BORLANDC_COMPILER 1
#  define HEARTH_COMPILER_VERSION __BORLANDC__
#  define HEARTH_COMPILER_NAME "Borland C"
#  if (__BORLANDC__ <= 0x0550)
#    define HEARTH_NO_MEMBER_TEMPLATE_FRIENDS 1
#  endif
#  if (__BORLANDC__ <= 0x0561) && (__BORLANDC__ < 0x0600)
#    define HEARTH_NO_MEMBER_FUNCTION_SPECIALIZATION 1
#  endif
#elif defined(__ICL) || defined(__ICC)
#  define HEARTH_INTEL_COMPILER 1
#  if defined(_MSC_VER)
#    define HEARTH_MSVC_COMPILER 1
#    define HEARTH_MICROSOFT_COMPILER 1
#  elif defined(__GNUC__)
#    define HEARTH_GNUC_COMPILER 1
#  endif
#  if defined(__ICL)
#    define HEARTH_COMPILER_VERSION __ICL
#  elif defined(__ICC)
#    define HEARTH_COMPILER_VERSION __ICC
#  endif
#  define HEARTH_COMPILER_NAME "Intel C++"
#  if defined(_MSC_VER)
#    define HEARTH_COMPILER_DESC "Intel C++ compiler, version " INTERNAL_STRINGIZE(HEARTH_COMPILER_VERSION) ", EDG version " INTERNAL_STRINGIZE(__EDG_VERSION__) ", VC++ version " INTERNAL_STRINGIZE(_MSC_VER)
#  elif defined(__GNUC__)
#    define HEARTH_COMPILER_DESC "Intel C++ compiler, version " INTERNAL_STRINGIZE(HEARTH_COMPILER_VERSION) ", EDG version " INTERNAL_STRINGIZE(__EDG_VERSION__) ", GCC version " INTERNAL_STRINGIZE(__GNUC__)
#  else
#    define HEARTH_COMPILER_DESC "Intel C++ compiler, version " INTERNAL_STRINGIZE(HEARTH_COMPILER_VERSION) ", EDG version " INTERNAL_STRINGIZE(__EDG_VERSION__)
#  endif
#elif defined(_MSC_VER)
#  define HEARTH_MSVC_COMPILER 1
#  define HEARTH_MICROSOFT_COMPILER 1
#  define HEARTH_COMPILER_VERSION _MSC_VER
#  define HEARTH_COMPILER_NAME "Microsoft Visual C++"
#  if defined(__clang__)
#    define HEARTH_CLANG_CL_COMPILER 1
#  endif
#  define HEARTH_MSVC_STANDARD_LIB 1
#  define HEARTH_MICROSOFT_STANDARD_LIB 1
#  if (_MSC_VER <= 1200)
#    if (_MSC_VER < 1200)
#      define HEARTH_MSVCOLD_COMPILER 1
#    else
#      define HEARTH_MSVC6_COMPILER 1
#    endif
#    if (_MSC_VER < 1200)
#      define HEARTH_NO_TEMPLATE_SPECIALIZATION 1
#    endif
#    define HEARTH_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS 1
#    define HEARTH_NO_VOID_RETURNS 1
#    define HEARTH_NO_EXCEPTION_STD_NAMESPACE 1
#    define HEARTH_NO_DEDUCED_TYPENAME 1
#    define HEARTH_NO_STATIC_CONSTANTS 1
#    define HEARTH_NO_COVARIANT_RETURN_TYPE 1
#    define HEARTH_NO_ARGUMENT_DEPENDENT_LOOKUP 1
#    define HEARTH_NO_TEMPLATE_TEMPLATES 1
#    define HEARTH_NO_TEMPLATE_PARTIAL_SPECIALIZATION 1
#    define HEARTH_NO_MEMBER_TEMPLATE_FRIENDS 1
#  elif (_MSC_VER <= 1300)
#    define HEARTH_MSVC7_COMPILER 1
#    define HEARTH_NO_COVARIANT_RETURN_TYPE 1
#    define HEARTH_NO_ARGUMENT_DEPENDENT_LOOKUP 1
#    define HEARTH_NO_TEMPLATE_TEMPLATES 1
#    define HEARTH_NO_TEMPLATE_PARTIAL_SPECIALIZATION 1
#    define HEARTH_NO_MEMBER_TEMPLATE_FRIENDS 1
#    define HEARTH_NO_MEMBER_FUNCTION_SPECIALIZATION 1
#  elif (_MSC_VER <= 1400)
#    define HEARTH_MSVC7_2003_COMPILER 1
#    define HEARTH_MSVC7_COMPILER 1
#  elif (_MSC_VER <= 1500)
#    define HEARTH_MSVC8_2005_COMPILER 1
#    define HEARTH_MSVC8_COMPILER 1
#  elif (_MSC_VER <= 1600)
#    define HEARTH_MSVC9_2008_COMPILER 1
#    define HEARTH_MSVC9_COMPILER 1
#  elif (_MSC_VER <= 1700)
#    define HEARTH_MSVC10_2010_COMPILER 1
#    define HEARTH_MSVC10_COMPILER 1
#  elif (_MSC_VER <= 1800)
#    define HEARTH_MSVC11_2011_COMPILER 1
#    define HEARTH_MSVC12_2012_COMPILER 1
#    define HEARTH_MSVC11_COMPILER 1
#    define HEARTH_MSVC12_COMPILER 1
#  elif (_MSC_VER <= 1900)
#    define HEARTH_MSVC13_2013_COMPILER 1
#    define HEARTH_MSVC13_COMPILER 1
#  elif (_MSC_VER <= 1910)
#    define HEARTH_MSVC14_2015_COMPILER 1
#    define HEARTH_MSVC14_COMPILER 1
#  elif (_MSC_VER <= 1911)
#    define HEARTH_MSVC15_2017_COMPILER 1
#    define HEARTH_MSVC15_COMPILER 1
#  endif
#elif defined(__xlC__)
#  define HEARTH_IBM_COMPILER 1
#  define HEARTH_COMPILER_VERSION __xlC__
#  define HEARTH_COMPILER_NAME "IBM XL C"
#else
#  define HEARTH_COMPILER_VERSION 0
#  define HEARTH_COMPILER_NAME "Unknown"
#endif

// Define compiler description if not already.
#if !defined(HEARTH_COMPILER_DESC)
#  define HEARTH_COMPILER_DESC HEARTH_COMPILER_NAME " compiler, version " INTERNAL_STRINGIZE(HEARTH_COMPILER_VERSION)
#endif

// Described as deprecated definitions for backwards compatibility by EABase.
// Since we're practically copying the implemenation, we should include these.
#if !defined(HEARTH_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#  define HEARTH_PARTIAL_TEMPLATE_SPECIALIZATION 1
#endif
#if !defined(HEARTH_NO_TEMPLATE_SPECIALIZATION)
#  define HEARTH_TEMPLATE_SPECIALIZATION 1
#endif
#if !defined(HEARTH_NO_MEMBER_TEMPLATES)
#  define HEARTH_MEMBER_TEMPLATES 1
#endif
#if !defined(HEARTH_NO_MEMBER_TEMPLATE_SPECIALIZATION)
#  define HEARTH_MEMBER_TEMPLATE_SPECIALIZATION 1
#endif

// Check if RTTI is enabled.
#if defined(__EDG_VERSION__) && !defined(__RTTI)
#  define HEARTH_NO_RTTI 1
#elif defined(__clang__) && !defined(cxx_rtti)
#  define HEARTH_NO_RTTI 1
#elif defined(__IBMCPP__) && !defined(__RTTI_ALL__)
#  define HEARTH_NO_RTTI 1
#elif defined(__GXX_ABI_VERSION__) && !defined(__GXX_RTTI)
#  define HEARTH_NO_RTTI 1
#elif defined(_MSC_VER) && !defined(_CPPRTTI)
#  define HEARTH_NO_RTTI 1
#elif defined(__ARMCC_VERSION) && defined(__TARGET_CPU_MPCORE) && !defined(__RTTI)
#  define HEARTH_NO_RTTI 1
#endif

// Check if exceptions/unwind is enabled.
#if !defined(HEARTH_NO_EXCEPTIONS) && !defined(HEARTH_NO_UNWIND)
#  if defined(HEARTH_GNUC_COMPILER) && defined(_NO_EX)
#    define HEARTH_NO_EXCEPTIONS 1
#  elif (defined(HEARTH_CLANG_COMPILER) || defined(HEARTH_GNUC_COMPILER) || defined(HEARTH_INTEL_COMPILER) || defined(HEARTH_RVCT_COMPILER)) && !defined(__EXCEPTIONS)
#    define HEARTH_NO_EXCEPTIONS 1
#  elif defined(HEARTH_MSVC_COMPILER) && !defined(_CPPUNWIND)
#    define HEARTH_NO_UNWIND 1
#  endif
#endif

// Check for Dinkumware standard lib.
#if !defined(HEARTH_HAS_DINKUMWARE_CPP_LIB) && !defined(HEARTH_NO_DINKUMWARE_CPP_LIB)
#  if defined(__cplusplus)
#    include <cstddef>
#  endif
#  if defined(__cplusplus) && defined(_CPPLIB_VER)
#    define HEARTH_HAS_DINKUMWARE_CPP_LIB 1
#  else
#    define HEARTH_NO_DINKUMWARE_CPP_LIB 1
#  endif
#endif

// Check if aligned new is enabled.
#if !defined(HEARTH_NO_ALIGNED_NEW)
#  if !(defined(_HAS_ALIGNED_NEW) && _HAS_ALIGNED_NEW) && !defined(HEARTH_CPP17_ENABLED)
#    define HEARTH_NO_ALIGNED_NEW 1
#  endif
#endif

// Android doesn't support standard lib.
#if defined(HEARTH_PLATFORM_ANDROID)
#  define HEARTH_NO_STANDARD_CPP_LIB 1
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
//                                      Compiler Traits                                          //
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

// Import and export
#if !defined(HEARTH_IMPORT) && defined(HEARTH_MSVC_COMPILER)
#  define HEARTH_IMPORT __declspec(dllimport)
#else
#  define HEARTH_IMPORT
#endif
#if !defined(HEARTH_EXPORT) && defined(HEARTH_MSVC_COMPILER)
#  define HEARTH_EXPORT __declspec(dllexport)
#else
#  define HEARTH_EXPORT __attribute__((visibility("default")))
#endif

// Big bad, no do.
#if defined(HEARTH_BUILD_SHARED) && defined(HEARTH_USE_SHARED)
#  error "Both of HEARTH_BUILD_SHARED and HEARTH_USE_SHARED defined, please make up your mind."
#endif

// Shared build?
#if defined(HEARTH_WINDOWS_PLATFORM) && defined(HEARTH_MSVC_COMPILER)
#  if defined(HEARTH_BUILD_SHARED)
#    define HEARTHAPI __declspec(dllexport)
#  else
#    define HEARTHAPI __declspec(dllimport)
#  endif
#  if !defined(HEARTHCALL)
#    define HEARTHCALL __stdcall
#  endif
#else
#  if defined(HEARTH_BUILD_SHARED)
#    define HEARTHAPI __attribute__((visibility("default")))
#  else
#    define HEARTHAPI
#  endif
#  if !defined(HEARTHCALL)
#    define HEARTHCALL __cdecl
#  endif
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
//                                        Graphics APIs                                          //
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
#if !defined(HEARTH_GFX_OPENGL_API) && !defined(HEARTH_GFX_OPENGLES_API) && !defined(HEARTH_GFX_VULKAN_API) && !defined(HEARTH_GFX_METAL_API) && !defined(HEARTH_GFX_DIRECT_API)
#  if HEARTH_MICROSOFT_PLATFORM
#    define HEARTH_GFX_DIRECT_API 1
#    define HEARTH_GFX_OPENGL_API 1 /* Gonna use this for testing to begin with. */
#  elif HEARTH_APPLE_PLATFORM
#    define HEARTH_GFX_METAL_API 1
#  elif HEARTH_ANDROID_PLATFORM
#    define HEARTH_GFX_OPENGLES_API 1
#  elif HEARTH_UNIX_PLATFORM
#    define HEARTH_GFX_OPENGL_API 1
#  else
#    define HEARTH_GFX_OPENGL_API 1
#  endif /* Platform specific API checks */
#endif /* Graphics API check */

#if !HEARTH_APPLE_PLATFORM && defined(HEARTH_GFX_METAL_API)
#  error "Metal is not an available graphics API for non-Apple platforms"
#endif /* Metal on non-Apple/Macintosh/iOS bad juju */

#if !HEARTH_MICROSOFT_PLATFORM && defined(HEARTH_GFX_DIRECT_API)
#  error "Direct(X) is not an available graphics API for non-Microsoft platforms"
#endif /* Direct on non-Windows bad juju */

#if HEARTH_APPLE_PLATFORM && (defined(HEARTH_GFX_OPENGL_API) || defined(HEARTH_GFX_OPENGLES_API))
#  error "OpenGL(ES) APIs are no longer natively supported on Apple platforms, use Metal instead"
#endif /* OpenGL/OpenGL ES on Apple/Macintosh/iOS bad juju */

#endif /* __HEARTH_CONFIG_HPP__ */
