#ifndef __lib_fmt_format_hpp__
#define __lib_fmt_format_hpp__

#include <cstdio>

#include <lib/string_view.hpp>
#include <lib/string.hpp>
#include <lib/vector.hpp>
#include <lib/meta.hpp>
#include <lib/enumerate.hpp>

#include <iostream>

namespace lib::fmt
{
  template <typename T>
  struct Formatter;

  template <typename S>
  struct Stream;

  template <>
  struct Stream<Buffer<char>>
  {
    Buffer<char> buff;

    Stream(Size max)
        : buff(max)
    {
    }

    void append(char c)
    {
      buff.push_back(c);
    }

    void append(StringView sv)
    {
      for (char c : sv)
        append(c);
    }
  };

  template <>
  struct Stream<std::FILE *>
  {
    std::FILE *buff;

    void append(char c)
    {
      std::fputc(c, buff);
    }

    void append(StringView sv)
    {
      std::fwrite(sv.begin(), sizeof(char),
                  sv.size(), buff);
    }
  };

  template <typename B>
  concept is_buffer =
      std::same_as<B, Stream<Buffer<char>>> ||
      std::same_as<B, Stream<std::FILE *>>;

  template <>
  struct Formatter<char>
  {
    void format(is_buffer auto &buff, char c) const
    {
      buff.append(c);
    }

    constexpr Size size(char c) const
    {
      return 1;
    }
  };

  template <>
  struct Formatter<StringView>
  {
    void format(is_buffer auto &buff, StringView s) const
    {
      buff.append(s);
    }

    constexpr Size size(StringView s) const
    {
      return s.size();
    }
  };

  template <>
  struct Formatter<String>
  {
    void format(is_buffer auto &buff, const String &s) const
    {
      Formatter<StringView>().format(buff, s);
    }

    Size size(const String &s) const
    {
      return s.size();
    }
  };

  template <Size n>
  struct Formatter<char[n]>
  {
    void format(is_buffer auto &buff, const char (&s)[n]) const
    {
      Formatter<StringView>().format(buff, s);
    }

    constexpr Size size(const char (&s)[n])
    {
      return n;
    }
  };

  template <>
  struct Formatter<const char *>
  {
    void format(is_buffer auto &buff, const char *s) const
    {
      Formatter<StringView>().format(buff, s);
    }

    constexpr Size size(const char *s) const
    {
      return CStringUtils::length(s);
    }
  };

  StringView bfmt(is_buffer auto &buff, StringView fmt)
  {
    buff.append(fmt.before('#'));
    return fmt.after('#');
  };

  template <typename arg_t>
  StringView format(is_buffer auto &buff, StringView fm, const arg_t &arg)
  {
    fm = bfmt(buff, fm);
    Formatter<arg_t>().format(buff, arg);
    return fm;
  }

  template <typename... args_t>
  constexpr Size size(const args_t &...args)
  {
    return (Formatter<args_t>().size(args) + ... + 0);
  }

  template <typename... args_t>
  String format(StringView fm, const args_t &...args)
  {
    Stream<Buffer<char>> buff(size(fm, args...));
    ((fm = format(buff, fm, args)), ...);
    buff.append(fm);
    return String(buff.buff.flush());
  }

  template <typename... args_t>
  void format_to(std::FILE *out, StringView fm, const args_t &...args)
  {
    Stream<std::FILE *> buff{out};
    ((fm = format(buff, fm, args)), ...);
    buff.append(fm);
  }

  struct literal_format
  {
    StringView fmt;

    String operator()(const auto &...args)
    {
      return format(fmt, args...);
    }
  };

  struct literal_format_to
  {
    StringView fmt;

    void operator()(std::FILE *out, const auto &...args)
    {
      format_to(out, fmt, args...);
    }
  };

  template <>
  struct Formatter<bool>
  {
    void format(
        is_buffer auto &buff,
        const bool &b) const
    {
      Formatter<StringView>().format(buff, (b ? "true" : "false"));
    }

    constexpr Size size(bool b) const
    {
      return b ? 4 : 5;
    }
  };

  template <is_integer T>
  struct Formatter<T>
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
        Formatter<char>().format(buff, '0');
      else
        while (t != 0)
        {
          tbuff.push("0123456789"[_abs(t % 10)]);
          t /= 10;
        }

      if (neg)
        tbuff.push('-');

      while (!tbuff.empty())
        Formatter<char>().format(buff, tbuff.pop());
    }

    constexpr Size size(T t) const
    {
      return sizeof(t) * 4;
    }
  };

  template <typename T>
  struct Formatter<Vector<T>>
  {
    void format(is_buffer auto &buff, const Vector<T> &v) const
    {
      Formatter<char>().format(buff, '{');

      for (const auto &[c, i] : enumerate(v))
      {
        Formatter<T>().format(buff, c);

        if (i < v.size() - 1)
        {
          Formatter<char>().format(buff, ',');
          Formatter<char>().format(buff, ' ');
        }
      }

      Formatter<char>().format(buff, '}');
    }

    constexpr Size size(const Vector<T> &v) const
    {
      Size vsize = 0;

      for (const T &t : v)
        vsize += Formatter<T>().size(t);

      return Formatter<char>().size('{') +
             Formatter<char>().size(',') * v.size() +
             Formatter<char>().size(' ') * v.size() +
             vsize;
    }
  };
}

lib::fmt::literal_format
operator""_fmt(const char *f, size_t n)
{
  return {{f, n}};
}

lib::fmt::literal_format_to
operator""_fmtto(const char *f, size_t n)
{
  return {{f, n}};
}

#endif
