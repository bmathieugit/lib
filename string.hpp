#ifndef __lib_string_hpp__
#define __lib_string_hpp__

#include <lib/vector.hpp>
#include <lib/buffer.hpp>
#include <lib/basic_types.hpp>
#include <lib/string_view.hpp>
#include <lib/utility.hpp>
#include <lib/algorithm.hpp>
#include <lib/range.hpp>

namespace lib
{
  template <typename C>
  class BasicString
  {
  private:
    Vector<C> storage;

  public:
    BasicString() = default;

    explicit BasicString(Size max)
        : storage(max)
    {
    }

    BasicString(BasicStringView<C> s)
        : BasicString(s.size())
    {
      append(s);
    }

    BasicString(const C *o)
        : BasicString(BasicStringView<C>(o, CStringUtils::length(o)))
    {
    }

    template <typename IT>
    BasicString(IT b, IT e)
        : storage(b, e)
    {
    }

    BasicString(FlushedBuffer<C> buff)
        : storage(buff)
    {
    }

    BasicString(const BasicString<C> &) = default;
    BasicString(BasicString &&) = default;

    ~BasicString() = default;

    BasicString<C> &operator=(const BasicString<C> &) = default;
    BasicString<C> &operator=(BasicString<C> &&) = default;

  public:
    Size size() const
    {
      return storage.size();
    }

    Size capacity() const
    {
      return storage.capacity();
    }

    bool empty() const
    {
      return storage.empty();
    }

    C *data()
    {
      return storage.data();
    }

    const C *data() const
    {
      return storage.data();
    }

  public:
    void clear()
    {
      storage.clear();
    }

    void remove(Size i)
    {
      storage.remove(i);
    }

  public:
    void push_back(C c)
    {
      storage.push_back(c);
    }

    void push_front(C c)
    {
      storage.push_front(c);
    }

    void pop_back()
    {
      storage.pop_back();
    }

    void pop_front()
    {
      storage.pop_front();
    }

    template <typename IT>
    void append(IT b, IT e)
    {
      storage.append(b, e);
    }

    void append(const BasicString &o)
    {
      append(o.begin(), o.end());
    }

    void append(BasicString &&o)
    {
      append(o.begin(), o.end());
    }

    void append(BasicStringView<C> o)
    {
      append(o.begin(), o.end());
    }

    void append(const C *o)
    {
      append(BasicStringView<C>(o));
    }

  public:
    operator BasicStringView<C>() const
    {
      return lib::BasicStringView<C>(this->data(), this->size());
    }

    C &operator[](Size i)
    {
      return storage[i];
    }

    const C &operator[](Size i) const
    {
      return storage[i];
    }

  public:
    C *begin()
    {
      return storage.begin();
    }

    C *end()
    {
      return storage.end();
    }

    const C *begin() const
    {
      return storage.begin();
    }

    const C *end() const
    {
      return storage.end();
    }
  };

  using String = BasicString<char>;
}

#endif
