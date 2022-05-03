#ifndef __lib_range_hpp__
#define __lib_range_hpp__

#include <lib/algorithm.hpp>
#include <lib/meta.hpp>
#include <lib/basic_types.hpp>

namespace lib
{
  template <typename T, typename LENGTH>
  concept LengthNativeArray = requires(LENGTH l, const T *o)
  {
    {
      l(o)
      } -> same_as<Size>;
  };

  template <typename IT, typename LENGTH = void>
  class Range
  {
    IT b;
    IT e;

  public:
    constexpr Range() : e(b) {}

    constexpr Range(IT _b, IT _e)
        : b(_b),
          e(_e)
    {
    }

    template <Size n>
    constexpr Range(const RemoveConstVolatilReference<decltype(*IT{})> (&o)[n])
        : Range(o, o + n)
    {
    }

    constexpr Range(const RemoveConstVolatilReference<decltype(*IT{})> *o, Size n)
        : Range(o, o + n)
    {
    }

    constexpr Range(const RemoveConstVolatilReference<decltype(*IT{})> *o) requires LengthNativeArray<RemoveConstVolatilReference<decltype(*IT{})>, LENGTH>
        : Range(o, LENGTH{}(o))
    {
    }

    constexpr Range(Rangeable auto &c)
        : Range(c.begin(), c.end())
    {
    }

    constexpr Range(const Range &) = default;
    constexpr Range(Range &&) = default;
    constexpr ~Range() = default;
    constexpr Range &operator=(const Range &) = default;
    constexpr Range &operator=(Range &&) = default;

  public:
    Size size() const
    {
      auto dist = e - b;
      return dist >= 0 ? dist : -dist;
    }

    bool empty() const
    {
      return size() == 0;
    }

  public:
    constexpr auto begin()
    {
      return b;
    }

    constexpr auto end()
    {
      return e;
    }

    constexpr auto begin() const
    {
      return b;
    }

    constexpr auto end() const
    {
      return e;
    }

    // FIXME: mettre en place un requires b + i  valide
    constexpr auto &operator[](Size i)
    {
      return *(b + i);
    }

    constexpr const auto &operator[](Size i) const
    {
      return *(b + i);
    }

  public:
    constexpr decltype(auto) apply(auto &&algorithm, auto &&...args)
    {
      return algorithm(begin(), end(), args...);
    }

    constexpr decltype(auto) apply(auto &&algorithm, auto &&...args) const
    {
      return algorithm(begin(), end(), args...);
    }

  public:
    constexpr Size count() const
    {
      return apply(CountIfAlgorithm(), [](auto &&)
                   { return true; });
    }

    constexpr Size count(const decltype(*IT{}) & t) const
    {
      return apply(CountAlgorithm(), t);
    }

    constexpr Size count_if(auto &&pred) const
    {
      return apply(CountAlgorithm(), pred);
    }

    constexpr auto find(const decltype(*IT{}) & t)
    {
      return apply(FindAlgorithm(), t);
    }

    constexpr auto find(const decltype(*IT{}) & t) const
    {
      return apply(FindAlgorithm(), t);
    }

    constexpr auto find_if(auto &&pred)
    {
      return apply(FindIfAlgorithm(), pred);
    }

    constexpr auto find_if(auto &&pred) const
    {
      return apply(FindIfAlgorithm(), pred);
    }

    constexpr auto find_if_not(auto &&pred)
    {
      return apply(FindIfNotAlgorithm(), pred);
    }

    constexpr auto find_if_not(auto &&pred) const
    {
      return apply(FindIfNotAlgorithm(), pred);
    }

    constexpr bool starts_with(const Rangeable auto &o) const
    {
      return apply(StartsWithAlgorithm(), o.begin(), o.end());
    }

    constexpr Range after_if(auto &&pred)
    {
      return Range(apply(AfterIfAlgorithm(), pred), end());
    }

    constexpr const Range after_if(auto &&pred) const
    {
      return Range(apply(AfterIfAlgorithm(), pred), end());
    }

    constexpr Range before_if(auto &&pred)
    {
      return Range(begin(), apply(BeforeIfAlgorithm(), pred));
    }

    constexpr const Range before_if(auto &&pred) const
    {
      return Range(begin(), apply(BeforeIfAlgorithm(), pred));
    }

    constexpr decltype(auto) around_if(auto &&pred)
    {
      return apply(AroundIfAlgorithm<Range>(), pred);
    }

    constexpr decltype(auto) around_if(auto &&pred) const
    {
      return apply(AroundIfAlgorithm<const Range>(), pred);
    }

    constexpr Range after(const decltype(*IT{}) & t)
    {
      return Range(apply(AfterAlgorithm(), t), end());
    }

    constexpr const Range after(const decltype(*IT{}) & t) const
    {
      return Range(apply(AfterAlgorithm(), t), end());
    }

    constexpr Range before(const decltype(*IT{}) & t)
    {
      return Range(begin(), apply(BeforeAlgorithm(), t));
    }

    constexpr const Range before(const decltype(*IT{}) & t) const
    {
      return Range(begin(), apply(BeforeAlgorithm(), t));
    }

    constexpr decltype(auto) around(const decltype(*IT{}) & t)
    {
      return apply(AroundAlgorithm<Range>(), t);
    }

    constexpr decltype(auto) around(const decltype(*IT{}) & t) const
    {
      return apply(AroundAlgorithm<const Range>(), t);
    }

    constexpr bool all_of(auto &&pred) const
    {
      return apply(AllOfAlgorithm(), pred);
    }

    constexpr bool any_of(auto &&pred) const
    {
      return apply(AnyOfAlgorithm(), pred);
    }

    constexpr bool none_of(auto &&pred) const
    {
      return apply(NoneOfAlgorithm(), pred);
    }
  };

  template <Rangeable C>
  auto from(C &c) -> Range<decltype(C{}.begin())>
  {
    return {c};
  }

  template <Rangeable C>
  auto from(const C &c) -> const Range<decltype(C{}.begin())>
  {
    return {c};
  }

  template <typename T, Size n>
  const Range<const T *> from(const T (&o)[n])
  {
    return {o, o + n};
  }

  bool operator==(const Rangeable auto &r, const Rangeable auto &o)
  {
    return lib::EqualsAlgorithm()(r.begin(), r.end(), o.begin(), o.end());
  }

  template <typename T, Size n>
  bool operator==(const Rangeable auto &r, const T (&o)[n])
  {
    return r == from(o);
  }

  bool operator!=(const Rangeable auto &r, const Rangeable auto &o)
  {
    return !(r == o);
  }

}

#endif