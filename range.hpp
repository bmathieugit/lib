#ifndef __lib_range_hpp__
#define __lib_range_hpp__

#include <lib/algorithm.hpp>
#include <lib/meta.hpp>
#include <lib/basic_types.hpp>
#include <lib/utility.hpp>

namespace lib
{
  template <typename T>
  concept Rangeable = requires(const T &t, T &t2, T &&t3)
  {
    t.begin();
    t.end();

    t2.begin();
    t2.end();

    t3.begin();
    t3.end();
  };

  template <typename IT>
  class Range
  {
    IT b;
    IT e;

  public:
    template <Rangeable R>
    constexpr Range(R &r) noexcept
        : b(r.begin()), e(r.end()) {}
    constexpr Range(IT _b, IT _e) noexcept
        : b(_b), e(_e) {}
    constexpr Range(const Range &) noexcept = default;
    constexpr Range(Range &&) noexcept = default;
    constexpr ~Range() noexcept = default;
    constexpr Range &operator=(const Range &) noexcept = default;
    constexpr Range &operator=(Range &&) noexcept = default;

  public:
    constexpr bool empty() const noexcept
    {
      return b == e;
    }

    constexpr auto begin() noexcept
    {
      return b;
    }

    constexpr auto end() noexcept
    {
      return e;
    }

    constexpr auto begin() const noexcept
    {
      return b;
    }

    constexpr auto end() const noexcept
    {
      return e;
    }

  public:
    template <Rangeable R>
    constexpr R as() noexcept
    {
      return R(begin(), end());
    }

    template <Rangeable R>
    constexpr const R as() const noexcept
    {
      return R(begin(), end());
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
    constexpr Size count() const noexcept
    {
      return apply(CountIfAlgorithm(),
                   [](const RemoveReference<decltype(*declval<IT>())> &)
                   { return true; });
    }

    constexpr Size count(const RemoveReference<decltype(*declval<IT>())> &t) const noexcept
    {
      return apply(CountAlgorithm(), t);
    }

    constexpr Size count_if(auto &&pred) const noexcept
    {
      return apply(CountIfAlgorithm(), pred);
    }

    constexpr auto find(const RemoveReference<decltype(*declval<IT>())> &t) noexcept
    {
      return apply(FindAlgorithm(), t);
    }

    constexpr auto find(const RemoveReference<decltype(*declval<IT>())> &t) const noexcept
    {
      return apply(FindAlgorithm(), t);
    }

    constexpr auto find_if(auto &&pred) noexcept
    {
      return apply(FindIfAlgorithm(), pred);
    }

    constexpr auto find_if(auto &&pred) const noexcept
    {
      return apply(FindIfAlgorithm(), pred);
    }

    constexpr auto find_if_not(auto &&pred) noexcept
    {
      return apply(FindIfNotAlgorithm(), pred);
    }

    constexpr auto find_if_not(auto &&pred) const noexcept
    {
      return apply(FindIfNotAlgorithm(), pred);
    }

    constexpr bool starts_with(const Rangeable auto &o) const noexcept
    {
      return apply(StartsWithAlgorithm(), o.begin(), o.end());
    }

    constexpr Range after_if(auto &&pred) noexcept
    {
      return Range(apply(AfterIfAlgorithm(), pred), end());
    }

    constexpr const Range after_if(auto &&pred) const noexcept
    {
      return Range(apply(AfterIfAlgorithm(), pred), end());
    }

    constexpr Range before_if(auto &&pred) noexcept
    {
      return Range(begin(), apply(BeforeIfAlgorithm(), pred));
    }

    constexpr const Range before_if(auto &&pred) const noexcept
    {
      return Range(begin(), apply(BeforeIfAlgorithm(), pred));
    }

    constexpr decltype(auto) around_if(auto &&pred) noexcept
    {
      return apply(AroundIfAlgorithm<Range>(), pred);
    }

    constexpr decltype(auto) around_if(auto &&pred) const noexcept
    {
      return apply(AroundIfAlgorithm<const Range>(), pred);
    }

    constexpr Range after(const RemoveReference<decltype(*declval<IT>())> &t) noexcept
    {
      return Range(apply(AfterAlgorithm(), t), end());
    }

    constexpr const Range after(const RemoveReference<decltype(*declval<IT>())> &t) const noexcept
    {
      return Range(apply(AfterAlgorithm(), t), end());
    }

    constexpr Range before(const RemoveReference<decltype(*declval<IT>())> &t) noexcept
    {
      return Range(begin(), apply(BeforeAlgorithm(), t));
    }

    constexpr const Range before(const RemoveReference<decltype(*declval<IT>())> &t) const noexcept
    {
      return Range(begin(), apply(BeforeAlgorithm(), t));
    }

    constexpr decltype(auto) around(const RemoveReference<decltype(*declval<IT>())> &t) noexcept
    {
      return apply(AroundAlgorithm<Range>(), t);
    }

    constexpr decltype(auto) around(const RemoveReference<decltype(*declval<IT>())> &t) const noexcept
    {
      return apply(AroundAlgorithm<const Range>(), t);
    }

    constexpr bool all_of(auto &&pred) const noexcept
    {
      return apply(AllOfAlgorithm(), pred);
    }

    constexpr bool any_of(auto &&pred) const noexcept
    {
      return apply(AnyOfAlgorithm(), pred);
    }

    constexpr bool none_of(auto &&pred) const noexcept
    {
      return apply(NoneOfAlgorithm(), pred);
    }
  };

  template <Rangeable C>
  constexpr auto rangeof(C &c) -> Range<decltype(declval<C>().begin())>
  {
    return {c};
  }

  template <Rangeable C>
  constexpr auto rangeof(const C &c) -> const Range<decltype(declval<C>().begin())>
  {
    return {c};
  }

  template <typename T, Size n>
  constexpr const Range<T *> rangeof(T (&o)[n])
  {
    return {o, o + n};
  }

  template <typename T, Size n>
  constexpr const Range<const T *> rangeof(const T (&o)[n])
  {
    return {o, o + n};
  }

  template <typename T>
  constexpr Range<T *> rangeof(T *o, Size n)
  {
    return {o, o + n};
  }

  template <typename T>
  constexpr const Range<const T *> rangeof(const T *o, Size n)
  {
    return {o, o + n};
  }

  constexpr bool operator==(const Rangeable auto &r, const Rangeable auto &o)
  {
    return EqualsAlgorithm()(r.begin(), r.end(), o.begin(), o.end());
  }

  template <typename T, Size n>
  constexpr bool operator==(const Rangeable auto &r, const T (&o)[n])
  {
    return r == rangeof(o);
  }

  constexpr bool operator!=(const Rangeable auto &r, const Rangeable auto &o)
  {
    return !(r == o);
  }

  template <typename T, Size n>
  constexpr bool operator!=(const Rangeable auto &r, const T (&o)[n])
  {
    return !(r == rangeof(o));
  }
}

#endif