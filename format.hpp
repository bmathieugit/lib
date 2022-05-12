#ifndef __lib_fmt_format_hpp__
#define __lib_fmt_format_hpp__

#include <cstdio>

#include <lib/string.hpp>
#include <lib/meta.hpp>
#include <lib/enumerate.hpp>

namespace lib::fmt
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

  template <typename B>
  concept is_buffer =
      same_as<B, FormatStream<String>> ||
      same_as<B, FormatStream<std::FILE *>>;

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
    buff.append(s);
    return buff;
  }

  template <Size n>
  constexpr FormatSize operator+(FormatSize size, const char (&s)[n]) noexcept
  {
    return {size.size + n};
  }

  template <is_buffer buffer, typename arg_t>
  StringView format_one_to(buffer &buff, StringView fmt, const arg_t &arg) noexcept
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
    buff.append(fmt);
    return move(buff.buff);
  }

  template <typename... args_t>
  void format_to(std::FILE *out, StringView fmt, const args_t &...args) noexcept
  {
    FormatStream<std::FILE *> buff{out};
    ((fmt = format_one_to(buff, fmt, args)), ...);
    buff.append(fmt);
  }

  template <typename buffer>
  constexpr FormatStream<buffer> &operator<<(
      FormatStream<buffer> &buff, bool b) noexcept
  {
    buff.append(b ? "true"_sv : "false"_sv);
    return buff;
  }

  constexpr FormatSize operator+(FormatSize size, bool) noexcept
  {
    return {size.size + 5};
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

constexpr lib::fmt::LiteralFormat operator""_fmt(const char *f, lib::Size n) noexcept
{
  return {lib::StringView(f, n)};
}

constexpr lib::fmt::LiteralFormatTo operator""_fmtto(const char *f, lib::Size n) noexcept
{
  return {lib::StringView(f, n)};
}

#endif
