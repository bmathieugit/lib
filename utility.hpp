#ifndef __lib_utility_hpp__
#define __lib_utility_hpp__

#include <lib/basic_types.hpp>

namespace lib
{
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