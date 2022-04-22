#ifndef __lib_range_hpp__
#define __lib_range_hpp__

#include <lib/algorithm.hpp>
#include <lib/meta.hpp>

namespace lib
{
  template <Rangeable C>
  class Range
  {
    decltype(C{}.begin()) b;
    decltype(C{}.end()) e;

  public:
    explicit Range(C &_c)
        : b(_c.begin()),
          e(_c.end())
    {
    }

    Range(const Range &) = default;
    Range(Range &&) = default;
    ~Range() = default;
    Range &operator=(const Range &) = default;
    Range &operator=(Range &&) = default;

  public:
    auto size() const
    {
      return e - b;
    }

    bool empty() const
    {
      return b == e;
    }

  public:
    auto begin()
    {
      return b;
    }

    auto end()
    {
      return e;
    }

    auto begin() const
    {
      return b;
    }

    auto end() const
    {
      return e;
    }

  public:
    decltype(auto) apply(auto &&algorithm, auto &&...args)
    {
      return algorithm(begin(), end(), args...);
    }

    decltype(auto) apply(auto &&algorithm, auto &&...args) const
    {
      return algorithm(begin(), end(), args...);
    }

    template <typename C2>
    constexpr bool starts_with(const Range<C2> &o) const
    {
      return apply(StartsWithAlgorithm(), o.begin(), o.end());
    }

    constexpr Range after(const decltype(*C{}.begin()) & t) const
    {
      return Range(apply(AfterAlgorithm(), t), end());
    }

    constexpr Range before(const decltype(*C{}.begin()) & t) const
    {
      return Range(apply(BeforeAlgorithm(), t), end());
    }

    constexpr decltype(auto) around(const decltype(*C{}.begin()) & t) const
    {
      return apply(AroundAlgorithm<Range<C>>(), t);
    }
  };

  bool operator==(const Rangeable auto &r, const Rangeable auto &o)
  {
    return lib::EqualsAlgorithm()(r.begin(), r.end(), o.begin(), o.end());
  }

  bool operator!=(const Rangeable auto &r, const Rangeable auto &o)
  {
    return !(r == o);
  }

}

#endif