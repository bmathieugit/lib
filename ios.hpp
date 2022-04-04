#ifndef __lib_ios_hpp__
#define __lib_ios_hpp__

#include <string_view>
#include <cstdio>

#include <lib/format.hpp>

namespace lib::ios
{
  template <typename T>
  size_t fwrite(std::FILE *f, T *data, size_t count);


  void fprintf(std::FILE *out,
               std::string_view fmt,
               const auto &...args);

  void fprintfln(std::FILE *out,
                 std::string_view fmt,
                 const auto &...args);

  void printf(std::string_view fmt,
              const auto &...args);

  void printfln(std::string_view fmt,
                const auto &...args);
}

template <typename T>
size_t lib::ios::fwrite(
    std::FILE *f,
    T *data,
    size_t count)
{
  return std::fwrite(data, sizeof(T), count, f);
}

void lib::ios::fprintf(
    std::FILE *out,
    std::string_view fmt,
    const auto &...args)
{
  lib::fmt::format_to(out, fmt, args...);
}

void lib::ios::fprintfln(
    std::FILE *out,
    std::string_view fmt,
    const auto &...args)
{
  lib::fmt::format_to(out, fmt, args...);
  std::fputc('\n', out);
}

void lib::ios::printf(
    std::string_view fmt,
    const auto &...args)
{
  lib::ios::fprintf(stdout, fmt, args...);
}

void lib::ios::printfln(
    std::string_view fmt,
    const auto &...args)
{
  lib::ios::fprintfln(stdout, fmt, args...);
}

#endif