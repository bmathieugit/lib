#ifndef __lib_args_hpp__
#define __lib_args_hpp__

#include <lib/string.hpp>
#include <lib/array.hpp>
#include <lib/basic_types.hpp>

namespace lib
{
  constexpr int toint(StringView s) noexcept
  {
    int res = 0;

    bool neg = s.range().starts_with(sv("-"));
    bool pos = s.range().starts_with(sv("+"));

    if (pos || neg)
    {
      auto tmp = s.sub(1);
      s = StringView(tmp.begin(), tmp.end());
    }

    bool all_digits = s.range().all_of(
        [](char c)
        { return '0' <= c && c <= '9'; });

    if (all_digits)
      for (char c : s)
        res = res * 10 + (c - '0');

    return neg ? -res : res;
  }

  constexpr Size touint(StringView s) noexcept
  {
    Size res = 0;

    bool all_digits = s.range().all_of(
        [](char c)
        { return '0' <= c && c <= '9'; });

    if (all_digits)
      for (char c : s)
        res = res * 10 + (c - '0');

    return res;
  }

  class CommandLine
  {
  private:
    ArrayView<const char *> args;

  public:
    constexpr CommandLine(int argc, const char **argv)
        : args(argv, argc) {}

    constexpr CommandLine(const CommandLine &) noexcept = default;
    constexpr CommandLine(CommandLine &&) noexcept = default;
    constexpr CommandLine &operator=(const CommandLine &) noexcept = default;
    constexpr CommandLine &operator=(CommandLine &&) noexcept = default;
    constexpr ~CommandLine() noexcept = default;

  public:
    constexpr Size size() const noexcept
    {
      return args.size();
    }

    constexpr bool empty() const noexcept
    {
      return args.empty();
    }

    constexpr bool contains(StringView wanted) const noexcept
    {
      return args.range().any_of(
          [wanted](const StringView &arg)
          { return arg.range().starts_with(wanted); });
    }

    constexpr StringView value(StringView wanted) const noexcept
    {
      auto found = args.range().find_if(
          [wanted](const StringView &arg)
          { return arg.range().starts_with(wanted); });

      return found != args.end()
                 ? (StringView(*found)).range().after('=').as<StringView>()
                 : StringView();
    }

    constexpr Size uinteger(StringView wanted) const noexcept
    {
      StringView v = value(wanted);
      return touint(v);
    }

    constexpr long long integer(StringView wanted) const noexcept
    {
      StringView v = value(wanted);
      return toint(v);
    }

    constexpr bool flag(StringView wanted) const noexcept
    {
      return contains(wanted);
    }
  };
}

#endif
