#ifndef __lib_fixed_vector_hpp__
#define __lib_fixed_vector_hpp__

#include <lib/strong.hpp>
#include <lib/basic_types.hpp>
#include <lib/utility.hpp>
#include <lib/range.hpp>

namespace lib
{
  template <typename T>
  class FixedVector
  {
    Size lgth;
    Size max;
    Strong<T[]> storage;

  public:
    template <typename... U>
    static constexpr FixedVector from(U &&...us) noexcept
    {
      FixedVector v(sizeof...(U));
      (v.push_back(forward<U>(us)), ...);
      return v;
    }

  public:
    constexpr explicit FixedVector(Size _max) noexcept
        : lgth(0),
          max(_max),
          storage(new T[max]) {}

    template <typename IT>
    constexpr FixedVector(IT b, IT e) noexcept
        : FixedVector()
    {
      append(b, e);
    }

    constexpr FixedVector(const FixedVector &o) noexcept
        : lgth(o.lgth),
          max(o.max),
          storage(new T[max])
    {
      for (Size i = 0; i < lgth; ++i)
        storage[i] = o.storage[i];
    }

    constexpr FixedVector(FixedVector &&o) noexcept
        : lgth(o.lgth),
          max(o.max),
          storage(move(o.storage))
    {
      o.lgth = 0;
      o.max = max;
    }

    constexpr ~FixedVector() noexcept = default;

    constexpr FixedVector &operator=(const FixedVector &o) noexcept
    {
      if (this != &o)
      {
        lgth = 0;

        for (const &i : o)
          push_back(i);
      }

      return *this;
    }

    constexpr FixedVector &operator=(FixedVector &&o) noexcept
    {
      if (this != &o)
      {
        lgth = o.lgth;
        max = o.max;
        storage = move(o.storage);
        o.max = 0;
        o.lgth = 0;
      }

      return *this;
    }

  public:
    constexpr void clear() noexcept
    {
      lgth = 0;
    }

  public:
    constexpr Size size() const noexcept
    {
      return lgth;
    }

    constexpr Size capacity() const noexcept
    {
      return max;
    }

    constexpr bool empty() const noexcept
    {
      return lgth == 0;
    }

    constexpr T *data() noexcept
    {
      return storage;
    }

    constexpr const T *data() const noexcept
    {
      return storage;
    }

  public:
    constexpr void push_back(const T &t) noexcept
    {
      if (lgth < max)
      {
        storage[lgth] = t;
        lgth = lgth + 1;
      }
    }

    constexpr void push_back(T &&t) noexcept
    {
      if (lgth < max)
      {
        storage[lgth] = move(t);
        lgth = lgth + 1;
      }
    }

    constexpr void push_front(const T &t) noexcept
    {
      if (lgth < max)
      {

        for (lib::Size i = lgth; i > 0; --i)
          storage[i] = move(storage[i - 1]);

        storage[0] = t;
        lgth = lgth + 1;
      }
    }

    constexpr void push_front(T &&t) noexcept
    {
      if (lgth < max)
      {
        for (lib::Size i = lgth; i > 0; --i)
          storage[i] = move(storage[i - 1]);

        storage[0] = move(t);
        lgth = lgth + 1;
      }
    }

    constexpr void append(const FixedVector &o) noexcept
    {
      for (const T &t : o)
        push_back(t);
    }

    constexpr void append(FixedVector &&o) noexcept
    {
      for (T &&t : o)
        push_back(move(t));
    }

    template <typename IT>
    constexpr void append(IT b, IT e) noexcept
    {
      while (b != e && lgth < max)
      {
        push_back(*b);
        b = b + 1;
      }
    }

    constexpr T &operator[](Size i) noexcept
    {
      return storage[i];
    }

    const T &operator[](Size i) const noexcept
    {
      return storage[i];
    }

  public:
    constexpr T *begin() noexcept
    {
      return storage;
    }

    constexpr T *end() noexcept
    {
      return static_cast<T *>(storage) + lgth;
    }

    constexpr const T *begin() const noexcept
    {
      return storage;
    }

    constexpr const T *end() const noexcept
    {
      return static_cast<const T *>(storage) + lgth;
    }
  };
}
#endif