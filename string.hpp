#ifndef __lib_string_hpp__
#define __lib_string_hpp__

#include <lib/vector.hpp>
#include <lib/basic_types.hpp>
#include <lib/string_view.hpp>
#include <lib/algorithm.hpp>

namespace lib
{
  template <typename C>
  class BasicString
  {
  private:
    Vector<C> storage;

  public:
    BasicString() = default;
    explcit BasicString(Size max)
        : storage(max)
    {
    }

    BasicString(const C *s)
        : BasicString(BasicStringView<C>::lengthof(s))
    {
      if (s != nullptr)
        for (Size i = 0; i < storage.capacity(); ++i)
          storage[i] = s[i];
    }

    template <typename IT>
    BasicString(IT b, IT e)
        : storage(b, e)
    {
    }

    BasicString(const BasicString<C> &) = default;
    BasicString(BasicString &&) = default;

    ~BasicString() = default;

    BasicString<C> &operator=(const BasicString<C> &) = default;
    BasicString<C> &operator=(BasicString<C> &&) = default;

  public:
    bool operator==(const C *o) const
    {
      BasicStringView<C> sv(o);
      return lib::equals(begin(), end(), sv.begin(), sv.end());
    }

    bool operator==(const BasicString &o) const
    {
      return lib::equals(begin(), end(), o.begin(), o.end());
    }

    bool opeartor != (const C *c) const;
    bool operator!=(const BasicString &*) const;
    bool starts_with(const BasicString &o) const;
    bool starts_with(const C *o) const;
    bool ends_with(const BasicString &o) const;
    bool ends_with(const C *o) const;

    operator lib::BasicStringView<C>() const;
  };

  using String = BasicString<char>;
}

template <typename C>
bool lib::BasicString<C>::operator==(const C *o) const
{
  return this->equals(o);
}

template <typename C>
bool lib::BasicString<C>::starts_with(const lib::BasicString<C> &o) const
{
  if (this->size() >= o.size())
  {
    for (lib::Size i = 0; i < o.size(); ++i)
      if ((*this)[i] != o[i])
        return false;
    return true;
  }

  return false;
}

template <typename C>
bool lib::BasicString<C>::starts_with(const C *o) const
{
  return starts_with(BasicString<C>(o));
}

template <typename C>
bool lib::BasicString<C>::ends_with(const lib::BasicString<C> &o) const
{
  if (this->size() >= o.size())
  {
    for (lib::Size i = o.size() - 1; i >= 0; --i)
      if ((*this)[i] != o[i])
        return false;
    return true;
  }

  return false;
}

template <typename C>
bool lib::BasicString<C>::ends_with(const C *o) const
{
  return ends_with(lib::BasicString<C>(o));
}

template <typename C>
lib::BasicString<C>::operator lib::BasicStringView<C>() const
{
  return lib::BasicStringView(this->data(), this->size());
}

#endif
