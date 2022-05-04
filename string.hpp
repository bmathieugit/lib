#ifndef __lib_string_hpp__
#define __lib_string_hpp__

#include <lib/vector.hpp>
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
    BasicString() noexcept = default;

    explicit BasicString(Size max)  noexcept 
        : storage(max)
    {
    }

    BasicString(BasicStringView<C> s) noexcept 
        : BasicString(s.size())
    {
      append(s);
    }

    BasicString(const C *o) noexcept 
        : BasicString(BasicStringView<C>(o, CStringUtils::length(o)))
    {
    }

    template <typename IT>
    BasicString(IT b, IT e) noexcept 
        : storage(b, e)
    {
    }

    BasicString(Strong<C[]>&& buff, Size lgth)  noexcept 
        : storage(move(buff), lgth)
    {
    }

    BasicString(const BasicString<C> &) noexcept  = default;
    BasicString(BasicString &&)  noexcept = default;

    ~BasicString()  noexcept = default;

    BasicString<C> &operator=(const BasicString<C> &) noexcept  = default;
    BasicString<C> &operator=(BasicString<C> &&)  noexcept = default;

  public:
    Size size() const noexcept 
    {
      return storage.size();
    }

    Size capacity() const noexcept 
    {
      return storage.capacity();
    }

    bool empty() const noexcept 
    {
      return storage.empty();
    }

    C *data() noexcept 
    {
      return storage.data();
    }

    const C *data() const noexcept 
    {
      return storage.data();
    }

  public:
    void clear() noexcept 
    {
      storage.clear();
    }

  public:
    void push_back(C c) noexcept 
    {
      storage.push_back(c);
    }

    void push_front(C c) noexcept 
    {
      storage.push_front(c);
    }

    template <typename IT>
    void append(IT b, IT e) noexcept 
    {
      storage.append(b, e);
    }

    void append(const BasicString &o) noexcept 
    {
      append(o.begin(), o.end());
    }

    void append(BasicString &&o) noexcept 
    {
      append(o.begin(), o.end());
    }

    void append(BasicStringView<C> o) noexcept 
    {
      append(o.begin(), o.end());
    }

    void append(const C *o) noexcept 
    {
      append(BasicStringView<C>(o));
    }

  public:
    operator BasicStringView<C>() const noexcept 
    {
      return lib::BasicStringView<C>(this->data(), this->size());
    }

    C &operator[](Size i) noexcept 
    {
      return storage[i];
    }

    const C &operator[](Size i) const noexcept 
    {
      return storage[i];
    }

  public:
    C *begin() noexcept 
    {
      return storage.begin();
    }

    C *end() noexcept 
    {
      return storage.end();
    }

    const C *begin() const noexcept 
    {
      return storage.begin();
    }

    const C *end() const noexcept 
    {
      return storage.end();
    }
  };

  using String = BasicString<char>;
}

#endif
