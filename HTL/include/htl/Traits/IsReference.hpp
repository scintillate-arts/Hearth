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
#include "Disjunction.hpp"

namespace htl {

  template<typename>
  struct IsLValueReference : FalseType { };

  template<typename TType>
  struct IsLValueReference<TType&> : TrueType { };

  template<typename>
  struct IsRValueReference : FalseType { };

  template<typename TType>
  struct IsRValueReference<TType&&> : TrueType { };

  template<typename TType>
  struct IsReference
    : Disjunction<IsLValueReference<TType>, IsRValueReference<TType>>::Type
  { };

  template<typename TType>
  inline constexpr bool IsLValueReference_V = IsLValueReference<TType>::value;

  template<typename TType>
  inline constexpr bool IsRValueReference_V = IsRValueReference<TType>::value;

  template<typename TType>
  inline constexpr bool IsReference_V = IsReference<TType>::value;

}