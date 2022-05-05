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
    static constexpr Span from(T *t, Size n) noexcept
    {
      return Span(t, n);
    }

    static constexpr Span from(T *t, auto &&length) noexcept
    {
      return Span(t, length(t));
    }

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
    constexpr auto range() noexcept
    {
      return rangeof(*this);
    }

    constexpr auto range() const noexcept
    {
      return rangeof(*this);
    }

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

  public:
    constexpr const Span sub(Size start) const noexcept
    {
      auto bbound = begin() + start < end() ? begin() + start : end();
      auto ebound = end();
      return Span(bbound, ebound);
    }

    constexpr const Span sub(Size start, Size count) const
    {
      auto bbound = begin() + start < end() ? begin() + start : end();
      auto ebound = bbound + count < end() ? bbound + count : end();
      return Span(bbound, ebound);
    }

    constexpr Span sub(Size start) noexcept
    {
      auto bbound = begin() + start < end() ? begin() + start : end();
      auto ebound = end();
      return Span(bbound, ebound);
    }

    constexpr Span sub(Size start, Size count) noexcept
    {
      auto bbound = begin() + start < end() ? begin() + start : end();
      auto ebound = bbound + count < end() ? bbound + count : end();
      return Span(bbound, ebound);
    }

    struct SplitSpan
    {
      Span before;
      Span after;
    };

    constexpr SplitSpan split(Size position) noexcept
    {
      return SplitSpan{sub(0, position), sub(position)};
    }

    constexpr const SplitSpan split(Size position) const noexcept
    {
      return SplitSpan{sub(0, position), sub(position)};
    }
  };
}
#endif