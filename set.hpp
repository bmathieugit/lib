#ifndef __lib_set_hpp__
#define __lib_set_hpp__

#include <lib/list.hpp>
#include <lib/algorithm.hpp>
#include <lib/basic_types.hpp>

#include <initializer_list>

namespace lib
{
  template <typename T>
  class Set
  {
    List<T> storage;

  public:
    Set() = default;
    Set(Size _max)
        : storage(_max)
    {
    }

    template <typename IT>
    Set(IT b, IT e)
        : Set()
    {
      append(b, e);
    }

    Set(std::initializer_list<T> init)
        : Set(init.begin(), init.end())
    {
    }

    Set(const Set &) = default;
    Set(Set &&) = default;
    ~Set() = default;
    Set &operator=(const Set &) = default;
    Set &operator=(Set &&) = default;

  public:
    Size size() const
    {
      return storage.size();
    }

    Size capacity() const
    {
      return storage.capacity();
    }

    bool empty() const
    {
      return storage.empty();
    }

    decltype(auto) apply(auto &&algorithm, auto &&...args)
    {
      return algorithm(begin(), end(), args...);
    }

    decltype(auto) apply(auto &&algorithm, auto &&...args) const
    {
      return algorithm(begin(), end(), args...);
    }

     bool operator==(const Set &o) const
    {
      return apply(lib::EqualsAlgorithm(), o.begin(), o.end());
    }

    bool operator!=(const Set &o) const
    {
      return !(*this == o);
    }
    
    void push(T &&t)
    {
      auto it = apply(lib::FindIfAlgorithm(), [&t](const T &o)
                      { return t <= o; });
      if (it == end() || *it != t)
        storage.insert(it, static_cast<T &&>(t));
    }

    void push(const T &t)
    {
      push(static_cast<T &&>(T(t)));
    }

    template <typename IT>
    void append(IT b, IT e)
    {
      while (b != e)
      {
        push(*b);
        ++b;
      }
    }

  public:
    auto begin()
    {
      return storage.begin();
    }

    auto end()
    {
      return storage.end();
    }

    auto begin() const
    {
      return storage.begin();
    }

    auto end() const
    {
      return storage.end();
    }
  };
}

#endif