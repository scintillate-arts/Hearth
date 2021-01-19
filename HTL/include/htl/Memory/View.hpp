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
  struct ViewHelper { };

}

template<typename TType>
struct htl::ViewTraits final {
  using ViewType       = View<TType>;
  using StorageType    = const TType*;
  using ValueType      = TType;
  using ValuePointer   = const TType*;
  using ValueReference = const TType&;

  constexpr
  static ValuePointer get(ViewType a_viewobj) noexcept {
    return static_cast<ValuePointer>(a_viewobj);
  }

  constexpr
  static ValueReference dereference(ViewType a_viewobj) noexcept {
    return *a_viewobj;
  }

  constexpr
  static ViewType makeRef(ValuePointer a_pData) noexcept {
    return View(a_pData);
  }
};

template<typename TType>
struct htl::ViewTraits<htl::View<TType>> final {
  using ViewType       = View<TType>;
  using StorageType    = const TType*;
  using ValueType      = TType;
  using ValuePointer   = const TType*;
  using ValueReference = const TType&;

  constexpr
  static ValuePointer get(ViewType a_viewobj) noexcept {
    return static_cast<ValuePointer>(a_viewobj);
  }

  constexpr
  static ValueReference dereference(ViewType a_viewobj) noexcept {
    return *a_viewobj;
  }

  constexpr
  static ViewType makeRef(ValuePointer a_pData) noexcept {
    return View(a_pData);
  }
};

template<typename TType>
class htl::View final
  : detail::ViewHelper<TType>
{
public:
  using Traits         = ViewTraits<View>;
  using StorageType    = typename Traits::StorageType;
  using ValueType      = typename Traits::ValueType;
  using ValuePointer   = typename Traits::ValuePointer;
  using ValueReference = typename Traits::ValueReference;

  constexpr View() noexcept
    : m_pImmutableData(nullptr)
  { }

  constexpr
  explicit View(StorageType a_pData) noexcept
    : m_pImmutableData(a_pData)
  { }

  constexpr
  View(const View& a_other)
    : m_pImmutableData(a_other.m_pImmutableData)
  { }

  constexpr
  View& operator=(const View& a_other) {
    m_pImmutableData = a_other.m_pImmutableData;
    return *this;
  }

  constexpr
  explicit operator ValuePointer() const noexcept {
    return m_pImmutableData;
  }

  constexpr
  ValueReference operator*() const noexcept {
    return *m_pImmutableData;
  }

  constexpr
  ValuePointer operator->() const noexcept {
    return m_pImmutableData;
  }

private:
  StorageType m_pImmutableData;
};

template<typename TType>
constexpr bool operator==(const htl::View<TType>& a_lhs, const htl::View<TType>& a_rhs) noexcept {
  using ViewTraits   = htl::ViewTraits<TType>;
  using ValuePointer = typename ViewTraits::ValuePointer;
  return static_cast<ValuePointer>(a_lhs) == static_cast<ValuePointer>(a_rhs);
}

template<typename TType>
constexpr bool operator!=(const htl::View<TType>& a_lhs, const htl::View<TType>& a_rhs) noexcept {
  return !(a_lhs == a_rhs);
}

template<typename TType>
constexpr bool operator<(const htl::View<TType>& a_lhs, const htl::View<TType>& a_rhs) noexcept {
  using ViewTraits   = htl::ViewTraits<TType>;
  using ValuePointer = typename ViewTraits::ValuePointer;
  return static_cast<ValuePointer>(a_lhs) < static_cast<ValuePointer>(a_rhs);
}

template<typename TType>
constexpr bool operator>(const htl::View<TType>& a_lhs, const htl::View<TType>& a_rhs) noexcept {
  return a_rhs < a_lhs;
}

template<typename TType>
constexpr bool operator<=(const htl::View<TType>& a_lhs, const htl::View<TType>& a_rhs) noexcept {
  return !(a_rhs < a_lhs);
}

template<typename TType>
constexpr bool operator>=(const htl::View<TType>& a_lhs, const htl::View<TType>& a_rhs) noexcept {
  return !(a_lhs < a_rhs);
}
