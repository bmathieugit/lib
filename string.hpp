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
    constexpr void increase(Size more) noexcept
    {
      storage.increase(more);
    }

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
      storage.lappend(o.storage());
    }

    constexpr void lappend(BasicString &&o) noexcept
    {
      storage.lappend(move(o.storage()));
    }

    constexpr void lappend(BasicStringView<C> o) noexcept
    {
      storage.lappend(o);
    }

    constexpr void lappend(BasicStringSpan<C> o) noexcept
    {
      storage.lappend(o);
    }

    constexpr void lappend(const C *o) noexcept
    {
      storage.lappend(o, o + StrLen<C>()(o));
    }

    template <typename IT>
    constexpr void append(IT b, IT e) noexcept
    {
      storage.append(b, e);
    }

    constexpr void append(const BasicString &o) noexcept
    {
      storage.append(o.storage);
    }

    constexpr void append(BasicString &&o) noexcept
    {
      storage.append(move(o.storage));
    }

    constexpr void append(BasicStringView<C> o) noexcept
    {
      storage.append(o);
    }

    constexpr void append(BasicStringSpan<C> o) noexcept
    {
      storage.append(o);
    }

    constexpr void append(const C *o) noexcept
    {
      storage.append(o, o + StrLen<C>()(o));
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

template <lib::Size n>
constexpr lib::String str(const char (&s) [n]) 
{
  return lib::String(s, s + n);
}

template <lib::Size n>
constexpr lib::StringView sv(const char (&s) [n])
{
  return lib::StringView(s, n);
}

#endif
