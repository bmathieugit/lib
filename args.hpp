#ifndef __lib_args_hpp__
#define __lib_args_hpp__

#include <lib/string_view.hpp>
#include <lib/algorithm.hpp>
#include <lib/vector.hpp>
#include <filesystem>

namespace lib
{
  template <typename C>
  class CommandLine
  {
    BasicStringView<C> key_value_sep;
    BasicStringView<C> value_sep;

    Vector<BasicStringView<C>> args;

  public:
    CommandLine(
        BasicStringView<C> _key_value_sep,
        BasicStringView<C> _value_sep,
        int argc, C **argv)
        : key_value_sep(_key_value_sep),
          value_sep(_value_sep),
          args(argc)
    {
      for (int i = 0; i < argc; ++i)
        args.push_back(BasicStringView<C>(argv[i]));
    }

    bool has(BasicStringView<C> arg)
    {
      return get(arg).has_value();
    }

    BasicStringView<C> get(BasicStringView<C> arg)
    {
      for (const auto &cdt : args)
        if (cdt.starts_with(arg))
          return cdt;
      return "";
    }

    BasicStringView<C> val(BasicStringView<C> arg)
    {
      return get(arg).after(key_value_sep);
    }

    static bool isinteger(BasicStringView<C> s)
    {
      if (s.empty())
        return false;

      if (s.starts_with("+") || s.starts_with("-")
        s = s.substr(1);

      for (auto c : s)
        if (!('0' <= c and c <= '9'))
          return false;
      
      return true;
    }

    static int toint(BasicStringView<C> s)
    {
      bool neg = s.starts_with("-");
      bool pos = s.starts_with("+");

      if (pos || neg)
        s = s.after(0);

      int res = 0;

      for (int i = 0; s[i] != '\0'; ++i)
        res = res * 10 + s[i] - '0';

      return neg ? -res : res;
    }

    int integer(BasicStringView<C> arg, int def = 0)
    {
      BasicStringView<C> v = val(arg);
      return isinteger(v) ? toint(v) : def;
    }

    bool boolean(BasicStringView<C> arg, bool def = false);
    BasicStringView<C> string(BasicStringView<C> arg, BasicStringView<C> def = "");
    std::filesystem::path path(BasicStringView<C> arg, BasicStringView<C> def = "");
  };
}

#endif
