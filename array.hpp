#ifndef __lib_array_hpp__
#define __lib_array_hpp__

#include <lib/basic_types.hpp>
#include <lib/range.hpp>
#include <lib/span.hpp>

namespace lib
{
  template <typename T>
  using ArrayView = Span<const T>;

  template <typename T>
  using ArraySpan = Span<T>;

  template <typename T, Size n>
  struct Array
  {
    T b[n];

  public:
    constexpr  auto range() noexcept
    {
      return rangeof(*this);
    }

    constexpr auto range() const noexcept
    {
      return rangeof(*this);
    }

    constexpr Size size() const noexcept
    {
      return n;
    }

    constexpr bool empty() const noexcept
    {
      return false;
    }

    constexpr T *data() noexcept
    {
      return b;
    }

    constexpr const T *data() const noexcept
    {
      return b;
    }

    T &operator[](Size i) & noexcept
    {
      return b[i];
    }

    constexpr const T &operator[](Size i) const & noexcept
    {
      return b[i];
    }

    constexpr T &&operator[](Size i) && noexcept
    {
      return b[i];
    }

    constexpr const T &&operator[](Size i) const && noexcept
    {
      return b[i];
    }

    constexpr operator ArrayView<T>() const noexcept
    {
      return ArrayView<T>(this->data(), this->size());
    }

    constexpr operator ArraySpan<T>() noexcept
    {
      return ArraySpan<T>(this->data(), this->size());
    }

  public:
    constexpr T *begin() noexcept
    {
      return b;
    }

    constexpr T *end() noexcept
    {
      return b + n;
    }

    constexpr const T *begin() const noexcept
    {
      return b;
    }

    constexpr const T *end() const noexcept
    {
      return b + n;
    }
  };
}

#endif