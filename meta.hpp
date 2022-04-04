#ifndef __lib_meta_hpp__
#define __lib_meta_hpp__

#include <concepts>

template <typename T, typename... O>
concept is_any_of = (std::same_as<T, O> || ...);

template <typename T>
concept is_integer = is_any_of<
    T,
    short, unsigned short, const short, const unsigned short,
    int, unsigned int, const int, const unsigned int,
    long, unsigned long, const long, const unsigned long,
    long long, unsigned long long, const long long, const unsigned long long>;

template <typename T>
concept is_boolean = std::same_as<T, bool>;

template <typename T>
concept is_character = is_any_of<T, char, wchar_t>;

#endif
