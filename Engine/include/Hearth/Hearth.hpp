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
#ifndef __INC_HEARTH_HEARTH_HPP__
#define __INC_HEARTH_HEARTH_HPP__ 1
#include "Config.hpp"
#include "Core.hpp"

namespace Hearth {
/**
 * \fn      initialize()
 * \brief   Initializes \c HearthEngine and all its subsystems.
 * \details This step of the process is very important to \c HearthEngine. It's responsible for initializing the
 *          \c Environment object, the \c Monitor objects, populating allocator memory blocks along with other things.
 */
/**
 * \fn      terminate()
 * \brief   Terminate \c HearthEngine and all it's subsystems.
 * \details This step of the process is also very important to \c HearthEngine. It's responsible for terminating the
 *          \c Environment object, the \c Monitor objects, and releasing the allocator memory blocks along with other
 *          things.
 */

  void initialize();
  void terminate() noexcept;
}

#endif /* __INC_HEARTH_HEARTH_HPP__ */