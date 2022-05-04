#ifndef __silt_test_hpp__
#define __silt_test_hpp__

#include <lib/vector.hpp>
#include <lib/string.hpp>
#include <lib/string_view.hpp>
#include <lib/algorithm.hpp>
#include <lib/logger.hpp>
#include <lib/array.hpp>

#include <exception>

#include <typeinfo>

namespace lib::test
{
  struct AssertError
  {
    lib::String r;

    lib::String reason() const noexcept
    {
      return r;
    }
  };

  struct Result
  {
    lib::StringView descr;
    bool success;
    lib::String reason;
  };

  template <size_t n>
  struct Results
  {
    lib::StringView descr;
    lib::Array<Result, n> res;

    void print() const noexcept
    {
      logger::info("|- Test suite '#'", descr);

      for (const Result &r : res)
      {
        logger::info("  |- # : #", r.descr, r.success);

        if (not r.success)
          logger::info("    |- /!\\ #", r.reason);
      }

      size_t ntotal = res.size();
      size_t nsucceed = lib::CountIfAlgorithm()(
          res.begin(), res.end(), [](const Result &r)
          { return r.success; });
      logger::info("  |->>> tests #/# succeed", nsucceed, ntotal);
    }
  };

  struct test
  {
    lib::StringView descr;
    void (*fn)();

    Result run() const noexcept
    try
    {
      fn();
      return {descr, true};
    }
    catch (const AssertError &e)
    {
      return {descr, false, e.reason()};
    }
    catch (const std::exception &e)
    {
      return {descr, false, e.what()};
    }
    catch (...)
    {
      return {descr, false, "unknown error"};
    }
  };

  template <size_t n>
  struct TestSuite
  {
    lib::StringView descr;
    lib::Array<test, n> tests;

    Results<n> run() const noexcept
    {
      Results<n> res;
      res.descr = descr;

      for (size_t i = 0; i < tests.size(); ++i)
        res.res[i] = tests[i].run();

      return res;
    }
  };

  struct TestDefinition
  {
    lib::StringView descr;

    test operator()(void (*fn)())
    {
      return {descr, fn};
    }
  };

  struct TestSuiteDefinition
  {
    lib::StringView descr;

    auto operator()(const auto... tests)
        -> TestSuite<sizeof...(tests)>
    {
      return {descr, {tests...}};
    }
  };
}

lib::test::TestDefinition operator""_test(const char *descr, size_t n)
{
  return {{descr, n}};
}

lib::test::TestSuiteDefinition operator""_suite(const char *descr, size_t n)
{
  return {{descr, n}};
}

namespace lib::test::is
{
  struct Equals
  {
    template <typename A, typename E>
    void operator()(const A &actual,
                    const E &expected) const
    {
      if (!(actual == expected))
        throw AssertError(
            "(actual: #) != (expected: #)"_fmt(
                actual, expected));
    }
  };

  struct NotEquals
  {
    template <typename A, typename E>
    void operator()(const A &actual,
                    const E &expected) const
    {
      if (!(actual != expected))
        throw AssertError(
            "(actual: #) == (expected: #)"_fmt(
                actual, expected));
    }
  };

  template <typename Ex>
  struct Throws
  {
    template <typename F>
    void operator()(F &&f)
    try
    {
      f();
      throw AssertError(
          "expected thrown exception : #"_fmt(
              typeid(Ex).name()));
    }
    catch (const Ex &e)
    {
      // nothing
    }
    catch (const std::exception &e)
    {
      throw AssertError(
          "expected thrown exception : #, but actual : #"_fmt(
              typeid(Ex).name(), e.what()));
    }
    catch (...)
    {
      throw AssertError(
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
    that(lib::test::is::Equals(), a, b);
  }

  void not_equals(const auto &a, const auto &b)
  {
    that(lib::test::is::NotEquals(), a, b);
  }

  template <typename Ex>
  void throws(auto &&f)
  {
    that(is::Throws<Ex>(), f);
  }
}

#endif