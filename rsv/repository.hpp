#ifndef __lib_rsv_repository_hpp__
#define __lib_rsv_repository_hpp__

#include <optional>
#include <vector>

namespace lib::rsv
{
  template <typename T>
  using predicate = bool (*)(const T &);
}

namespace lib::rsv
{
  template <typename T>
  class repository
  {
  public:
    virtual ~repository() = default;
    virtual int insert(const T &t) = 0;
    virtual int insert(T &&t) = 0;
    virtual int update(const T &t, predicate<T> pred) = 0;
    virtual int remove(predicate<T> pred) = 0;
    virtual std::optional<T> select_one(predicate<T> pred) = 0;
    virtual std::vector<T> select_many(predicate<T> pred) = 0;
  };
}

#endif