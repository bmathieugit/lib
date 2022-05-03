#ifndef __lib_utility_hpp__
#define __lib_utility_hpp__

#include <lib/basic_types.hpp>
#include <lib/meta.hpp>

namespace lib
{

  template<typename T>
  requires NotConst<T>
  constexpr decltype(auto) move(T&& t) noexcept
  {
    return static_cast<RemoveReference<T>&&>(t);
  }

  template <typename T>
  const T& as_const(T &&t)
  {
    return t;
  }

  class CStringUtils
  {
  public:
    template <typename C>
    static constexpr Size length(const C *ncstring)
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