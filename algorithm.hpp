#ifndef __lib_algorithm_hpp__
#define __lib_algorithm_hpp__

#include <functional>
#include <lib/basic_types.hpp>

namespace lib::op
{
  constexpr auto Equals = [](const auto &t)
  {
    return [&](auto &&o)
    {
      return o == t;
    };
  };

  constexpr auto NotEquals = [](const auto &t)
  {
    return [&](auto &&o)
    {
      return !(o == t);
    };
  };

  constexpr auto Less = [](const auto &t)
  {
    return [&](auto &&o)
    {
      return o < t;
    };
  };

  constexpr auto LessEquals = [](const auto &t)
  {
    return [&](auto &&o)
    {
      return o <= t;
    };
  };

  constexpr auto Greater = [](const auto &t)
  {
    return [&](auto &&o)
    {
      return o > t;
    };
  };

  constexpr auto GreaterEquals = [](const auto &t)
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
    constexpr IT operator()(IT b, IT e, P pred) const
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
    constexpr IT operator()(IT b, IT e, const T &t) const
    {
      return FindIfAlgorithm()(b, e, lib::op::Equals(t));
    }
  };

  struct FindIfNotAlgorithm
  {
    template <typename IT, typename P>
    constexpr IT operator()(IT b, IT e, P pred) const
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
    constexpr IT operator()(IT b, IT e, P pred) const
    {
      auto tmp = FindIfAlgorithm()(b, e, pred);
      return tmp == e ? e : ++tmp;
    }
  };

  struct AfterAlgorithm
  {
    template <typename IT, typename T>
    constexpr IT operator()(IT b, IT e, const T &t) const
    {
      return AfterIfAlgorithm()(b, e, lib::op::Equals(t));
    }
  };

  struct BeforeIfAlgorithm
  {
    template <typename IT, typename P>
    constexpr IT operator()(IT b, IT e, P pred) const
    {
      auto tmp = FindIfAlgorithm()(b, e, pred);
      return tmp == b ? b : tmp--;
    }
  };

  struct BeforeAlgorithm
  {
    template <typename IT, typename T>
    constexpr IT operator()(IT b, IT e, const T &t) const
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
    constexpr AroundResult operator()(IT b, IT e, P pred) const
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
    constexpr decltype(auto) operator()(IT b, IT e, const T &t) const
    {
      return AroundIfAlgorithm<R>()(b, e, lib::op::Equals(t));
    }
  };

  struct CountIfAlgorithm
  {
    template <typename IT, typename P>
    constexpr Size operator()(IT b, IT e, P pred) const
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
    constexpr Size operator()(IT b, IT e, const T &t) const
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
        IT b, IT e, IT2 b2, IT2 e2) const
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
    constexpr bool operator()(IT b, IT e, IT2 b2, IT2 e2) const
    {
      auto &&[r1, r2] = MismatchAlgorithm()(b, e, b2, e2);
      return r1 == e && r2 == e2;
    }
  };

  struct StartsWithAlgorithm
  {
    template <typename IT, typename IT2>
    constexpr bool operator()(IT b, IT e, IT2 b2, IT2 e2) const
    {
      return MismatchAlgorithm()(b, e, b2, e2).second == e2;
    }
  };

  struct AllOfAlgorithm
  {
    template <typename IT, typename P>
    constexpr bool operator()(IT b, IT e, P pred) const
    {
      return FindIfNotAlgorithm()(b, e, pred) == e;
    }
  };

  struct AnyOfAlgorithm
  {
    template <typename IT, typename P>
    constexpr bool operator()(IT b, IT e, P pred) const
    {
      return FindIfAlgorithm()(b, e, pred) != e;
    }
  };

  struct NoneOfAlgorithm
  {
    template <typename IT, typename P>
    constexpr bool operator()(IT b, IT e, P pred) const
    {
      return FindIfAlgorithm()(b, e, pred) == e;
    }
  };

}

#endif