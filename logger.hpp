#ifndef __log_logger_hpp__
#define __log_logger_hpp__

#include <cstdio>
#include <ctime>
#include <source_location>

#include <lib/string.hpp>
#include <lib/ios.hpp>
#include <lib/format.hpp>
#include <lib/utility.hpp>

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

  void log(level l, StringView msg, const auto &...pms) noexcept
  {
    constexpr std::source_location location = std::source_location::current();
    constexpr StringView filename(location.file_name());
    constexpr Size line(location.line());
    std::time_t tnow = std::time(nullptr);
    std::tm *now = std::localtime(&tnow);
    ios::fprintf(stdout, "[#]:[#/#/# #:#:#]:[#:#]:",
                 l,
                 logger::pad2d{now->tm_mday},
                 logger::pad2d{now->tm_mon},
                 1900 + now->tm_year,
                 logger::pad2d{now->tm_hour},
                 logger::pad2d{now->tm_min},
                 logger::pad2d{now->tm_sec},
                 filename,
                 line);
    ios::fprintfln(stdout, msg, pms...);
  }

  void trace(StringView msg, const auto &...pms) noexcept
  {
    logger::log(level::trace, msg, pms...);
  }

  void debug(StringView msg, const auto &...pms) noexcept
  {
    logger::log(level::debug, msg, pms...);
  }

  void info(StringView msg, const auto &...pms) noexcept
  {
    logger::log(level::info, msg, pms...);
  }

  void warn(StringView msg, const auto &...pms) noexcept
  {
    logger::log(level::warn, msg, pms...);
  }

  void error(StringView msg, const auto &...pms) noexcept
  {
    logger::log(level::error, msg, pms...);
  }

  void fatal(StringView msg, const auto &...pms) noexcept
  {
    logger::log(level::fatal, msg, pms...);
  }
}

namespace lib::fmt
{
  template <typename buffer>
  constexpr FormatStream<buffer> &operator<<(
      FormatStream<buffer> &buff, lib::logger::pad2d p2) noexcept
  {
    if (0 <= p2.i and p2.i <= 9)
      buff << '0';
    buff << p2.i;
    return buff;
  }

  constexpr FormatSize operator+(FormatSize size, lib::logger::pad2d) noexcept
  {
    return {size.size + 2};
  }

  template <typename buffer>
  constexpr FormatStream<buffer> &operator<<(
      FormatStream<buffer> &buff, lib::logger::level l) noexcept
  {
    constexpr lib::StringView ltable[] = {
        "trace", "debug", "info", "warn", "error", "fatal"};
    buff << ltable[(int)l];
    return buff;
  }

  constexpr FormatSize operator+(FormatSize size, lib::logger::level) noexcept
  {
    return {size.size + 5};
  }
}
#endif
