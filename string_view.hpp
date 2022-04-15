#ifndef __lib_string_view_hpp__
#define __lib_string_view_hpp__

#include <lib/basic_types.hpp>
#include <lib/utility.hpp>
#include <lib/algorithm.hpp>
#include <lib/array_view.hpp>

namespace lib
{
  template <typename C>
  struct BasicStringViewLength
  {
    constexpr Size operator()(const C *o) const
    {
      return CStringUtils::length(o);
    }
  };

  template <typename C>
  using BasicStringView = ArrayView<C, BasicStringViewLength<C>>;

  using StringView = BasicStringView<char>;
}

#endif