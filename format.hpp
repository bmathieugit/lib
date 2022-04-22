#ifndef __lib_fmt_format_hpp__
#define __lib_fmt_format_hpp__

#include <cstdio>

#include <lib/string_view.hpp>
#include <lib/string.hpp>
#include <lib/meta.hpp>
#include <lib/enumerate.hpp>

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
      same_as<B, Stream<Buffer<char>>> ||
      same_as<B, Stream<std::FILE *>>;

  template <>
  struct Formatter<char>
  {
    void format(is_buffer auto &buff, char c) const
    {
      buff.append(c);
    }

    Size size(char c) const
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

    Size size(StringView s) const
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

    Size size(const char (&s)[n])
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

    Size size(const char *s) const
    {
      return CStringUtils::length(s);
    }
  };

  template <typename... args_t>
  Size size(const args_t &...args)
  {
    return (Formatter<args_t>().size(args) + ... + 0);
  }

  template <typename arg_t>
  StringView format_one_to(is_buffer auto &buff, StringView fmt, const arg_t &arg)
  {
    auto [before, after] = fmt.around('#');
    buff.append(before);
    Formatter<arg_t>().format(buff, arg);
    return after;
  }

  template <typename... args_t>
  String format(StringView fmt, const args_t &...args)
  {
    Stream<Buffer<char>> buff(size(fmt, args...));
    ((fmt = format_one_to(buff, fmt, args)), ...);
    buff.append(fmt);
    return String(buff.buff.flush());
  }

  template <typename... args_t>
  void format_to(std::FILE *out, StringView fmt, const args_t &...args)
  {
    Stream<std::FILE *> buff{out};
    ((fmt = format_one_to(buff, fmt, args)), ...);
    buff.append(fmt);
  }

  template <>
  struct Formatter<bool>
  {
    void format(
        is_buffer auto &buff,
        const bool &b) const
    {
      Formatter<StringView>().format(buff, (b ? "true" : "false"));
    }

    Size size(bool b) const
    {
      return b ? 4 : 5;
    }
  };

  template <is_signed_integer T>
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

    Size size(T t) const
    {
      return sizeof(t) * 4;
    }
  };

  template <is_unsigned_integer T>
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

      stack_array tbuff;

      if (t == 0)
        Formatter<char>().format(buff, '0');
      else
        while (t != 0)
        {
          tbuff.push("0123456789"[t % 10]);
          t /= 10;
        }

      while (!tbuff.empty())
        Formatter<char>().format(buff, tbuff.pop());
    }

    Size size(T t) const
    {
      return sizeof(t) * 4;
    }
  };

  template <typename C>
  requires Rangeable<C>
  struct Formatter<C> 
  {
    void format(is_buffer auto &buff, const Rangeable auto &v) const
    {
      Formatter<char>().format(buff, '{');

      for (const auto &[c, i] : enumerate(v))
      {
        Formatter<remove_cvref<decltype(c)>>().format(buff, c);

        if (i < v.size() - 1)
        {
          Formatter<char>().format(buff, ',');
          Formatter<char>().format(buff, ' ');
        }
      }

      Formatter<char>().format(buff, '}');
    }

    Size size(const Rangeable auto &v) const
    {
      Size vsize = 0;

      for (const auto &t : v)
        vsize += Formatter<remove_cvref<decltype(t)>>().size(t);

      return Formatter<char>().size('{') +
             Formatter<char>().size(',') * v.size() +
             Formatter<char>().size(' ') * v.size() +
             vsize;
    }
  };

  

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
