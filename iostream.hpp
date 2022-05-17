#ifndef __lib_fmt_iostream_hpp__
#define __lib_fmt_iostream_hpp__

#include <cstdio>

#include <lib/string.hpp>
#include <lib/range.hpp>
#include <lib/array.hpp>
#include <lib/meta.hpp>

namespace lib
{
  template <typename OUT, typename... Args>
  concept Output = requires(OUT &out, Args &&...args)
  {
    out.append(StringView{});
    out.append(char{});
  };

  struct OutputSize
  {
    Size size = 0;
  };

  template <Output OUT>
  class OutputWriter
  {
  public:
    template <typename... Args>
    constexpr auto write(Args &&...args) noexcept
    {
      auto out = OUT(forward<Args>(args)...);
      if constexpr (not same_as<decltype(out.result()), void>)
        return out.result();
    }
  };

  struct StringOutput
  {
    String res;

    template <typename... Args>
    StringOutput(Args &&...args) noexcept
        : res(String((OutputSize() + ... + args).size))
    {
      (*this << ... << forward<Args>(args));
    }

    constexpr void append(StringView sv) noexcept
    {
      res.lappend(sv);
    }

    constexpr void append(char c) noexcept
    {
      res.lpush_back(c);
    }

    constexpr String &&result() noexcept
    {
      return move(res);
    }
  };

  using StringWriter = OutputWriter<StringOutput>;

  struct FileOutput
  {
    std::FILE *out;

    template <typename... Args>
    FileOutput(std::FILE *f, Args &&...args) noexcept
        : out(f)
    {
      (*this << ... << forward<Args>(args));
    }

    void append(char c) noexcept
    {
      std::fputc(c, out);
    }

    void append(StringView sv) noexcept
    {
      std::fwrite(sv.begin(), sizeof(char), sv.size(), out);
    }

    void result() noexcept {}
  };

  using FileWriter = OutputWriter<FileOutput>;

  template <typename... Args>
  void print(Args &&...args) noexcept
  {
    FileWriter().write(stdout, forward<Args>(args)...);
  }

  template <typename... Args>
  void println(Args &&...args) noexcept
  {
    FileWriter().write(stdout, forward<Args>(args)..., '\n');
  }
}

namespace lib
{
  template <Output OUT>
  constexpr OUT &operator<<(OUT &buff, char c) noexcept
  {
    buff.append(c);
    return buff;
  }

  constexpr OutputSize operator+(OutputSize size, char) noexcept
  {
    return {size.size + 1};
  }

  template <Output OUT>
  constexpr OUT &operator<<(OUT &buff, StringView s) noexcept
  {
    buff.append(s);
    return buff;
  }

  constexpr OutputSize operator+(OutputSize size, StringView sv) noexcept
  {
    return {size.size + sv.size()};
  }

  template <Output OUT>
  constexpr OUT &operator<<(OUT &buff, const char *s) noexcept
  {
    return buff << StringView(s, StrLen<char>()(s));
  }

  constexpr OutputSize operator+(OutputSize size, const char *s) noexcept
  {
    return {size.size + StrLen<char>()(s)};
  }

  template <Output OUT>
  constexpr OUT &operator<<(OUT &buff, bool b) noexcept
  {
    return buff << (b ? sv("true") : sv("false"));
  }

  constexpr OutputSize operator+(OutputSize size, bool) noexcept
  {
    return {size.size + 5};
  }

  class stack_array
  {
    char data[40];
    int i = -1;

  public:
    constexpr void push(char c) noexcept { data[++i] = c; }
    constexpr char pop() noexcept { return data[i--]; }
    constexpr bool empty() noexcept { return i == -1; }
  };

  template <Output OUT, IsUnsignedInteger T>
  constexpr OUT &operator<<(OUT &buff, T t) noexcept
  {

    stack_array tbuff;

    if (t == 0)
      buff.append('0');
    else
      while (t != 0)
      {
        tbuff.push("0123456789"[t % 10]);
        t /= 10;
      }

    while (!tbuff.empty())
      buff.append(tbuff.pop());

    return buff;
  }

  template <Output OUT, IsSignedInteger T>
  constexpr OUT &operator<<(OUT &buff, T t) noexcept
  {

    bool neg = t < 0;

    T tmp = neg ? -t : t;

    stack_array tbuff;

    if (tmp == 0)
      buff.append('0');
    else
      while (tmp != 0)
      {
        tbuff.push("0123456789"[tmp % 10]);
        tmp /= 10;
      }

    if (neg)
      buff.append('-');

    while (!tbuff.empty())
      buff.append(tbuff.pop());

    return buff;
  }

  template <IsInteger T>
  constexpr OutputSize operator+(OutputSize size, T) noexcept
  {
    return {size.size + sizeof(T) * 4};
  }

  template <typename T>
  struct HexFormat
  {
    const T &t;
  };

  template <Output OUT, typename T>
  constexpr OUT &operator<<(OUT &buff, HexFormat<T> h) noexcept
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
  constexpr OutputSize operator+(OutputSize size, HexFormat<T> h) noexcept
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

  template <Output OUT, typename T>
  constexpr OUT &operator<<(OUT &buff, BinFormat<T> h) noexcept
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
  constexpr OutputSize operator+(OutputSize size, BinFormat<T> h) noexcept
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
