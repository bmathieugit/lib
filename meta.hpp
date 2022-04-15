#ifndef __lib_meta_hpp__
#define __lib_meta_hpp__

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
concept is_boolean = same_as<T, bool>;

template <typename T>
concept is_character = is_any_of<T, char, wchar_t>;

#endif
