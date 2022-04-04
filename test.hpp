#ifndef __silt_test_hpp__
#define __silt_test_hpp__

#include <vector>
#include <string>
#include <string_view>
#include <exception>
#include <array>
#include <typeinfo>

#include <lib/logger.hpp>

namespace lib::test
{
  struct asserterror
  {
    std::string r;

    std::string reason() const noexcept
    {
      return r;
    }
  };

  struct result
  {
    std::string_view descr;
    bool success;
    std::string reason;
  };

  template <size_t n>
  struct results
  {
    std::string_view descr;
    std::array<result, n> res;

    void print() const
    {
      logger::info("|-Test suite '#'", descr);

      for (const result &r : res)
      {
        logger::info("  |- # : #", r.descr, r.success);

        if (not r.success)
          logger::info("    |- /!\\ #", r.reason);
      }

      size_t total = res.size();
      size_t succeed = std::count_if(
          res.begin(), res.end(), [](const result &r)
          { return r.success; });
      logger::info("  |->>> tests #/# succeed", succeed, total);
    }
  };

  struct test
  {
    std::string_view descr;
    void (*fn)();

    result run() const
    try
    {
      fn();
      return {descr, true};
    }
    catch (const asserterror &e)
    {
      logger::error("oops #", e.reason());
      return {descr, false, e.reason()};
    }
    catch (const std::exception &e)
    {
      logger::error("oops2");
      return {descr, false, e.what()};
    }
    catch (...)
    {
      logger::error("oops3");
      return {descr, false, "unknown error"};
    }
  };

  template <size_t n>
  struct test_suite
  {
    std::string_view descr;
    std::array<test, n> tests;

    results<n> run() const
    {
      results<n> res;
      res.descr = descr;

      for (size_t i = 0; i < tests.size(); ++i)
        res.res[i] = tests[i].run();

      return res;
    }
  };

  struct test_definition
  {
    std::string_view descr;

    test operator()(void (*fn)())
    {
      return {descr, fn};
    }
  };

  struct test_suite_definition
  {
    std::string_view descr;

    auto operator()(const auto... tests)
        -> test_suite<sizeof...(tests)>
    {
      return {descr, {tests...}};
    }
  };
}

lib::test::test_definition operator""_test(const char *descr, size_t n)
{
  return {{descr, n}};
}

lib::test::test_suite_definition operator""_suite(const char *descr, size_t n)
{
  return {{descr, n}};
}

namespace lib::test::is
{
  struct equals
  {
    template <typename A, typename E>
    void operator()(const A &actual,
                    const E &expected) const
    {
      if (not(actual == expected))
        throw asserterror(
            "(actual: #) != (expected: #)"_fmt(
                actual, expected));
    }
  };

  template <typename Ex>
  struct throws
  {
    template <typename F>
    void operator()(F &&f)
    try
    {
      f();
      throw asserterror(
          "expected thrown exception : #"_fmt(
              typeid(Ex).name()));
    }
    catch (const Ex &e)
    {
      // nothing
    }
    catch (const std::exception &e)
    {
      throw asserterror(
          "expected thrown exception : #, but actual : #"_fmt(
              typeid(Ex).name(), e.what()));
    }
    catch (...)
    {
      throw asserterror(
          "expected thrown exception : #, but actual : (...)"_fmt(
              typeid(Ex).name()));
    }
  };
}

namespace lib::test::assert
{
  void that(auto is, const auto &...args)
  {
    is(args...);
  }

  void equals(const auto &a, const auto &b)
  {
    that(lib::test::is::equals{}, a, b);
  }

  template <typename Ex>
  void throws(auto &&f)
  {
    that(is::throws<Ex>{}, f);
  }
}

#endif