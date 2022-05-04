#ifndef __lib_strong_hpp__
#define __lib_strong_hpp__

#include <lib/meta.hpp>
#include <lib/basic_types.hpp>

namespace lib
{
  template <typename T>
  struct StrongDeleter
  {
    constexpr void operator()(T **p) const noexcept
    {
      delete (*p);
      p = nullptr;
    }
  };

  template <typename T>
  struct StrongDeleter<T[]>
  {
    constexpr void operator()(T **p) const noexcept
    {
      delete[](*p);
      p = nullptr;
    }
  };

  namespace meta
  {
    template <typename T>
    struct ExtractT
    {
      using type = T;
    };

    template <typename T>
    struct ExtractT<T[]>
    {
      using type = T;
    };
  }

  template <typename T>
  using ExtractT = typename meta::ExtractT<T>::type;

  template <typename T>
  class Strong
  {
  public:
    mutable ExtractT<T> *ptr = nullptr;

  public:
    constexpr Strong() = default;

    constexpr Strong(ExtractT<T> *p) noexcept
    {
      ptr = p;
    }

    constexpr Strong(const Strong &) = delete;

    constexpr Strong(Strong &&o) noexcept
    {
      ptr = o.ptr;
      o.ptr = nullptr;
    }

    constexpr ~Strong() noexcept
    {
      StrongDeleter<T>{}(&ptr);
    }

    constexpr Strong &operator=(const Strong &) = delete;

    constexpr Strong &operator=(Strong &&o) noexcept
    {
      if (this != &o)
      {
        StrongDeleter<T>{}(&ptr);
        ptr = o.ptr;
        o.ptr = nullptr;
      }

      return *this;
    }

  public:
    constexpr ExtractT<T> &operator*() noexcept
    {
      return *ptr;
    }

    constexpr const ExtractT<T> &operator*() const noexcept
    {
      return *ptr;
    }

    constexpr ExtractT<T> *operator->() noexcept
    {
      return ptr;
    }

    constexpr const ExtractT<T> *operator->() const noexcept
    {
      return ptr;
    }

  public:
    constexpr ExtractT<T> &operator[](Size i) noexcept
        requires NativeArray<T>
    {
      return ptr[i];
    }

    constexpr const ExtractT<T> &operator[](Size i) const noexcept
        requires NativeArray<T>
    {
      return ptr[i];
    }

  public : constexpr operator ExtractT<T> *() noexcept
    {
      return ptr;
    }

    constexpr operator const ExtractT<T> *() const noexcept
    {
      return ptr;
    }

    constexpr operator bool() const noexcept
    {
      return ptr != nullptr;
    }
  };
}

#endif