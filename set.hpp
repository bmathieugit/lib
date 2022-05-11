#ifndef __lib_set_hpp__
#define __lib_set_hpp__

#include <lib/list.hpp>
#include <lib/basic_types.hpp>
#include <lib/range.hpp>

namespace lib
{
  template <typename T>
  concept LessComparable = requires(const T &t1, const T &t2)
  {
    t1 == t2;
    t1 != t2;
    t1 <= t2;
    t1 < t2;
  };

  template <LessComparable T>
  class Set
  {
    List<T> storage;

  public:
    template <typename... U>
    static constexpr Set from(U &&...us) noexcept
    {
      Set s(sizeof...(U));
      (s.push(forward<U>(us)), ...);
      return s;
    }

  public:
    Set() noexcept = default;
    Set(Size _max) noexcept
        : storage(_max)
    {
    }

    template <typename IT>
    Set(IT b, IT e) noexcept
        : Set()
    {
      append(b, e);
    }

    Set(const Set &) noexcept = default;
    Set(Set &&) noexcept = default;
    ~Set() noexcept = default;
    Set &operator=(const Set &) noexcept = default;
    Set &operator=(Set &&) noexcept = default;

  public:
    constexpr auto range() noexcept
    {
      return rangeof(*this);
    }

    constexpr auto range() const noexcept
    {
      return rangeof(*this);
    }

    Size size() const noexcept
    {
      return storage.size();
    }

    Size capacity() const noexcept
    {
      return storage.capacity();
    }

    bool empty() const noexcept
    {
      return storage.empty();
    }

    void push(T &&t)
    {
      auto it = rangeof(*this).find_if(
          [&t](const T &o) noexcept
          { return t <= o; });

      if (it == end() || *it != t)
        storage.insert(it, static_cast<T &&>(t));
    }

    void push(const T &t) noexcept
    {
      push(static_cast<T &&>(T(t)));
    }

    template <typename IT>
    void append(IT b, IT e) noexcept
    {
      while (b != e)
      {
        push(*b);
        ++b;
      }
    }

  public:
    auto begin() noexcept
    {
      return storage.begin();
    }

    auto end() noexcept
    {
      return storage.end();
    }

    auto begin() const noexcept
    {
      return storage.begin();
    }

    auto end() const noexcept
    {
      return storage.end();
    }
  };
}

#endif