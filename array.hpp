#ifndef __lib_array_hpp__
#define __lib_array_hpp__

#include <lib/basic_types.hpp>

namespace lib
{
  template <typename T, Size n>
  struct Array
  {
    T b[n];

  public:
    constexpr Size size() const
    {
      return n;
    }

    constexpr bool empty() const
    {
      return false;
    }

    T *data()
    {
      return b;
    }

    constexpr const T *data() const
    {
      return b;
    }

    T &operator[](Size i) &
    {
      return b[i];
    }

    constexpr const T &operator[](Size i) const &
    {
      return b[i];
    }

    T &&operator[](Size i) &&
    {
      return b[i];
    }

    constexpr const T &&operator[](Size i) const &&
    {
      return b[i];
    }

  public:
    T *begin()
    {
      return b;
    }

    T *end()
    {
      return b + n;
    }

    constexpr const T *begin() const
    {
      return b;
    }

    constexpr const T *end() const
    {
      return b + n;
    }

    constexpr const T *cbegin() const
    {
      return b;
    }

    constexpr const T *cend() const
    {
      return b + n;
    }
  };
}

#endif