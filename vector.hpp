#ifndef __lib_vector_hpp__
#define __lib_vector_hpp__

#include <lib/basic_types.hpp>
#include <lib/algorithm.hpp>
#include <lib/range.hpp>
#include <lib/strong.hpp>
#include <lib/utility.hpp>

#include <initializer_list>

namespace lib
{
  template <typename T>
  class Vector
  {
  private:
    Size lgth = 0;
    Size max = 0;
    Strong<T[]> storage;

  public:
    Vector() = default;

    explicit Vector(Size _max)
        : lgth(0),
          max(_max),
          storage{new T[max]}
    {
    }

    Vector(std::initializer_list<T> init)
        : Vector(init.size())
    {
      append(init.begin(), init.end());
    }

    template <typename IT>
    Vector(IT b, IT e)
        : Vector()
    {
      append(b, e);
    }

    Vector(Strong<T[]> &&fb, Size lgth)
        : lgth(lgth),
          max(lgth),
          storage(move(fb))
    {
    }

    Vector(const Vector &o)
        : lgth(o.lgth),
          max(o.max),
          storage(new T[max])
    {
      for (lib::Size i = 0; i < lgth; ++i)
        storage[i] = o.storage[i];
    }

    Vector(Vector &&o)
        : lgth(o.lgth),
          max(o.max),
          storage(move(o.storage))
    {
      o.lgth = 0;
      o.max = 0;
    }

    ~Vector() = default;

    Vector &operator=(const Vector &o)
    {
      if (this != &o)
      {
        lgth = o.lgth;
        max = o.max;
        storage = new T[max];

        for (lib::Size i = 0; i < lgth; ++i)
          storage[i] = o.storage[i];
      }

      return *this;
    }

    Vector &operator=(Vector &&o)
    {
      if (this != &o)
      {
        lgth = o.lgth;
        max = o.max;
        storage = new T[max];

        for (lib::Size i = 0; i < lgth; ++i)
          storage[i] = move(o.storage[i]);
      }

      return *this;
    }

  public:
    Size size() const
    {
      return lgth;
    }

    Size capacity() const
    {
      return max;
    }

    bool empty() const
    {
      return lgth == 0;
    }

    T *data()
    {
      return storage;
    }

    const T *data() const
    {
      return storage;
    }

  protected:
    void increase()
    {
      if (max == 0)
        max = 10;

      Strong<T[]> nstorage = new T[max * 2];

      for (Size i = 0; i < lgth; ++i)
        nstorage[i] = move(storage[i]);

      storage = move(nstorage);
      max = max * 2;
    }

  public:
    void clear()
    {
      storage = move(Strong<T[]>());
      max = 0;
      lgth = 0;
    }

  public:
    void push_back(const T &t)
    {
      if (lgth >= max)
        increase();

      storage[size()] = t;
      lgth = lgth + 1;
    }

    void push_back(T &&t)
    {
      if (lgth >= max)
        increase();

      storage[size()] = static_cast<T &&>(t);
      lgth = lgth + 1;
    }

    void push_front(const T &t)
    {
      if (lgth >= max)
        increase();

      for (lib::Size i = lgth; i > 0; --i)
        storage[i] = static_cast<T &&>(storage[i - 1]);

      storage[0] = t;
      lgth = lgth + 1;
    }

    void push_front(T &&t)
    {
      if (lgth >= max)
        increase();

      for (lib::Size i = lgth; i > 0; --i)
        storage[i] = static_cast<T &&>(storage[i - 1]);

      storage[0] = static_cast<T &&>(t);
      lgth = lgth + 1;
    }

    void append(const Vector &o)
    {
      for (const T &t : o)
        push_back(t);
    }

    void append(Vector &&o)
    {
      for (T &&t : o)
        push_back(static_cast<T &&>(t));
    }

    template <typename IT>
    void append(IT b, IT e)
    {
      while (b != e)
      {
        push_back(*b);
        b = b + 1;
      }
    }

    T &operator[](Size i)
    {
      return storage[i];
    }

    const T &operator[](Size i) const
    {
      return storage[i];
    }

  public:
    T *begin()
    {
      return storage;
    }

    T *end()
    {
      return static_cast<T *>(storage) + lgth;
    }

    const T *begin() const
    {
      return storage;
    }

    const T *end() const
    {
      return static_cast<const T *>(storage) + lgth;
    }
  };
}

#endif