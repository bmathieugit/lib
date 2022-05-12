#ifndef __lib_fmt_format_hpp__
#define __lib_fmt_format_hpp__

#include <cstdio>

#include <lib/string.hpp>
#include <lib/range.hpp>
#include <lib/array.hpp>
#include <lib/meta.hpp>

namespace lib
{
  template <typename T>
  struct Formatter;

  struct FormatSize
  {
    Size size = 0;
  };

  template <typename S>
  struct FormatStream;

  template <>
  struct FormatStream<String>
  {
    String buff;

    FormatStream(Size _max) noexcept : buff(_max) {}

    constexpr void append(char c) noexcept
    {
      buff.lpush_back(c);
    }

    constexpr void append(StringView sv) noexcept
    {
      buff.lappend(sv);
    }
  };

  template <>
  struct FormatStream<std::FILE *>
  {
    std::FILE *buff;

    void append(char c)
    {
      std::fputc(c, buff);
    }

    void append(StringView sv)
    {
      std::fwrite(sv.begin(), sizeof(char), sv.size(), buff);
    }
  };

  using StringStream = FormatStream<String>;
  using FileStream = FormatStream<std::FILE *>;

  template <typename buffer>
  constexpr FormatStream<buffer> &operator<<(
      FormatStream<buffer> &buff, char c) noexcept
  {
    buff.append(c);
    return buff;
  }

  constexpr FormatSize operator+(FormatSize size, char) noexcept
  {
    return {size.size + 1};
  }

  template <typename buffer>
  constexpr FormatStream<buffer> &operator<<(
      FormatStream<buffer> &buff, StringView s) noexcept
  {
    buff.append(s);
    return buff;
  }

  constexpr FormatSize operator+(FormatSize size, StringView sv) noexcept
  {
    return {size.size + sv.size()};
  }

  template <typename buffer, Size n>
  constexpr FormatStream<buffer> &operator<<(
      FormatStream<buffer> &buff, const char (&s)[n]) noexcept
  {
    return buff << StringView(s, n);
  }

  template <Size n>
  constexpr FormatSize operator+(FormatSize size, const char (&s)[n]) noexcept
  {
    return {size.size + n};
  }

  template <typename buffer, typename arg_t>
  StringView format_one_to(
      FormatStream<buffer> &buff,
      StringView fmt, const arg_t &arg) noexcept
  {
    auto [before, after] = rangeof(fmt).around('#');
    buff << before.as<StringView>() << arg;
    return after.as<StringView>();
  }

  template <typename... args_t>
  String format(StringView fmt, const args_t &...args) noexcept
  {
    FormatSize size = ((FormatSize() + ... + args) + fmt);
    FormatStream<String> buff(size.size);
    ((fmt = format_one_to(buff, fmt, args)), ...);
    buff << fmt;
    return move(buff.buff);
  }

  template <typename... args_t>
  void format_to(std::FILE *out, StringView fmt, const args_t &...args) noexcept
  {
    FormatStream<std::FILE *> buff{out};
    ((fmt = format_one_to(buff, fmt, args)), ...);
    buff << fmt;
  }

  struct LiteralFormat
  {
    StringView fmt;

    constexpr String operator()(const auto &...args) const noexcept
    {
      return format(fmt, args...);
    }
  };

  struct LiteralFormatTo
  {
    StringView fmt;

    constexpr void operator()(std::FILE *out, const auto &...args) const noexcept
    {
      format_to(out, fmt, args...);
    }
  };
}

constexpr lib::LiteralFormat operator""_fmt(const char *f, lib::Size n) noexcept
{
  return {lib::StringView(f, n)};
}

constexpr lib::LiteralFormatTo operator""_fmtto(const char *f, lib::Size n) noexcept
{
  return {lib::StringView(f, n)};
}

namespace lib
{
  template <typename buffer>
  constexpr FormatStream<buffer> &operator<<(
      FormatStream<buffer> &buff, bool b) noexcept
  {
    return buff << (b ? 'Y' : 'N');
  }

  constexpr FormatSize operator+(FormatSize size, bool) noexcept
  {
    return {size.size + 1};
  }

  template <typename buffer, IsUnsignedInteger T>
  constexpr FormatStream<buffer> &operator<<(
      FormatStream<buffer> &buff, T t) noexcept
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

    stack_array tbuff;

    if (t == 0)
      buff << '0';
    else
      while (t != 0)
      {
        tbuff.push("0123456789"[t % 10]);
        t /= 10;
      }

    while (!tbuff.empty())
      buff << tbuff.pop();

    return buff;
  }

  template <typename buffer, IsSignedInteger T>
  constexpr FormatStream<buffer> &operator<<(
      FormatStream<buffer> &buff, T t) noexcept
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

    bool neg = t < 0;

    T tmp = neg ? -t : t;

    stack_array tbuff;

    if (tmp == 0)
      buff << '0';
    else
      while (tmp != 0)
      {
        tbuff.push("0123456789"[tmp % 10]);
        tmp /= 10;
      }

    if (neg)
      buff << '-';

    while (!tbuff.empty())
      buff << tbuff.pop();

    return buff;
  }

  template <IsInteger T>
  constexpr FormatSize operator+(FormatSize size, T) noexcept
  {
    return {size.size + sizeof(T) * 4};
  }

  template <typename T>
  struct HexFormat
  {
    const T &t;
  };

  template <typename Buffer, typename T>
  constexpr FormatStream<Buffer> &operator<<(
      FormatStream<Buffer> &buff, HexFormat<T> h) noexcept
  {
    constexpr StringView hextable = "0123456789ABCDEF";

    const char *b = reinterpret_cast<const char *>(&h.t) - 1;
    const char *e = reinterpret_cast<const char *>(&h.t) + sizeof(T) - 1;

    while (e != b)
    {
      buff << hextable[(*e & 0b11110000) >> 4]
           << hextable[(*e & 0b00001111)];
      --e;
    }

    return buff;
  }

  template <typename T>
  constexpr FormatSize operator+(FormatSize size, HexFormat<T> h) noexcept
  {
    return {size.size + 2 * sizeof(T)};
  }

  template <typename T>
  constexpr HexFormat<T> hex(const T &t)
  {
    return HexFormat<T>{t};
  }

  template <typename T>
  struct BinFormat
  {
    const T &t;
  };

  template <typename Buffer, typename T>
  constexpr FormatStream<Buffer> &operator<<(
      FormatStream<Buffer> &buff, BinFormat<T> h) noexcept
  {
    constexpr StringView bintable = "01";

    const char *b = reinterpret_cast<const char *>(&h.t) - 1;
    const char *e = reinterpret_cast<const char *>(&h.t) + sizeof(T) - 1;

    while (e != b)
    {
      buff << bintable[(*e & 0b10000000) >> 7]
           << bintable[(*e & 0b01000000) >> 6]
           << bintable[(*e & 0b00100000) >> 5]
           << bintable[(*e & 0b00010000) >> 4]
           << bintable[(*e & 0b00001000) >> 3]
           << bintable[(*e & 0b00000100) >> 2]
           << bintable[(*e & 0b00000010) >> 1]
           << bintable[(*e & 0b00000001)];
      --e;
    }

    return buff;
  }

  template <typename T>
  constexpr FormatSize operator+(FormatSize size, BinFormat<T> h) noexcept
  {
    return {size.size + 8 * sizeof(T)};
  }

  template <typename T>
  constexpr BinFormat<T> bin(const T &t)
  {
    return BinFormat<T>{t};
  }
}

#endif
