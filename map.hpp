#ifndef __lib_map_hpp__
#define __lib_map_hpp__

#include <lib/set.hpp>

namespace lib
{
  template <typename K, typename V>
  class Map
  {
    struct Pair
    {
      K key;
      V value;

      bool operator<=(const Pair &o)
      {
        return key <= o.key;
      }
    };

    Set<Pair<K, V>> keys;

  public:
    
  };
}

#endif