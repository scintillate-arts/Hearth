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
#ifndef __INC_HEARTH_MEMORY_VIEW_HPP__
#define __INC_HEARTH_MEMORY_VIEW_HPP__ 1

namespace Hearth::Memory {

  template<typename TType>
  class View final {
  public:
    constexpr View() noexcept
      : m_immutableData(nullptr)
    { }

    constexpr
    explicit View(const TType& data) noexcept
      : m_immutableData(&data)
    { }

    constexpr
    explicit View(const TType* data) noexcept
      : m_immutableData(data)
    { }

    constexpr
    bool operator==(const View& other) const noexcept {
      return m_immutableData == other.m_immutableData;
    }

    constexpr
    bool operator!=(const View& other) const noexcept {
      return !operator==(other);
    }

    constexpr
    bool operator<(const View& other) const noexcept {
      return m_immutableData < other.m_immutableData;
    }

    constexpr
    const TType& operator*() const noexcept {
      return *m_immutableData;
    }

    constexpr
    const TType* operator->() const noexcept {
      return m_immutableData;
    }

    constexpr
    const TType* get() const noexcept {
      return m_immutableData;
    }

  private:
    const TType* m_immutableData;
  };

}

#endif /* __INC_HEARTH_MEMORY_VIEW_HPP__ */
