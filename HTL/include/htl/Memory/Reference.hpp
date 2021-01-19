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
#include "../Forward.hpp"
#include "../Traits/EnableIf.hpp"
#include "../Traits/IsObject.hpp"

namespace htl::detail {

  template<typename TType, typename = EnableIf_T<IsObject_V<TType>, TType>>
  struct ReferenceHelper { };

}

template<typename TType>
struct htl::ReferenceTraits final {
  using ReferenceType  = Reference<TType>;
  using StorageType    = TType*;
  using ValueType      = TType;
  using ValuePointer   = TType*;
  using ValueReference = TType&;

  constexpr
  static ValuePointer get(ReferenceType a_refobj) noexcept {
    return static_cast<ValuePointer>(a_refobj);
  }

  constexpr
  static ValueReference dereference(ReferenceType a_refobj) noexcept {
    return *a_refobj;
  }

  constexpr
  static ReferenceType makeRef(ValuePointer a_pData) noexcept {
    return Reference(a_pData);
  }
};

template<typename TType>
struct htl::ReferenceTraits<htl::Reference<TType>> final {
  using ReferenceType  = Reference<TType>;
  using StorageType    = TType*;
  using ValueType      = TType;
  using ValuePointer   = TType*;
  using ValueReference = TType&;

  constexpr
  static ValuePointer get(ReferenceType a_refobj) noexcept {
    return static_cast<ValuePointer>(a_refobj);
  }

  constexpr
  static ValueReference dereference(ReferenceType a_refobj) noexcept {
    return *a_refobj;
  }

  constexpr
  static ReferenceType makeRef(ValuePointer a_pData) noexcept {
    return Reference(a_pData);
  }
};

template<typename TType>
class htl::Reference<const TType*> {
  static_assert(true, "Reference template class cannot ");
};

template<typename TType>
class htl::Reference final
  : detail::ReferenceHelper<TType>
{
public:
  using Traits         = ReferenceTraits<Reference>;
  using StorageType    = typename Traits::StorageType;
  using ValueType      = typename Traits::ValueType;
  using ValuePointer   = typename Traits::ValuePointer;
  using ValueReference = typename Traits::ValueReference;

  constexpr Reference() noexcept
    : m_pMutableData(nullptr)
  { }

  constexpr
  explicit Reference(StorageType a_pData) noexcept
    : m_pMutableData(a_pData)
  { }

  constexpr
  Reference(const Reference& a_other)
    : m_pMutableData(a_other.m_pMutableData)
  { }

  constexpr
  Reference& operator=(const Reference& a_other) {
    m_pMutableData = a_other.m_pMutableData;
    return *this;
  }

  constexpr
  explicit operator ValuePointer() const noexcept {
    return m_pMutableData;
  }

  constexpr
  ValueReference operator*() noexcept {
    return *m_pMutableData;
  }

  constexpr
  ValuePointer operator->() noexcept {
    return m_pMutableData;
  }

private:
  StorageType m_pMutableData;
};

template<typename TType>
constexpr bool operator==(const htl::Reference<TType>& a_lhs, const htl::Reference<TType>& a_rhs) noexcept {
  using RefTraits    = htl::ReferenceTraits<TType>;
  using ValuePointer = typename RefTraits::ValuePointer;
  return static_cast<ValuePointer>(a_lhs) == static_cast<ValuePointer>(a_rhs);
}

template<typename TType>
constexpr bool operator!=(const htl::Reference<TType>& a_lhs, const htl::Reference<TType>& a_rhs) noexcept {
  return !(a_lhs == a_rhs);
}

template<typename TType>
constexpr bool operator<(const htl::Reference<TType>& a_lhs, const htl::Reference<TType>& a_rhs) noexcept {
  using RefTraits    = htl::ReferenceTraits<TType>;
  using ValuePointer = typename RefTraits::ValuePointer;
  return static_cast<ValuePointer>(a_lhs) < static_cast<ValuePointer>(a_rhs);
}

template<typename TType>
constexpr bool operator>(const htl::Reference<TType>& a_lhs, const htl::Reference<TType>& a_rhs) noexcept {
  return a_rhs < a_lhs;
}

template<typename TType>
constexpr bool operator<=(const htl::Reference<TType>& a_lhs, const htl::Reference<TType>& a_rhs) noexcept {
  return !(a_rhs < a_lhs);
}

template<typename TType>
constexpr bool operator>=(const htl::Reference<TType>& a_lhs, const htl::Reference<TType>& a_rhs) noexcept {
  return !(a_lhs < a_rhs);
}
