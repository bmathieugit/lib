#ifndef __log_logger_hpp__
#define __log_logger_hpp__

#include <cstdio>
#include <ctime>

#include <lib/string_view.hpp>
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

  void log(level l, lib::StringView msg, const auto &...pms)
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

  void trace(lib::StringView msg, const auto &...pms)
  {
    lib::logger::log(level::trace, msg, pms...);
  }

  void debug(lib::StringView msg, const auto &...pms)
  {
    lib::logger::log(level::debug, msg, pms...);
  }

  void info(lib::StringView msg, const auto &...pms)
  {
    lib::logger::log(level::info, msg, pms...);
  }

  void warn(lib::StringView msg, const auto &...pms)
  {
    lib::logger::log(level::warn, msg, pms...);
  }

  void error(lib::StringView msg, const auto &...pms)
  {
    lib::logger::log(level::error, msg, pms...);
  }

  void fatal(lib::StringView msg, const auto &...pms)
  {
    lib::logger::log(level::fatal, msg, pms...);
  }
}

template <>
struct lib::fmt::Formatter<lib::logger::pad2d>
{
  void format(is_buffer auto &buff, lib::logger::pad2d p2)
  {
    if (0 <= p2.i and p2.i <= 9)
      lib::fmt::Formatter<char>().format(buff, '0');

    lib::fmt::Formatter<int>().format(buff, p2.i);
  }
};

template <>
struct lib::fmt::Formatter<lib::logger::level>
{
  void format(is_buffer auto &buff, lib::logger::level l)
  {
    constexpr lib::StringView ltable[] = {
        "trace", "debug", "info", "warn", "error", "fatal"};
    lib::fmt::Formatter<lib::StringView>().format(buff, ltable[(int)l]);
  }
};

#endif
