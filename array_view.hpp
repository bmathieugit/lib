#ifndef __lib_array_view_hpp__
#define __lib_array_view_hpp__

#include <lib/basic_types.hpp>
#include <lib/algorithm.hpp>
#include <lib/meta.hpp>
#include <lib/range.hpp>

namespace lib
{
  template <typename T, typename LENGTH = void>
  using ArrayView = Range<const T *, LENGTH>;
}

#endif