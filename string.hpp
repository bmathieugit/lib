#ifndef __lib_string_hpp__
#define __lib_string_hpp__

#include <lib/vector.hpp>
#include <lib/basic_types.hpp>
#include <lib/utility.hpp>
#include <lib/range.hpp>
#include <lib/span.hpp>

namespace lib
{
  template <IsCharacter C>
  using BasicStringView = DelimitedSpan<const C, StrLen<const C>>;

  using StringView = BasicStringView<char>;

  template <IsCharacter C>
  using BasicStringSpan = DelimitedSpan<C, StrLen<C>>;

  using StringSpan = BasicStringSpan<char>;

  template <IsCharacter C>
  class BasicString
  {
  private:
    Vector<C> storage;

  public:
    BasicString() noexcept = default;

    explicit constexpr BasicString(Size max) noexcept
        : storage(max)
    {
    }

    constexpr BasicString(BasicStringView<C> s) noexcept
        : BasicString(s.size())
    {
      lappend(s);
    }

    constexpr BasicString(const C *o) noexcept
        : BasicString(BasicStringView<C>(o))
    {
    }

    template <typename IT>
    constexpr BasicString(IT b, IT e) noexcept
        : storage(b, e) {}

    constexpr BasicString(Strong<C[]> &&buff, Size lgth) noexcept
        : storage(move(buff), lgth) {}

    constexpr BasicString(const BasicString<C> &) noexcept = default;
    constexpr BasicString(BasicString &&) noexcept = default;
    constexpr ~BasicString() noexcept = default;
    constexpr BasicString<C> &operator=(const BasicString<C> &) noexcept = default;
    constexpr BasicString<C> &operator=(BasicString<C> &&) noexcept = default;

  public:
    constexpr auto range() noexcept
    {
      return rangeof(*this);
    }

    constexpr auto range() const noexcept
    {
      return rangeof(*this);
    }

    constexpr Size size() const noexcept
    {
      return storage.size();
    }

    constexpr Size capacity() const noexcept
    {
      return storage.capacity();
    }

    constexpr bool empty() const noexcept
    {
      return storage.empty();
    }

    constexpr C *data() noexcept
    {
      return storage.data();
    }

    constexpr const C *data() const noexcept
    {
      return storage.data();
    }

  public:
    constexpr void clear() noexcept
    {
      storage.clear();
    }

  public:
    constexpr void lpush_back(C c) noexcept
    {
      storage.lpush_back(c);
    }

    constexpr void push_back(C c) noexcept
    {
      storage.push_back(c);
    }

    constexpr void push_front(C c) noexcept
    {
      storage.push_front(c);
    }

    template <typename IT>
    constexpr void lappend(IT b, IT e) noexcept
    {
      storage.lappend(b, e);
    }

    constexpr void lappend(const BasicString &o) noexcept
    {
      lappend(o.begin(), o.end());
    }

    constexpr void lappend(BasicString &&o) noexcept
    {
      lappend(o.begin(), o.end());
    }

    constexpr void lappend(BasicStringView<C> o) noexcept
    {
      lappend(o.begin(), o.end());
    }

    constexpr void lappend(const C *o) noexcept
    {
      lappend(BasicStringView<C>(o));
    }

    template <typename IT>
    constexpr void append(IT b, IT e) noexcept
    {
      storage.append(b, e);
    }

    constexpr void append(const BasicString &o) noexcept
    {
      append(o.begin(), o.end());
    }

    constexpr void append(BasicString &&o) noexcept
    {
      append(o.begin(), o.end());
    }

    constexpr void append(BasicStringView<C> o) noexcept
    {
      append(o.begin(), o.end());
    }

    constexpr void append(const C *o) noexcept
    {
      append(BasicStringView<C>(o));
    }

  public:
    constexpr operator BasicStringView<C>() const noexcept
    {
      return BasicStringView<C>(data(), size());
    }

    constexpr operator BasicStringSpan<C>() noexcept
    {
      return BasicStringSpan<C>(data(), size());
    }

    constexpr C &operator[](Size i) noexcept
    {
      return storage[i];
    }

    constexpr const C &operator[](Size i) const noexcept
    {
      return storage[i];
    }

  public:
    constexpr C *begin() noexcept
    {
      return storage.begin();
    }

    constexpr C *end() noexcept
    {
      return storage.end();
    }

    constexpr const C *begin() const noexcept
    {
      return storage.begin();
    }

    constexpr const C *end() const noexcept
    {
      return storage.end();
    }
  };

  using String = BasicString<char>;
}

constexpr lib::String operator""_s(const char *s, lib::Size n)
{
  return lib::String(s, s + n);
}

constexpr lib::StringView operator""_sv(const char *s, lib::Size n)
{
  return lib::StringView(s, n);
}

#endif
