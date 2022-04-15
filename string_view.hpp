#ifndef __lib_string_view_hpp__
#define __lib_string_view_hpp__

#include <lib/basic_types.hpp>
#include <lib/utility.hpp>
#include <lib/algorithm.hpp>

namespace lib
{
  template <typename C>
  class BasicStringView
  {
  private:
    const C *b = nullptr;
    Size lgth = 0;

  public:
    constexpr BasicStringView() = default;

    constexpr BasicStringView(const C *s, Size l)
        : b{s},
          lgth{l}
    {
    }

    constexpr BasicStringView(const C *s)
        : BasicStringView(s, CStringUtils::length(s))
    {
    }

    constexpr BasicStringView(const C *_b, const C *_e)
        : BasicStringView(_b, _e - _b < 0 ? 0 : _e - _b)
    {
    }

    constexpr BasicStringView(const BasicStringView &) = default;
    constexpr BasicStringView(BasicStringView &&) = default;
    constexpr ~BasicStringView() = default;

    constexpr BasicStringView &operator=(const BasicStringView &) = default;
    constexpr BasicStringView &operator=(BasicStringView &&) = default;

  public:
    decltype(auto) apply(auto &&algorithm, auto &&...args)
    {
      return algorithm(begin(), end(), args...);
    }

    decltype(auto) apply(auto &&algorithm, auto &&...args) const
    {
      return algorithm(begin(), end(), args...);
    }

  public:
    constexpr bool operator==(const BasicStringView &o) const
    {
      return apply(EqualsAlgorithm(), o.begin(), o.end());
    }

    constexpr bool operator==(const C *o) const
    {
      return *this == BasicStringView(o);
    }

    constexpr bool operator!=(const BasicStringView &o) const
    {
      return !(*this == o);
    }

    constexpr bool operator!=(const C *o) const
    {
      return !(*this == o);
    }

    constexpr bool starts_with(const BasicStringView &o) const
    {
      return apply(StartsWithAlgorithm(), o.begin(), o.end());
    }

    constexpr bool starts_with(const C *o) const
    {
      return starts_with(BasicStringView(o));
    }

  public:
    constexpr Size size() const
    {
      return lgth;
    }

    constexpr bool empty() const
    {
      return lgth == 0;
    }

    constexpr const C &operator[](Size i) const
    {
      return b[i];
    }

  public:
    constexpr BasicStringView after(C c) const
    {
      return BasicStringView(apply(AfterAlgorithm(), c), end());
    }

    constexpr BasicStringView before(C c) const
    {
      return BasicStringView(begin(), apply(BeforeAlgorithm(), c));
    }

    constexpr decltype(auto) around(C c) const
    {
      return apply(AroundAlgorithm<BasicStringView<C>>(), c);
    }

  public:
    constexpr const C *begin() const
    {
      return b;
    }

    constexpr const C *end() const
    {
      return b + lgth;
    }

    constexpr const C *cbegin() const
    {
      return b;
    }

    constexpr const C *cend() const
    {
      return b + lgth;
    }

    constexpr const C *data() const
    {
      return b;
    }
  };

  using StringView = BasicStringView<char>;
}

#endif