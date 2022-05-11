#ifndef __lib_range_hpp__
#define __lib_range_hpp__

#include <lib/meta.hpp>
#include <lib/basic_types.hpp>
#include <lib/utility.hpp>

namespace lib::op
{
  constexpr auto Not = [](auto &&pred) noexcept
  {
    return [&](auto &&o)
    {
      return !pred(o);
    };
  };

  constexpr auto Equals = [](const auto &t) noexcept
  {
    return [&](auto &&o)
    {
      return o == t;
    };
  };

  constexpr auto NotEquals = [](const auto &t) noexcept
  {
    return Not(Equals(t));
  };

  constexpr auto Less = [](const auto &t) noexcept
  {
    return [&](auto &&o)
    {
      return o < t;
    };
  };

  constexpr auto LessEquals = [](const auto &t) noexcept
  {
    return [&](auto &&o)
    {
      return o <= t;
    };
  };

  constexpr auto Greater = [](const auto &t) noexcept
  {
    return [&](auto &&o)
    {
      return o > t;
    };
  };

  constexpr auto GreaterEquals = [](const auto &t) noexcept
  {
    return [&](auto &&o)
    {
      return o >= t;
    };
  };
}

namespace lib
{

  struct FindIfAlgorithm
  {
    template <typename IT, typename P>
    constexpr IT operator()(IT b, IT e, P pred) const noexcept
    {
      while (b != e)
        if (pred(*b))
          return b;
        else
          ++b;

      return b;
    }
  };

  struct FindAlgorithm
  {
    template <typename IT, typename T>
    constexpr IT operator()(IT b, IT e, const T &t) const noexcept
    {
      return FindIfAlgorithm()(b, e, lib::op::Equals(t));
    }
  };

  struct FindIfNotAlgorithm
  {
    template <typename IT, typename P>
    constexpr IT operator()(IT b, IT e, P pred) const noexcept
    {
      while (b != e)
        if (!pred(*b))
          return b;
        else
          ++b;

      return b;
    }
  };

  struct AfterIfAlgorithm
  {
    template <typename IT, typename P>
    constexpr IT operator()(IT b, IT e, P pred) const noexcept
    {
      auto tmp = FindIfAlgorithm()(b, e, pred);
      return tmp == e ? e : ++tmp;
    }
  };

  struct AfterAlgorithm
  {
    template <typename IT, typename T>
    constexpr IT operator()(IT b, IT e, const T &t) const noexcept
    {
      return AfterIfAlgorithm()(b, e, lib::op::Equals(t));
    }
  };

  struct BeforeIfAlgorithm
  {
    template <typename IT, typename P>
    constexpr IT operator()(IT b, IT e, P pred) const noexcept
    {
      auto tmp = FindIfAlgorithm()(b, e, pred);
      return tmp == b ? b : tmp--;
    }
  };

  struct BeforeAlgorithm
  {
    template <typename IT, typename T>
    constexpr IT operator()(IT b, IT e, const T &t) const noexcept
    {
      return BeforeIfAlgorithm()(b, e, lib::op::Equals(t));
    }
  };

  template <typename R>
  struct AroundIfAlgorithm
  {
    struct AroundResult
    {
      R before;
      R after;
    };

    template <typename IT, typename P>
    constexpr AroundResult operator()(IT b, IT e, P pred) const noexcept
    {
      IT fit = FindIfAlgorithm()(b, e, pred);
      return AroundResult{
          R(b, fit), R(fit != e ? ++fit : e, e)};
    }
  };

  template <typename R>
  struct AroundAlgorithm
  {
    template <typename IT, typename T>
    constexpr decltype(auto) operator()(IT b, IT e, const T &t) const noexcept
    {
      return AroundIfAlgorithm<R>()(b, e, lib::op::Equals(t));
    }
  };

  struct CountIfAlgorithm
  {
    template <typename IT, typename P>
    constexpr Size operator()(IT b, IT e, P pred) const noexcept
    {
      Size cnt = 0;

      while (b != e)
      {
        if (pred(*b))
          ++cnt;

        ++b;
      }

      return cnt;
    }
  };

  struct CountAlgorithm
  {
    template <typename IT, typename T>
    constexpr Size operator()(IT b, IT e, const T &t) const noexcept
    {
      return CountIfAlgorithm()(b, e, lib::op::Equals(t));
    }
  };

  struct MismatchAlgorithm
  {
    template <typename IT, typename IT2>
    struct Pair
    {
      IT first;
      IT2 second;
    };

    template <typename IT, typename IT2>
    constexpr MismatchAlgorithm::Pair<IT, IT2> operator()(
        IT b, IT e, IT2 b2, IT2 e2) const noexcept
    {
      while (b != e && b2 != e2)
        if (*b == *b2)
        {
          ++b;
          ++b2;
        }
        else
          break;

      return {b, b2};
    }
  };

  struct EqualsAlgorithm
  {
    template <typename IT, typename IT2>
    constexpr bool operator()(IT b, IT e, IT2 b2, IT2 e2) const noexcept
    {
      auto &&[r1, r2] = MismatchAlgorithm()(b, e, b2, e2);
      return r1 == e && r2 == e2;
    }
  };

  struct StartsWithAlgorithm
  {
    template <typename IT, typename IT2>
    constexpr bool operator()(IT b, IT e, IT2 b2, IT2 e2) const noexcept
    {
      return MismatchAlgorithm()(b, e, b2, e2).second == e2;
    }
  };

  struct AllOfAlgorithm
  {
    template <typename IT, typename P>
    constexpr bool operator()(IT b, IT e, P pred) const noexcept
    {
      return FindIfNotAlgorithm()(b, e, pred) == e;
    }
  };

  struct AnyOfAlgorithm
  {
    template <typename IT, typename P>
    constexpr bool operator()(IT b, IT e, P pred) const noexcept
    {
      return FindIfAlgorithm()(b, e, pred) != e;
    }
  };

  struct NoneOfAlgorithm
  {
    template <typename IT, typename P>
    constexpr bool operator()(IT b, IT e, P pred) const noexcept
    {
      return FindIfAlgorithm()(b, e, pred) == e;
    }
  };
}

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
    constexpr decltype(auto) apply(auto &&algorithm, auto &&...args) noexcept
    {
      return algorithm(begin(), end(), forward<decltype(args) &&>(args)...);
    }

    constexpr decltype(auto) apply(auto &&algorithm, auto &&...args) const noexcept
    {
      return algorithm(begin(), end(), forward<decltype(args) &&>(args)...);
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
  constexpr auto rangeof(C &c) noexcept -> Range<decltype(declval<C>().begin())>
  {
    return {c};
  }

  template <Rangeable C>
  constexpr auto rangeof(const C &c) noexcept -> const Range<decltype(declval<C>().begin())>
  {
    return {c};
  }

  template <typename T, Size n>
  constexpr const Range<T *> rangeof(T (&o)[n]) noexcept
  {
    return {o, o + n};
  }

  template <typename T, Size n>
  constexpr const Range<const T *> rangeof(const T (&o)[n]) noexcept
  {
    return {o, o + n};
  }

  template <typename T>
  constexpr Range<T *> rangeof(T *o, Size n) noexcept
  {
    return {o, o + n};
  }

  template <typename T>
  constexpr const Range<const T *> rangeof(const T *o, Size n) noexcept
  {
    return {o, o + n};
  }

  constexpr bool operator==(const Rangeable auto &r, const Rangeable auto &o) noexcept
  {
    return EqualsAlgorithm()(r.begin(), r.end(), o.begin(), o.end());
  }

  template <typename T, Size n>
  constexpr bool operator==(const Rangeable auto &r, const T (&o)[n]) noexcept
  {
    return r == rangeof(o);
  }

  constexpr bool operator!=(const Rangeable auto &r, const Rangeable auto &o) noexcept
  {
    return !(r == o);
  }

  template <typename T, Size n>
  constexpr bool operator!=(const Rangeable auto &r, const T (&o)[n]) noexcept
  {
    return !(r == rangeof(o));
  }
}

#endif