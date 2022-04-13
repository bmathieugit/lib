#ifndef __lib_mayby_hpp__
#define __lib_mayby_hpp__

#include <optional>

namespace lib
{
  class Nothing
  {
  };

  template <typename T>
  class Maybe
  {
  private:
    bool has = false;

    union
    {
      Nothing noth;
      T val;
    };

  public:
    Maybe()
        : has(false),
          noth()
    {
    }

    Maybe(const Nothing &)
        : Maybe()
    {
    }

    Maybe(const T &t)
        : has(true),
          val(t)
    {
    }

    Maybe(T &&t)
        : has(true),
          val(static_cast<T &&>(t))
    {
    }

    ~Maybe()
    {
      if (has)
        val.~T();
    }

    Maybe<T> &operator=(const Maybe<T> &o)
    {
      if (this != &o)
      {
        has = o.has;

        if (o.has)
          val = o.val;
        else
          noth = Nothing();
      }

      return *this;
    }

    Maybe<T> &operator=(Maybe<T> &&o)
    {
      if (this != &o)
      {
        has = o.has;

        if (o.has)
          val = static_cast<T &&>(o.val);
        else
          noth = Nothing();
      }

      return *this;
    }

    Maybe<T> &operator=(const T &o)
    {
      has = true;
      val = o;

      return *this;
    }

    Maybe<T> &operator=(const T &&o)
    {
      has = true;
      val = static_cast<T &&>(o);

      return *this;
    }

    Maybe<T> &operator=(const Nothing &)
    {
      has = false;
      noth = Nothing();

      return *this;
    }

  public:
    bool has_value() const
    {
      return has;
    }
    
    explicit operator bool() const
    {
      return has;
    }

    T &value() &
    {
      return val;
    }
    
    const T &value() const &
    {
      return val;
    }

    T &&value() &&
    {
      return val;
    }
    
    const T &value() const &&
    {
      return val;
    }
  };
}

#endif