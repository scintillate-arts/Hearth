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
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#  define HAPI_WIN32_OS
#  define HAPI_WIN64_OS
#  define HAPI_WINDOWS_OS
#  define HAPI_OS_NAME "Microsoft Windows"
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  define HAPI_WIN32_OS
#  define HAPI_WINDOWS_OS
#  define HAPI_OS_NAME "Microsoft Windows"
#elif defined(linux) || defined(__linux) || defined(__linux__)
#  define HAPI_LINUX_OS
#  define HAPI_OS_NAME "Torvalds Linux"
#endif /* Platform detection. */
