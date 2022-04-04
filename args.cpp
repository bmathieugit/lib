#include <string_view>
#include <algorithm>
#include <span>

#include <lib/args.hpp>

static bool __isinteger(std::string_view s)
{
  if (s.empty())
    return false;

  if (s[0] == '+' or s[0] == '-')
    s = s.substr(1);

  return std::all_of(
      s.begin(), s.end(), [](char c)
      { return '0' <= c and c <= '9'; });
}

static int __toint(std::string_view s)
{
  bool neg = s[0] == '-';

  if (s[0] == '+' or s[0] == '-')
    s = s.substr(1);

  int res = 0;

  for (int i = 0; s[i] != '\0'; ++i)
    res = res * 10 + s[i] - '0';

  return neg ? -res : res;
}

lib::cmdline::cmdline(
    std::string_view _prefix,
    std::string_view _key_value_sep,
    std::string_view _value_sep,
    int argc, char **argv)
    : prefix(_prefix),
      key_value_sep(_key_value_sep),
      value_sep(_value_sep),
      args(std::span<char *>(argv, argc))
{
}

bool lib::cmdline::has(
    std::string_view arg)
{
  return not get(arg).empty();
}

std::string_view lib::cmdline::get(
    std::string_view arg)
{
  auto found =
      std::find_if(
          args.begin(), args.end(),
          [&](const std::string_view &cdt)
          {
            return cdt.starts_with(prefix) and
                   cdt.substr(prefix.size())
                       .starts_with(arg);
          });

  return found != args.end() ? *found : "";
}

std::string_view lib::cmdline::val(
    std::string_view arg)
{
  std::string_view fullv = get(arg);
  size_t i = fullv.find(key_value_sep);

  if (i == std::string_view::npos)
    return "";
  else
    return fullv.substr(i + key_value_sep.size());
}

int lib::cmdline::integer(
    std::string_view arg, int def)
{
  std::string_view v = val(arg);
  bool isint = __isinteger(v);
  return isint ? __toint(v) : def;
}

bool lib::cmdline::boolean(
    std::string_view arg, bool def)
{
  std::string_view v = val(arg);

  if (v == "true")
    return true;
  else if (v == "false")
    return false;
  else
    return def;
}

std::string_view lib::cmdline::string(
    std::string_view arg, std::string_view def)
{
  std::string_view v = val(arg);
  return v.empty() ? def : v;
}

std::filesystem::path lib::cmdline::path(
    std::string_view arg,
    std::string_view def)
{
  std::string_view v = val(arg);
  return std::filesystem::path(v.empty() ? def : v);
}