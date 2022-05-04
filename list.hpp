#ifndef __lib_list_hpp__
#define __lib_list_hpp__

#include <lib/meta.hpp>
#include <lib/basic_types.hpp>
#include <lib/vector.hpp>
#include <lib/algorithm.hpp>
#include <lib/range.hpp>

namespace lib
{
  template <typename T>
  class List
  {
    struct Node
    {
      Size pred;
      Size next;
      T obj;
    };

    Size first = Size(-1);
    Size last = Size(-1);
    Vector<Node> storage;

    struct Iterator
    {
      List<T> &l;
      Size cur = Size(-1);

      constexpr Iterator &operator++() noexcept
      {
        if (cur != Size(-1))
        {
          Size next = l.storage[cur].next;
          cur = next;
        }

        return *this;
      }

      constexpr bool operator==(const Iterator &o) const noexcept
      {
        return cur == o.cur;
      }

      constexpr bool operator!=(const Iterator &o) const noexcept
      {
        return !(*this == o);
      }

      constexpr Size operator-(const Iterator &o) const noexcept
      {
        Size dist = 0;

        auto i = *this;
        auto oi = o;

        while (i != oi)
        {
          ++i;
          ++dist;
        }

        return dist;
      }

      constexpr Iterator operator++(int) noexcept
      {
        auto tmp = *this;
        ++(*this);
        return tmp;
      }

      constexpr T &operator*() noexcept
      {
        return l.storage[cur].obj;
      }
    };

    struct ConstIterator
    {
      const List<T> &l;
      Size cur = Size(-1);

      constexpr bool operator==(const ConstIterator &o) const noexcept
      {
        return cur == o.cur;
      }

      constexpr bool operator!=(const ConstIterator &o) const noexcept
      {
        return !(*this == o);
      }

      constexpr ConstIterator &operator++() noexcept
      {
        if (cur != Size(-1))
        {
          cur = l.storage[cur].next;
        }

        return *this;
      }

      constexpr ConstIterator operator++(int) noexcept
      {
        auto tmp = *this;
        ++(*this);
        return tmp;
      }

      constexpr Size operator-(const ConstIterator &o) const noexcept
      {
        Size dist = 0;

        auto i = *this;
        auto oi = o;

        while (i != oi)
        {
          ++i;
          ++dist;
        }

        return dist;
      }

      constexpr const T &operator*() const noexcept
      {
        return l.storage[cur].obj;
      }
    };

  public:
    template <typename... U>
    static constexpr List from(U &&...us) noexcept
    {
      List l(sizeof...(U));
      (l.push_back(forward<U>(us)), ...);
      return l;
    }

  public:
    constexpr List() noexcept = default;

    constexpr List(Size _max) noexcept
        : storage(_max)
    {
    }

    template <typename IT>
    constexpr List(IT b, IT e) noexcept
        : List()
    {
      append(b, e);
    }

    constexpr List(const List &) noexcept = default;
    constexpr List(List &&) noexcept = default;
    constexpr ~List() noexcept = default;

    constexpr List &operator=(const List &) noexcept = default;
    constexpr List &operator=(List &&) noexcept = default;

  public:
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

  public:
    constexpr void insert(Iterator it, const T &t) noexcept
    {
      insert(ConstIterator{it.l, it.cur}, static_cast<T &&>(T(t)));
    }

    constexpr void insert(ConstIterator it, const T &t) noexcept
    {
      insert(it, static_cast<T &&>(T(t)));
    }

    constexpr void insert(Iterator it, T &&t) noexcept
    {
      insert(ConstIterator{it.l, it.cur}, static_cast<T &&>(t));
    }

    constexpr void insert(ConstIterator it, T &&t) noexcept
    {
      storage.push_back(Node{Size(-1), Size(-1), static_cast<T &&>(t)});

      if (first == Size(-1) &&
          last == Size(-1))
      {
        first = size() - 1; // 0
        last = size() - 1;  // 0
      }
      else if (it.cur == Size(-1))
      {
        storage[size() - 1].pred = last;
        storage[last].next = size() - 1;
        last = size() - 1;
      }
      else if (it.cur == first)
      {
        storage[size() - 1].next = first;
        storage[first].pred = storage.size() - 1;
        first = storage.size() - 1;
      }
      else
      {
        storage[size() - 1].next = it.cur;
        storage[size() - 1].pred = storage[it.cur].pred;

        if (storage[it.cur].pred != Size(-1))
          storage[storage[it.cur].pred].next = size() - 1;

        storage[it.cur].pred = size() - 1;
      }
    }

    constexpr void push_back(const T &t) noexcept
    {
      insert(end(), t);
    }

    constexpr void push_back(T &&t) noexcept
    {
      insert(end(), t);
    }

    constexpr void push_front(const T &t) noexcept
    {
      insert(begin(), t);
    }

    constexpr void push_front(T &&t) noexcept
    {
      insert(begin(), t);
    }

    constexpr void append(const List &o) noexcept
    {
      for (const T &t : o)
        push_back(t);
    }

    constexpr void append(List &&o) noexcept
    {
      for (T &&t : o)
        push_back(static_cast<T &&>(t));
    }

    template <typename IT>
    constexpr void append(IT b, IT e) noexcept
    {
      while (b != e)
      {
        push_back(*b);
        b = b + 1;
      }
    }

  public:
    constexpr Iterator begin() noexcept
    {
      return Iterator{*this, first};
    }

    constexpr Iterator end() noexcept
    {
      return Iterator{*this};
    }

    constexpr ConstIterator begin() const noexcept
    {
      return ConstIterator{*this, first};
    }

    constexpr ConstIterator end() const noexcept
    {
      return ConstIterator{*this};
    }
  };
}

#endif