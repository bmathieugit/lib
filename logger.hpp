#ifndef __log_logger_hpp__
#define __log_logger_hpp__

#include <lib/string.hpp>
#include <lib/array.hpp>
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

  template <Output OUT>
  constexpr OUT &operator<<(OUT &buff, level l) noexcept
  {
    constexpr lib::Array<lib::StringView, 6> ltable = {
        "trace", "debug", "info", "warn", "error", "fatal"};

    return buff << ltable[(int)l];
  }

  constexpr OutputSize operator+(OutputSize size, level) noexcept
  {
    return {size.size + 5};
  }

  inline void log(level l, const auto &...pms) noexcept
  {
    println(l, " : ", pms...);
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
