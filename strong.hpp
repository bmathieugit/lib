#ifndef __lib_strong_hpp__
#define __lib_strong_hpp__

#include <lib/meta.hpp>
#include <lib/basic_types.hpp>

namespace lib
{
  template <typename T>
  class Strong
  {
    mutable T *ptr = nullptr;

  public:
    Strong() = default;
    Strong(T *p)
        : ptr{p} {}
    Strong(const Strong &) = delete;

    Strong(Strong &&o)
        : ptr{o.ptr}
    {
      o.ptr = nullptr;
    }

    ~Strong()
    {
      destruct();
    }

    Strong &operator=(const Strong &) = delete;
    Strong &operator=(Strong &&o)
    {
      if (this != &o)
      {
        destruct();
        ptr = o.ptr;
        o.ptr = nullptr;
      }

      return *this;
    }

    operator T *()
    {
      return ptr;
    }

    operator const T *() const
    {
      return ptr;
    }

    T &operator*()
    {
      return *ptr;
    }

    const T &operator*() const
    {
      return *ptr;
    }

    operator bool() const
    {
      return ptr != nullptr;
    }

    T *operator->()
    {
      return ptr;
    }

    const T *operator->() const
    {
      return ptr;
    }

    void destruct() const
    {
      delete ptr;
      ptr = nullptr;
    }
  };

  template <typename T>
  class Strong<T[]>
  {
    mutable T *ptr = nullptr;

  public:
    Strong() = default;
    Strong(T *p)
        : ptr{p} {}
    Strong(const Strong &) = delete;

    Strong(Strong &&o)
        : ptr{o.ptr}
    {
      o.ptr = nullptr;
    }

    ~Strong()
    {
      destruct();
    }

    Strong &operator=(const Strong &) = delete;
    Strong &operator=(Strong &&o)
    {
      if (this != &o)
      {
        destruct();
        ptr = o.ptr;
        o.ptr = nullptr;
      }

      return *this;
    }

    Strong &operator=(T *p)
    {
      if (ptr != p)
      {
        destruct();
        ptr = p;
      }

      return *this;
    }

    operator T *()
    {
      return ptr;
    }

    operator const T *() const
    {
      return ptr;
    }

    T &operator*()
    {
      return *ptr;
    }

    const T &operator*() const
    {
      return *ptr;
    }

    operator bool() const
    {
      return ptr != nullptr;
    }

    T *operator->()
    {
      return ptr;
    }

    const T *operator->() const
    {
      return ptr;
    }

    T &operator[](Size i)
    {
      return ptr[i];
    }

    const T &operator[](Size i) const
    {
      return ptr[i];
    }

    void destruct() const
    {
      delete[] ptr;
      ptr = nullptr;
    }
  };
}

#endif