#ifndef __lib_rsv_repository_hpp__
#define __lib_rsv_repository_hpp__

#include <optional>
#include <functional>

namespace lib::rsv
{
  template <typename T>
  using predicate = bool (*)(const T &);

  template <typename T>
  using one = std::optional<T>;

  template <typename T>
  class repository;

  template <typename T>
  struct cursor_iterator
  {
    repository<T> *rep = nullptr;
    predicate<T> pred;
    one<T> t = std::nullopt;
    size_t offset = 0;

    cursor_iterator(repository<T> *r,
                    predicate<T> p,
                    size_t o)
        : rep{r},
          pred{p},
          offset{o}
    {
      if (rep != nullptr)
        t = rep->select_one(pred, offset);
    }

    bool operator==(const cursor_iterator<T> &o) const
    {
      return t == o.t;
    }

    bool operator!=(const cursor_iterator<T> &o) const
    {
      return not operator==(o);
    }

    cursor_iterator<T> &operator++()
    {
      if (t.has_value())
      {
        offset++;
        t = std::move(rep->select_one(pred, offset));
      }

      return *this;
    }

    cursor_iterator<T> operator++(int)
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

  template <typename T>
  struct cursor_citerator;

  template <typename T>
  class cursor
  {
    repository<T> *rep;
    predicate<T> pred;

  public:
    explicit cursor(repository<T> *r, predicate<T> p) : rep{r}, pred{p} {}

  public:
    cursor_iterator<T> begin()
    {
      return {rep, pred, 0};
    }

    cursor_iterator<T> end()
    {
      return {nullptr, pred, 0};
    }

    cursor_citerator<T> begin() const
    {
      return {rep, pred, 0};
    }

    cursor_citerator<T> end() const
    {
      return {nullptr, pred, 0};
    }

    cursor_citerator<T> cbegin() const
    {
      return {rep, pred, 0};
    }

    cursor_citerator<T> cend() const
    {
      return {nullptr, pred, 0};
    }
  };

  template <typename T>
  class repository
  {
  public:
    virtual int insert(const T &t) = 0;
    virtual int insert(T &&t) = 0;
    virtual int update(const T &t, predicate<T> pred) = 0;
    virtual int remove(predicate<T> pred) = 0;
    virtual one<T> select_one(predicate<T> pred, size_t offset = 0) = 0;
    virtual cursor<T> select_many(predicate<T> pred) = 0;
  };
}

#endif