#ifndef __log_logger_hpp__
#define __log_logger_hpp__

#include <lib/string.hpp>
#include <lib/iostream.hpp>
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

  template <Output OUT>
  constexpr OUT &operator<<(OUT &buff, lib::logger::pad2d p2) noexcept
  {
    if (p2.i <= 9)
      buff << '0';
    buff << p2.i;
    return buff;
  }

  constexpr OutputSize operator+(OutputSize size, lib::logger::pad2d) noexcept
  {
    return {size.size + 2};
  }

  template <Output OUT>
  constexpr OUT &operator<<(OUT &buff, lib::logger::level l) noexcept
  {
    constexpr lib::StringView ltable[] = {
        "trace", "debug", "info", "warn", "error", "fatal"};
    buff << ltable[(int)l];
    return buff;
  }

  constexpr OutputSize operator+(OutputSize size, lib::logger::level) noexcept
  {
    return {size.size + 5};
  }

  inline void log(level l, const auto &...pms) noexcept
  {
    println('[', l, "] : ", pms...);
  }

  inline void trace(const auto &...pms) noexcept
  {
    log(level::trace, pms...);
  }

  inline void debug(const auto &...pms) noexcept
  {
    log(level::debug, pms...);
  }

  inline void info(const auto &...pms) noexcept
  {
    log(level::info, pms...);
  }

  inline void warn(const auto &...pms) noexcept
  {
    log(level::warn, pms...);
  }

  inline void error(const auto &...pms) noexcept
  {
    log(level::error, pms...);
  }

  inline void fatal(const auto &...pms) noexcept
  {
    log(level::fatal, pms...);
  }
}

#endif
