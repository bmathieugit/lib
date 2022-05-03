#ifndef __lib_list_hpp__
#define __lib_list_hpp__

#include <lib/meta.hpp>
#include <lib/basic_types.hpp>
#include <lib/vector.hpp>
#include <lib/algorithm.hpp>
#include <lib/range.hpp>

#include <initializer_list>

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

      Iterator &operator++()
      {
        if (cur != Size(-1))
        {
          Size next = l.storage[cur].next;
          cur = next;
        }

        return *this;
      }

      bool operator==(const Iterator &o) const
      {
        return cur == o.cur;
      }

      bool operator!=(const Iterator &o) const
      {
        return !(*this == o);
      }

      Size operator-(const Iterator &o) const
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

      Iterator operator++(int)
      {
        auto tmp = *this;
        ++(*this);
        return tmp;
      }

      T &operator*()
      {
        return l.storage[cur].obj;
      }
    };

    struct ConstIterator
    {
      const List<T> &l;
      Size cur = Size(-1);

      bool operator==(const ConstIterator &o) const
      {
        return cur == o.cur;
      }

      bool operator!=(const ConstIterator &o) const
      {
        return !(*this == o);
      }

      ConstIterator &operator++()
      {
        if (cur != Size(-1))
        {
          cur = l.storage[cur].next;
        }

        return *this;
      }

      ConstIterator operator++(int)
      {
        auto tmp = *this;
        ++(*this);
        return tmp;
      }

      Size operator-(const ConstIterator &o) const
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

      const T &operator*() const
      {
        return l.storage[cur].obj;
      }
    };

  public:
    List() = default;

    List(Size _max)
        : storage(_max)
    {
    }

    List(std::initializer_list<T> init)
        : List(init.begin(), init.end())
    {
    }

    template <typename IT>
    List(IT b, IT e)
        : List()
    {
      append(b, e);
    }

    List(const List &) = default;
    List(List &&) = default;
    ~List() = default;

    List &operator=(const List &) = default;
    List &operator=(List &&) = default;

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

  public:
    void insert(Iterator it, const T &t)
    {
      insert(ConstIterator{it.l, it.cur}, static_cast<T &&>(T(t)));
    }

    void insert(ConstIterator it, const T &t)
    {
      insert(it, static_cast<T &&>(T(t)));
    }

    void insert(Iterator it, T &&t)
    {
      insert(ConstIterator{it.l, it.cur}, static_cast<T &&>(t));
    }

    void insert(ConstIterator it, T &&t)
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

    void push_back(const T &t)
    {
      insert(end(), t);
    }

    void push_back(T &&t)
    {
      insert(end(), t);
    }

    void push_front(const T &t)
    {
      insert(begin(), t);
    }

    void push_front(T &&t)
    {
      insert(begin(), t);
    }

    void append(const List &o)
    {
      for (const T &t : o)
        push_back(t);
    }

    void append(List &&o)
    {
      for (T &&t : o)
        push_back(static_cast<T &&>(t));
    }

    template <typename IT>
    void append(IT b, IT e)
    {
      while (b != e)
      {
        push_back(*b);
        b = b + 1;
      }
    }

  public:
    Iterator begin()
    {
      return Iterator{*this, first};
    }

    Iterator end()
    {
      return Iterator{*this};
    }

    ConstIterator begin() const
    {
      return ConstIterator{*this, first};
    }

    ConstIterator end() const
    {
      return ConstIterator{*this};
    }
  };
}

#endif