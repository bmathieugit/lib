#ifndef __log_logger_hpp__
#define __log_logger_hpp__

#include <cstdio>
#include <string_view>
#include <ctime>

#include <lib/ios.hpp>
#include <lib/format.hpp>

namespace lib::logger
{
  enum class level : int
  {
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    error = 4,
    fatal = 5
  };

  struct pad2d
  {
    int i;
  };

  void log(level l,
           std::string_view msg,
           const auto &...pms);

  void trace(std::string_view msg,
             const auto &...pms);

  void debug(std::string_view msg,
             const auto &...pms);

  void info(std::string_view msg,
            const auto &...pms);

  void warn(std::string_view msg,
            const auto &...pms);

  void error(std::string_view msg,
             const auto &...pms);

  void fatal(std::string_view msg,
             const auto &...pms);
}

template <>
struct lib::fmt::formatter<lib::logger::pad2d>
{
  void format(
      is_buffer auto &buff,
      lib::logger::pad2d p2)
  {
    if (0 <= p2.i and p2.i <= 9)
      lib::fmt::formatter<char>{}.format(buff, '0');

    lib::fmt::formatter<int>{}.format(buff, p2.i);
  }
};

template <>
struct lib::fmt::formatter<lib::logger::level>
{
  void format(
      is_buffer auto &buff,
      lib::logger::level l)
  {
    constexpr std::string_view ltable[] = {
        "trace", "debug", "info", "warn", "error", "fatal"};
    lib::fmt::formatter<std::string_view>{}.format(buff, ltable[(int)l]);
  }
};

void lib::logger::log(lib::logger::level l,
                       std::string_view msg,
                       const auto &...pms)
{
  std::time_t tnow = std::time(nullptr);
  std::tm *now = std::localtime(&tnow);
  lib::ios::fprintf(stdout, "[#/#/# #:#:#]:[#]:",
                     lib::logger::pad2d{now->tm_mday},
                     lib::logger::pad2d{now->tm_mon},
                     1900 + now->tm_year,
                     lib::logger::pad2d{now->tm_hour},
                     lib::logger::pad2d{now->tm_min},
                     lib::logger::pad2d{now->tm_sec}, l);
  lib::ios::fprintfln(stdout, msg, pms...);
}

void lib::logger::trace(std::string_view msg,
                         const auto &...pms)
{
  lib::logger::log(level::trace, msg, pms...);
}

void lib::logger::debug(std::string_view msg,
                         const auto &...pms)
{
  lib::logger::log(level::debug, msg, pms...);
}

void lib::logger::info(std::string_view msg,
                        const auto &...pms)
{
  lib::logger::log(level::info, msg, pms...);
}

void lib::logger::warn(std::string_view msg,
                        const auto &...pms)
{
  lib::logger::log(level::warn, msg, pms...);
}

void lib::logger::error(std::string_view msg,
                         const auto &...pms)
{
  lib::logger::log(level::error, msg, pms...);
}

void lib::logger::fatal(std::string_view msg,
                         const auto &...pms)
{
  lib::logger::log(level::fatal, msg, pms...);
}

#endif
