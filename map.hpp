#ifndef __lib_map_hpp__
#define __lib_map_hpp__

#include <lib/set.hpp>
#include <lib/basic_types.hpp>
#include <lib/algorithm.hpp>

namespace lib
{
  // FIXME: finish this class !!!!!
  template <typename K, typename V>
  class Map
  {
    struct Pair
    {
      K key;
      V value;

      bool operator<=(const Pair &o) const
      {
        return key <= o.key;
      }

      bool operator==(const Pair &o) const
      {
        return key == o.key;
      }

      bool operator!=(const Pair &o) const
      {
        return key != o.key;
      }
    };

    Set<Pair<K, V>> storage;

  public:
    Map() = default;

    Map(Size _max)
        : storage(_max)
    {
    }

    Map(const Map &) = default;
    Map(Map &&) = default;
    Map &operator=(const Map &) = default;
    Map &operator=(Map &&) = default;
    ~Map();

  public:
    Size size() const
    {
      return storage.size();
    }

    bool empty() const
    {
      return storage.empty();
    }

    Size capacity() const
    {
      return storage.capacity();
    }

  public:
    decltype(auto) apply(auto &&algorithm, auto &&...args)
    {
      return algorithm(begin(), end(), args...);
    }

    decltype(auto) apply(auto &&algorithm, auto &&...args) const
    {
      return algorithm(begin(), end(), args...);
    }

    bool operator==(const Map &o) const
    {
      return apply(lib::EqualsAlgorithm(), o.begin(), o.end());
    }

    bool operator!=(const Map &o) const
    {
      return !(*this == o);
    }
  };
}

#endif