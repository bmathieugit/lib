#ifndef __lib_fmt_format_hpp__
#define __lib_fmt_format_hpp__

#include <lib/string_view.hpp>
#include <lib/string.hpp>
#include <lib/vector.hpp>
#include <array>
#include <map>
#include <cstdio>
#include <lib/meta.hpp>
#include <lib/enumerate.hpp>

namespace lib::fmt
{
  template <typename T>
  struct formatter;

  template <typename S>
  struct buffer;

  template <>
  struct buffer<lib::String>
  {
    lib::String buff;

    void push_back(char c)
    {
      buff.push_back(c);
    }

    void append(lib::StringView sv)
    {
      buff.append(sv.begin(), sv.end());
    }
  };

  template <>
  struct buffer<std::FILE *>
  {
    std::FILE *buff;

    void push_back(char c)
    {
      std::fputc(c, buff);
    }

    void append(lib::StringView sv)
    {
      std::fwrite(sv.begin(), sizeof(char),
                  sv.size(), buff);
    }
  };

  template <typename B>
  concept is_buffer =
      std::same_as<B, buffer<lib::String>> or
      std::same_as<B, buffer<std::FILE *>>;
}

template <>
struct lib::fmt::formatter<char>
{
  void format(
      is_buffer auto &buff, char c) const
  {
    buff.push_back(c);
  }
};

template <>
struct lib::fmt::formatter<lib::StringView>
{
  void format(
      is_buffer auto &buff,
      lib::StringView s) const
  {
    buff.append(s);
  }
};

template <>
struct lib::fmt::formatter<lib::String>
{
  void format(
      is_buffer auto &buff,
      const lib::String &s) const
  {
    lib::fmt::formatter<lib::StringView>{}.format(buff, lib::StringView(s.data(), s.size()));
  }
};

template <lib::Size n>
struct lib::fmt::formatter<char[n]>
{
  void format(
      is_buffer auto &buff,
      const char (&s)[n]) const
  {
    lib::fmt::formatter<lib::StringView>{}.format(buff, s);
  }
};

template <>
struct lib::fmt::formatter<const char *>
{
  void format(
      is_buffer auto &buff,
      const char *s) const
  {
    lib::fmt::formatter<lib::StringView>{}.format(buff, s);
  }
};

namespace lib::fmt
{

  lib::StringView bfmt(
      is_buffer auto &buff,
      lib::StringView fmt);

  template <typename arg_t>
  lib::StringView format(
      is_buffer auto &buff,
      lib::StringView fm,
      const arg_t &arg);

  template <typename... args_t>
  lib::String format(
      lib::StringView fm,
      const args_t &...args);

  template <typename... args_t>
  void format_to(
      std::FILE *out,
      lib::StringView fm,
      const args_t &...args);

  struct literal_format
  {
    lib::StringView fmt;

    lib::String operator()(const auto &...args)
    {
      return lib::fmt::format(fmt, args...);
    }
  };

  struct literal_format_to
  {
    lib::StringView fmt;

    void operator()(std::FILE *out, const auto &...args)
    {
      lib::fmt::format_to(out, fmt, args...);
    }
  };
}

lib::fmt::literal_format
operator"" _fmt(const char *f, lib::Size n);

lib::fmt::literal_format_to
operator"" _fmtto(const char *f, lib::Size n);

lib::StringView lib::fmt::bfmt(
    is_buffer auto &buff,
    lib::StringView fmt)
{
  lib::Size htag = fmt.findsub('#');
  lib::StringView part = fmt.substr(0, htag);

  buff.append(part);

  return fmt.substr(htag != fmt.size() ? htag + 1 : htag);
};

template <typename arg_t>
lib::StringView lib::fmt::format(
    is_buffer auto &buff,
    lib::StringView fm,
    const arg_t &arg)
{
  fm = lib::fmt::bfmt(buff, fm);
  lib::fmt::formatter<arg_t>{}.format(buff, arg);
  return fm;
}

template <typename... args_t>
void lib::fmt::format_to(
    std::FILE *out,
    lib::StringView fm,
    const args_t &...args)
{
  lib::fmt::buffer<std::FILE *> buff{out};
  ((fm = lib::fmt::format(buff, fm, args)), ...);
  buff.append(fm);
}

template <typename... args_t>
lib::String lib::fmt::format(
    lib::StringView fm,
    const args_t &...args)
{
  lib::fmt::buffer<lib::String> buff;
  ((fm = lib::fmt::format(buff, fm, args)), ...);
  buff.append(fm);
  return buff.buff;
};

template <is_integer T>
struct lib::fmt::formatter<T>
{
  class stack_array
  {
    char data[40];
    int i = -1;

  public:
    void push(char c) { data[++i] = c; }
    char pop() { return data[i--]; }
    bool empty() { return i == -1; }
  };

  void format(is_buffer auto &buff, T t) const
  {
    bool neg = t < 0;
    auto _abs = [](int i)
    { return i < 0 ? -i : i; };

    stack_array tbuff;

    if (t == 0)
      lib::fmt::formatter<char>{}.format(buff, '0');
    else
      while (t != 0)
      {
        tbuff.push("0123456789"[_abs(t % 10)]);
        t /= 10;
      }

    if (neg)
      tbuff.push('-');

    while (not tbuff.empty())
      lib::fmt::formatter<char>{}.format(buff, tbuff.pop());
  }
};

template <>
struct lib::fmt::formatter<bool>
{
  void format(
      is_buffer auto &buff,
      const bool &b) const
  {
    lib::fmt::formatter<lib::StringView>{}
        .format(buff, (b ? "true" : "false"));
  }
};

template <typename T>
struct lib::fmt::formatter<lib::Vector<T>>
{
  void format(
      is_buffer auto &buff,
      const lib::Vector<T> &v) const
  {
    lib::fmt::formatter<char>{}.format(buff, '{');

    for (lib::Size i = 0; i < v.size(); ++i)
    {
      lib::fmt::formatter<T>{}.format(buff, v[i]);

      if (i < v.size() - 1)
        lib::fmt::formatter<lib::StringView>{}.format(buff, ", ");
    }

    lib::fmt::formatter<char>{}.format(buff, '}');
  }
};

template <typename T, lib::Size n>
struct lib::fmt::formatter<std::array<T, n>>
{
  void format(
      is_buffer auto &buff,
      const std::array<T, n> &v) const
  {
    lib::fmt::formatter<char>{}.format(buff, '{');

    for (lib::Size i = 0; i < v.size(); ++i)
    {
      lib::fmt::formatter<T>{}.format(buff, v[i]);

      if (i < v.size() - 1)
        lib::fmt::formatter<lib::StringView>{}.format(buff, ", ");
    }

    lib::fmt::formatter<char>{}.format(buff, '}');
  }
};

template <typename K, typename V>
struct lib::fmt::formatter<std::map<K, V>>
{
  void format(
      is_buffer auto &buff,
      const std::map<K, V> &m) const
  {
    lib::fmt::formatter<char>{}.format(buff, '{');

    for (const auto &[p, i] : lib::enumerate(m))
    {
      lib::fmt::formatter<char>{}.format(buff, '{');
      lib::fmt::formatter<K>{}.format(buff, p.first);
      lib::fmt::formatter<char>{}.format(buff, ',');
      lib::fmt::formatter<V>{}.format(buff, p.second);
      lib::fmt::formatter<char>{}.format(buff, '}');

      if (i < m.size() - 1)
        lib::fmt::formatter<char>{}.format(buff, ',');
    }

    lib::fmt::formatter<char>{}.format(buff, '}');
  }
};

#endif
