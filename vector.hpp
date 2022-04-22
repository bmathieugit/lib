#ifndef __lib_vector_hpp__
#define __lib_vector_hpp__

#include <lib/basic_types.hpp>
#include <lib/algorithm.hpp>
#include <lib/buffer.hpp>
#include <lib/range.hpp>

#include <initializer_list>

namespace lib
{
  template <typename T>
  class Vector
  {
  private:
    Size lgth;
    Size max;
    T *storage;

  public:
    Vector()
        : lgth(0),
          max(0),
          storage(nullptr)
    {
    }

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

    Vector(FlushedBuffer<T> fb)
        : lgth(fb.lgth),
          max(fb.max),
          storage(fb.buff)
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
          storage(o.storage)
    {
      o.lgth = 0;
      o.max = 0;
      o.storage = nullptr;
    }

    ~Vector()
    {
      lgth = 0;
      max = 0;
      delete[] storage;
      storage = nullptr;
    }

    Vector &operator=(const Vector &o)
    {
      if (this != &o)
      {
        delete[] storage;

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
        delete[] storage;
        lgth = o.lgth;
        max = o.max;
        storage = new T[max];

        for (lib::Size i = 0; i < lgth; ++i)
          storage[i] = static_cast<T &&>(o.storage[i]);
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

      T *tmp = storage;
      storage = new T[max * 2];

      for (Size i = 0; i < lgth; ++i)
        storage[i] = static_cast<T &&>(tmp[i]);

      delete[] tmp;
      max = max * 2;
    }

  public:
    void clear()
    {
      delete[] storage;
      storage = nullptr;
      max = 0;
      lgth = 0;
    }

    void remove(Size i)
    {
      for (lib::Size j = i + 1; j < lgth; ++j)
        storage[j - 1] = static_cast<T &&>(storage[j]);

      lgth = lgth - 1;
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

    void pop_back()
    {
      remove(lgth - 1);
    }

    void pop_front()
    {
      remove(0);
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
      return storage + lgth;
    }

    const T *begin() const
    {
      return storage;
    }

    const T *end() const
    {
      return storage + lgth;
    }
  };
}

#endif