/* Copyright (c) 2020-2021 Simular Games, LLC.
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
#include "BooleanType.hpp"
#include "Conditional.hpp"

namespace htl {

  template<typename...>
  struct Conjunction;

  template<>
  struct Conjunction<> : TrueType { };

  template<typename TBool1>
  struct Conjunction<TBool1> : TrueType { };

  template<typename TBool1, typename TBool2>
  struct Conjunction<TBool1, TBool2>
    : Conditional<TBool1::value, TBool2, TBool1>::Type
  { };

  template<typename TBool1, typename TBool2, typename TBool3, typename... TBoolN>
  struct Conjunction<TBool1, TBool2, TBool3, TBoolN...>
    : Conditional<TBool1::value, Conjunction<TBool2, TBool3, TBoolN...>, TBool1>::Type
  { };

  template<typename... TBooleans>
  inline constexpr bool Conjunction_V = Conjunction<TBooleans...>::value;

}