#ifndef __lib_meta_hpp__
#define __lib_meta_hpp__

#include <lib/basic_types.hpp>

namespace meta
{
  template <typename T>
  struct NotConst
  {
    static constexpr bool value = true;
  };

  template <typename T>
  struct NotConst<const T>
  {
    static constexpr bool value = false;
  };
}

template <typename T>
concept NotConst = meta::NotConst<T>::value;

template <typename T, typename U>
struct same_type
{
  static constexpr bool value = false;
};

template <typename T>
struct same_type<T, T>
{
  static constexpr bool value = true;
};

template <typename T, typename O>
concept same_as = same_type<T, O>::value && same_type<O, T>::value;

template <typename T, typename... O>
concept is_any_of = (same_as<T, O> || ...);

template <typename T>
concept is_integer = is_any_of<
    T,
    short, unsigned short, const short, const unsigned short,
    int, unsigned int, const int, const unsigned int,
    long, unsigned long, const long, const unsigned long,
    long long, unsigned long long, const long long, const unsigned long long>;

template <typename T>
concept is_unsigned_integer = is_any_of<
    T,
    unsigned short, const unsigned short,
    unsigned int, const unsigned int,
    unsigned long, const unsigned long,
    unsigned long long, const unsigned long long>;

template <typename T>
concept is_signed_integer = is_integer<T> && !is_unsigned_integer<T>;

template <typename T>
concept is_boolean = same_as<T, bool>;

template <typename T>
concept is_character = is_any_of<T, char, wchar_t>;

namespace meta
{
  template <typename T>
  struct is_native_array
  {
    static constexpr const bool value = false;
  };

  template <typename T>
  struct is_native_array<T[]>
  {
    static constexpr const bool value = true;
  };
}

template <typename T>
constexpr bool is_native_array = meta::is_native_array<T>::value;

namespace meta
{

  template <typename T>
  struct RemoveReference
  {
    using type = T;
  };

  template <typename T>
  struct RemoveReference<T &>
  {
    using type = T;
  };

  template <typename T>
  struct RemoveReference<T &&>
  {
    using type = T;
  };

  template <typename T>
  struct RemoveConstVolatile
  {
    using type = T;
  };

  template <typename T>
  struct RemoveConstVolatile<const T>
  {
    using type = T;
  };

  template <typename T>
  struct RemoveConstVolatile<volatile T>
  {
    using type = T;
  };

  template <typename T>
  struct RemoveConstVolatile<const volatile T>
  {
    using type = T;
  };
}

template <typename T>
using RemoveConstVolatile = typename meta::RemoveConstVolatile<T>::type;

template <typename T>
using RemoveReference = typename meta::RemoveReference<T>::type;

template <typename T>
using RemoveConstVolatilReference = RemoveConstVolatile<RemoveReference<T>>;

template <typename C>
concept Rangeable = requires(const C &c1, C &c2)
{
  c1.begin();
  c1.end();
  c1.end() - c1.begin();
  {
    c1.end() == c1.begin()
    } -> same_as<bool>;
  {
    c1.end() != c1.begin()
    } -> same_as<bool>;

  c2.begin();
  c2.end();
  c2.end() - c2.begin();
  {
    c2.end() == c2.begin()
    } -> same_as<bool>;
  {
    c2.end() != c2.begin()
    } -> same_as<bool>;
};

#endif
