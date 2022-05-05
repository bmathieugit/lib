#ifndef __lib_array_view_hpp__
#define __lib_array_view_hpp__

#include <lib/basic_types.hpp>
#include <lib/meta.hpp>
#include <lib/range.hpp>

namespace lib
{
  template <typename T>
  class Span
  {
    T *b = nullptr;
    Size lgth = 0;

  public:
    constexpr Span() noexcept = default;
    template <Size n>
    constexpr Span(T (&t)[n]) noexcept
        : b(t), lgth(n) {}
    constexpr Span(T *_b, Size _lght) noexcept
        : b(_b), lgth(_lght) {}
    constexpr Span(T *_b, T *_e) noexcept
        : b(_b), lgth(_e - _b) {}
    constexpr Span(const Span &) noexcept = default;
    constexpr Span(Span &&) noexcept = default;
    constexpr ~Span() noexcept = default;
    constexpr Span &operator=(const Span &) noexcept = default;
    constexpr Span &operator=(Span &&) noexcept = default;

  public:
    constexpr Size size() const noexcept
    {
      return lgth;
    }

    constexpr bool empty() const noexcept
    {
      return size() == 0;
    }

    constexpr auto begin() noexcept
    {
      return b;
    }

    constexpr auto end() noexcept
    {
      return b + lgth;
    }

    constexpr auto begin() const noexcept
    {
      return b;
    }

    constexpr auto end() const noexcept
    {
      return b + lgth;
    }

  public:
    constexpr auto &operator[](Size i) noexcept
    {
      return *(b + i);
    }

    constexpr const auto &operator[](Size i) const noexcept
    {
      return *(b + i);
    }
  };
}
#endif