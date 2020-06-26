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
#include "compiler.hpp"
#include "platform.hpp"

// Don't allow import and export.
#if defined(HAPI_SHARED_IMPORT) && defined(HAPI_SHARED_EXPORT)
#  error "Both HAPI_SHARED_IMPORT and HAPI_SHARED_EXPORT defined, please choose one or the other."
#endif

// Set up shared library macros.
#if defined(HAPI_WINDOWS_OS) && defined(HAPI_MSVC_COMPILER)
#  define HAPICALL __stdcall
#  define HAPI_IMPORT __declspec(dllimport)
#  define HAPI_EXPORT __declspec(dllexport)
#  if defined(HAPI_SHARED_IMPORT)
#    define HAPI __declspec(dllimport)
#  elif defined(HAPI_SHARED_EXPORT)
#    define HAPI __declspec(dllexport)
#  else
#    define HAPI
#  endif
#else
#  define HAPI_IMPORT
#  define HAPI_EXPORT __attribute__((visibility("default")))
#  if defined(HAPI_SHARED_EXPORT)
#    define HAPI __attribute__((visibility("default")))
#  else
#    define HAPI
#  endif
#endif

// Define namespace shortcuts.
#define HAPI_NAMESPACE_NAME  com::simular::hapi
#define HAPI_NAMESPACE_BEGIN namespace HAPI_NAMESPACE_NAME {
#define HAPI_NAMESPACE_END   }
#define HAPI_LIBRARY_NAME    "Hearth Engine"

// Predefine namespace for rename.
HAPI_NAMESPACE_BEGIN
HAPI_NAMESPACE_END

// Rename for shortening.
namespace hearth = HAPI_NAMESPACE_NAME;
