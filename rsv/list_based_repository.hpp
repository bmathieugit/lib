#ifndef __lib_list_based_repository_hpp__
#define __lib_list_based_repository_hpp__

#include <lib/rsv/repository.hpp>
#include <list>
#include <algorithm>

namespace lib::rsv
{

  template <typename T>
  class list_based_repository
      : public repository<T>
  {
  public:
    std::list<T> data;

  public:
    list_based_repository(const std::list<T> &d) : data(d) {}
    list_based_repository(std::list<T> &&d) : data(d) {}
    
    virtual ~list_based_repository() = default;
    
    virtual int insert(const T &t);
    virtual int insert(T &&t);
    virtual int update(const T &t, predicate<T> pred);
    virtual int remove(predicate<T> pred);
    virtual std::optional<T> select_one(predicate<T> pred);
    virtual std::vector<T> select_many(predicate<T> pred);
  };
}

template <typename T>
int lib::rsv::list_based_repository<T>::
    insert(const T &t)
{
  data.push_back(t);
  return 1;
}

template <typename T>
int lib::rsv::list_based_repository<T>::
    insert(T &&t)
{
  data.push_back(t);
  return 1;
}

template <typename T>
int lib::rsv::list_based_repository<T>::
    update(const T &t, lib::rsv::predicate<T> pred)
{
  int nb = 0;

  for (T &d : data)
    if (pred(d))
    {
      d = t;
      ++nb;
    }

  return nb;
}

template <typename T>
int lib::rsv::list_based_repository<T>::
    remove(lib::rsv::predicate<T> pred)
{
  return std::erase_if(data, pred);
}

template <typename T>
std::optional<T> lib::rsv::list_based_repository<T>::
    select_one(lib::rsv::predicate<T> pred)
{
  for (const T &t : data)
    if (pred(t))
      return t;

  return {};
}

template <typename T>
std::vector<T> lib::rsv::list_based_repository<T>::
    select_many(lib::rsv::predicate<T> pred)
{
  std::vector<T> objs;

  for (const T &t : data)
    if (pred(t))
      objs.push_back(t);

  return objs;
}

#endif