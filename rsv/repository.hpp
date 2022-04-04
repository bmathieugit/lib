#ifndef __lib_rsv_repository_hpp__
#define __lib_rsv_repository_hpp__

#include <optional>

namespace lib::rsv
{
  template <typename T>
  using predicate = bool (*)(const T &);

  template <typename T>
  using one = std::optional<T>;
}

namespace lib::rsv
{
  template <typename T,
            typename PK>
  class repository;

  template <typename T,
            typename PK>
  struct cursor_iterator
  {
    repository<T, PK> *rep;
    predicate<T> pred;
    size_t offset = 0;
    one<T> t;

    cursor_iterator(repository<T, PK> *r,
                    predicate<T> p,
                    size_t o)
        : rep{r},
          pred{p},
          offset{o}
    {
      if (rep != nullptr)
        t = rep->select_one(pred, offset);
    }

    bool operator==(const cursor_iterator<T, PK> &o) const
    {
      return rep->pk(*t) == rep->pk(*o.t);
    }

    bool operator!=(const cursor_iterator<T, PK> &o) const
    {
      return not operator==(o);
    }

    cursor_iterator<T, PK> &operator++()
    {
      if (t.has_value())
      {
        offset++;
        t = std::move(rep->select_one(pred, offset));
      }

      return *this;
    }

    cursor_iterator<T, PK> operator++(int)
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    T &operator*()
    {
      return *t;
    }
  };

  template <typename T, typename PK>
  struct cursor_citerator;

  template <typename T, typename PK>
  class cursor
  {
    repository<T, PK> *rep;
    predicate<T> pred;

  public:
    explicit cursor(repository<T, PK> *r, predicate<T> p) : rep{r}, pred{p} {}

  public:
    cursor_iterator<T, PK> begin()
    {
      return {rep, pred, 0};
    }

    cursor_iterator<T, PK> end()
    {
      return {nullptr, pred, 0};
    }

    cursor_citerator<T, PK> begin() const
    {
      return {rep, pred, 0};
    }

    cursor_citerator<T, PK> end() const
    {
      return {nullptr, pred, 0};
    }

    cursor_citerator<T, PK> cbegin() const
    {
      return {rep, pred, 0};
    }

    cursor_citerator<T, PK> cend() const
    {
      return {nullptr, pred, 0};
    }
  };

  template <typename T, typename PK>
  class repository
  {
  public:
    virtual const PK &pk(const T &t) = 0;
    virtual int insert(const T &t) = 0;
    virtual int insert(T &&t) = 0;
    virtual int update(const T &t, predicate<T> pred) = 0;
    virtual int remove(predicate<T> pred) = 0;
    virtual one<T> select_one(predicate<T> pred, size_t offset = 0) = 0;
    virtual cursor<T, PK> select_many(predicate<T> pred) = 0;
  };
}

#endif