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
    using value = ExtractT<T>;
    using pointer = value *;

    mutable pointer ptr = nullptr;

  public:
    constexpr Strong() = default;

    constexpr Strong(pointer p) noexcept
        : ptr(p)
    {
    }

    constexpr Strong(const Strong &) = delete;

    constexpr Strong(Strong &&o) noexcept
        : ptr(o.ptr)
    {
      o.ptr = nullptr;
    }

    constexpr ~Strong() noexcept
    {
      StrongDeleter<T>()(&ptr);
    }

    constexpr Strong &operator=(const Strong &) = delete;

    constexpr Strong &operator=(Strong &&o) noexcept
    {
      if (this != &o)
      {
        StrongDeleter<T>()(&ptr);
        ptr = o.ptr;
        o.ptr = nullptr;
      }

      return *this;
    }

  public:
    constexpr value &operator*() noexcept
    {
      return *ptr;
    }

    constexpr const value &operator*() const noexcept
    {
      return *ptr;
    }

    constexpr pointer operator->() noexcept
    {
      return ptr;
    }

    constexpr const pointer operator->() const noexcept
    {
      return ptr;
    }

    constexpr value &operator[](Size i) noexcept
        requires NativeArray<T>
    {
      return ptr[i];
    }

    constexpr const value &operator[](Size i) const noexcept
        requires NativeArray<T>
    {
      return ptr[i];
    }

    constexpr operator pointer() noexcept
    {
      return ptr;
    }

    constexpr operator const pointer() const noexcept
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