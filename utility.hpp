#ifndef __lib_utility_hpp__
#define __lib_utility_hpp__

#include <lib/basic_types.hpp>
#include <lib/meta.hpp>

namespace lib
{

  template <typename T>
  requires NotConst<T>
  constexpr decltype(auto) move(T &&t) noexcept
  {
    return static_cast<RemoveReference<T> &&>(t);
  }

  template <class T>
  constexpr T &&forward(RemoveReference<T> &t) noexcept
  {
    return static_cast<T &&>(t);
  }

  template <class T>
  constexpr T &&forward(RemoveReference<T> &&t) noexcept
  {
    // static_assert(!std::is_lvalue_reference<T>::value,
    //               "Can not forward an rvalue as an lvalue.");
    return static_cast<T &&>(t);
  }

  template <typename T>
  constexpr T &&declval();

  template <typename T>
  const T &as_const(T &&t) noexcept
  {
    return t;
  }

  template <typename C>
  struct StrLen
  {
    constexpr Size operator()(const char *cs) const noexcept
    {
      Size i = 0;

      if (cs != nullptr)
        while (cs[i] != '\0')
          ++i;

      return i;
    }
  };

  class CStringUtils
  {
  public:
    template <typename C>
    static constexpr Size length(const C *ncstring) noexcept
    {
      Size i = 0;

      if (ncstring != nullptr)
        while (ncstring[i] != '\0')
          ++i;

      return i;
    }
  };
}

#endif