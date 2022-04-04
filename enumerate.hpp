#ifndef __lib_enumerate_hpp__
#define __lib_enumerate_hpp__

#include <utility>

namespace lib
{
  template <typename T>
  struct enumerate_data
  {
    T &t;
    size_t i;
  };

  template <typename IT>
  struct enumerate_iterator
  {
    size_t i;
    size_t max;
    IT it;

    auto operator*()
    {
      return enumerate_data<decltype(*it)>{*it, i};
    }

    bool operator==(const enumerate_iterator<IT>& o) const
    {
      return it == o.it;
    }

    bool operator!=(const enumerate_iterator<IT>& o) const
    {
      return it != o.it;
    }

    enumerate_iterator<IT> &operator++()
    {
      if (i < max)
      {
        i++;
        it++;
      }

      return *this;
    }

    enumerate_iterator operator++(int)
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }
  };

  template <typename IT>
  enumerate_iterator<IT>
  make_enumerate_iterator(
      size_t i, size_t max, IT it)
  {
    return {i, max, it};
  }

  template <typename T>
  struct enumerate_cdata
  {
    const T &t;
    size_t i;
  };

  template <typename IT>
  struct enumerate_citerator
  {
    size_t i;
    size_t max;
    IT it;

    auto operator*()
    {
      return enumerate_cdata<decltype(*it)>{*it, i};
    }

    bool operator==(const enumerate_citerator<IT>& o) const
    {
      return it == o.it;
    }

    bool operator!=(const enumerate_citerator<IT>& o) const
    {
      return it != o.it;
    }

    enumerate_citerator<IT> &operator++()
    {
      if (i < max)
      {
        i++;
        it++;
      }

      return *this;
    }

    enumerate_citerator operator++(int)
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }
  };

  template <typename IT>
  enumerate_citerator<IT>
  make_enumerate_citerator(
      size_t i, size_t max, IT it)
  {
    return {i, max, it};
  }

  template <typename C>
  struct enumerate_adapter
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
  enumerate_adapter<C> enumerate(C &c)
  {
    return enumerate_adapter<C>{&c};
  }

  template <typename C>
  const enumerate_adapter<const C> enumerate(const C &c)
  {
    return enumerate_adapter<const C>{&c};
  }
}

#endif