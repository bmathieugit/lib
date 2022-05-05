#ifndef __lib_ios_hpp__
#define __lib_ios_hpp__

#include <lib/string.hpp>
#include <lib/format.hpp>

#include <cstdio>

namespace lib::ios
{
  template <typename T>
  size_t fwrite(std::FILE *f, T *data, size_t count)
  {
    return std::fwrite(data, sizeof(T), count, f);
  }

  void fprintf(std::FILE *out, StringView fmt, const auto &...args)
  {
    fmt::format_to(out, fmt, args...);
  }

  void fprintfln(std::FILE *out, StringView fmt, const auto &...args)
  {
    fmt::format_to(out, fmt, args...);
    std::fputc('\n', out);
  }

  void printf(StringView fmt, const auto &...args)
  {
    ios::fprintf(stdout, fmt, args...);
  }

  void printfln(StringView fmt, const auto &...args)
  {
    ios::fprintfln(stdout, fmt, args...);
  }
}
#endif