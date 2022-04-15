#ifndef __lib_enumerate_hpp__
#define __lib_enumerate_hpp__

#include <utility>

namespace lib
{
  template <typename T>
  struct EnumerateData
  {
    T &t;
    size_t i;
  };

  template <typename IT>
  struct EnumerateIterator
  {
    size_t i;
    size_t max;
    IT it;

    auto operator*()
    {
      return EnumerateData<decltype(*it)>{*it, i};
    }

    bool operator==(const EnumerateIterator<IT>& o) const
    {
      return it == o.it;
    }

    bool operator!=(const EnumerateIterator<IT>& o) const
    {
      return it != o.it;
    }

    EnumerateIterator<IT> &operator++()
    {
      if (i < max)
      {
        i++;
        it++;
      }

      return *this;
    }

    EnumerateIterator operator++(int)
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }
  };

  template <typename IT>
  EnumerateIterator<IT>
  make_enumerate_iterator(
      size_t i, size_t max, IT it)
  {
    return {i, max, it};
  }

  template <typename T>
  struct EnumerateCData
  {
    const T &t;
    size_t i;
  };

  template <typename IT>
  struct EnumerateCIterator
  {
    size_t i;
    size_t max;
    IT it;

    auto operator*()
    {
      return EnumerateCData<decltype(*it)>{*it, i};
    }

    bool operator==(const EnumerateCIterator<IT>& o) const
    {
      return it == o.it;
    }

    bool operator!=(const EnumerateCIterator<IT>& o) const
    {
      return it != o.it;
    }

    EnumerateCIterator<IT> &operator++()
    {
      if (i < max)
      {
        i++;
        it++;
      }

      return *this;
    }

    EnumerateCIterator operator++(int)
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }
  };

  template <typename IT>
  EnumerateCIterator<IT>
  make_enumerate_citerator(
      size_t i, size_t max, IT it)
  {
    return {i, max, it};
  }

  template <typename C>
  struct EnumerateAdapter
  {
    C *c;

    auto begin()
    {
      return make_enumerate_iterator(
          0, c->size(), c->begin());
    }

    auto end()
    {
      return make_enumerate_iterator(
          c->size(), c->size(), c->end());
    }

    auto begin() const
    {
      return make_enumerate_citerator(
          0, c->size(), c->cbegin());
    }

    auto end() const
    {
      return make_enumerate_citerator(
          c->size(), c->size(), c->cend());
    }

    auto cbegin() const
    {
      return make_enumerate_citerator(
          0, c->size(), c->cbegin());
    }

    auto cend() const
    {
      return make_enumerate_citerator(
          c->size(), c->size(), c->cend());
    }
  };

  template <typename C>
  EnumerateAdapter<C> enumerate(C &c)
  {
    return EnumerateAdapter<C>{&c};
  }

  template <typename C>
  const EnumerateAdapter<const C> enumerate(const C &c)
  {
    return EnumerateAdapter<const C>{&c};
  }
}

#endif