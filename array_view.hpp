#ifndef __lib_array_view_hpp__
#define __lib_array_view_hpp__

#include <lib/basic_types.hpp>
#include <lib/algorithm.hpp>
#include <lib/meta.hpp>

namespace lib
{
  template <typename T, typename LENGTH>
  concept LengthNativeArray = requires(LENGTH l, const T *o)
  {
    {l(o)} -> same_as<Size>;
  };

  template <typename T, typename LENGTH = void>
  class ArrayView
  {

    const T *b = nullptr;
    Size lgth = 0;

  public:
    constexpr ArrayView() = default;

    constexpr ArrayView(const T *_b, Size l)
        : b(_b),
          lgth(l)
    {
    }

    constexpr ArrayView(const T *o) requires LengthNativeArray<T, LENGTH>
        : ArrayView(o, LENGTH{}(o))
    {
    }

    template <Size n>
    constexpr ArrayView(const T (&o)[n])
        : ArrayView(o, n)
    {
    }

    constexpr ArrayView(const T *_b, const T *_e)
        : ArrayView(_b, _e - _b < 0 ? 0 : _e - _b)
    {
    }

    constexpr ArrayView(const ArrayView &) = default;
    constexpr ArrayView(ArrayView &&) = default;

    constexpr ArrayView &operator=(const ArrayView &) = default;
    constexpr ArrayView &operator=(ArrayView &&) = default;

  public:
    constexpr decltype(auto) apply(auto &&algorithm, auto &&...args)
    {
      return algorithm(begin(), end(), args...);
    }

    constexpr decltype(auto) apply(auto &&algorithm, auto &&...args) const
    {
      return algorithm(begin(), end(), args...);
    }

  public:
    constexpr bool operator==(ArrayView o) const
    {
      return apply(EqualsAlgorithm(), o.begin(), o.end());
    }

    template <Size n>
    constexpr bool operator==(const T (&o)[n]) const
    {
      return *this == ArrayView(o, n);
    }

    constexpr bool operator==(const T *o) const requires LengthNativeArray<T, LENGTH>
    {
      return *this == ArrayView(o);
    }

    constexpr bool operator!=(ArrayView o) const
    {
      return !(*this == o);
    }

    template <Size n>
    constexpr bool operator!=(const T (&o)[n]) const
    {
      return *this != ArrayView(o, n);
    }

    constexpr bool operator!=(const T *o) const requires LengthNativeArray<T, LENGTH>
    {
      return *this != ArrayView(o);
    }

    constexpr bool starts_with(ArrayView o) const
    {
      return apply(StartsWithAlgorithm(), o.begin(), o.end());
    }

    template <Size n>
    constexpr bool starts_with(const T (&o)[n]) const
    {
      return starts_with(ArrayView(o, n));
    }

    constexpr bool starts_with(const T *o) const requires LengthNativeArray<T, LENGTH>
    {
      return starts_with(ArrayView(o));
    }

  public : constexpr Size size() const
    {
      return lgth;
    }

    constexpr bool empty() const
    {
      return size() == 0;
    }

    constexpr const T &operator[](Size i) const
    {
      return b[i];
    }

  public:
    constexpr ArrayView after(const T &t) const
    {
      return ArrayView(apply(AfterAlgorithm(), t), end());
    }

    constexpr ArrayView before(const T &t) const
    {
      return ArrayView(begin(), apply(BeforeAlgorithm(), t));
    }

    constexpr decltype(auto) around(const T &t) const
    {
      return apply(AroundAlgorithm<ArrayView<T, LENGTH>>(), t);
    }

  public:
    constexpr const T *begin() const
    {
      return b;
    }

    constexpr const T *end() const
    {
      return b + lgth;
    }

    constexpr const T *cbegin() const
    {
      return b;
    }

    constexpr const T *cend() const
    {
      return b + lgth;
    }

    constexpr const T *data() const
    {
      return b;
    }
  };
}

#endif