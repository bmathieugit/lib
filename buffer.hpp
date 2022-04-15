#ifndef __lib_string_buffer_hpp__
#define __lib_string_buffer_hpp__

#include <lib/basic_types.hpp>

namespace lib
{
  template <typename T>
  struct FlushedBuffer
  {
    Size max;
    Size lgth;
    T *buff;
  };

  template <typename T>
  class Buffer
  {
    Size max;
    Size lgth;
    T *buff;

  public:
    Buffer(Size _max)
        : max(_max),
          lgth(0),
          buff(new T[max])
    {
    }

    Buffer(const Buffer &o) = delete;
    //     : Buffer(o.max),
    //       lgth(o.lgth)
    // {
    //   for (Size i = 0; i < o.lgth; ++i)
    //     buff[i] = o.buff[i];
    // }

    Buffer(Buffer &&) = delete;
    //     : max(o.max),
    //       lgth(o.lgth),
    //       buff(o.buff)
    // {
    //   o.max = 0;
    //   o.lgth = 0;
    //   o.buff = nullptr;
    // }

    ~Buffer()
    {
      max = 0;
      lgth = 0;
      delete[] buff;
    }

    Buffer &operator=(const Buffer &o) = delete;
    // {
    //   if (this != &o)
    //   {
    //     delete[] buff;
    //     lgth = o.lgth;
    //     max = o.max;
    //     buff = new T[max];

    //     for (Size i = 0; i < o.lgth; ++i)
    //       buff[i] = o.buff[i];
    //   }

    //   return *this;
    // }

    Buffer &operator=(Buffer &&o) = delete;
    // {
    //   if (this != &o)
    //   {
    //     delete[] buff;
    //     lgth = o.lgth;
    //     max = o.max;
    //     buff = o.buff;
    //     o.lgth = 0;
    //     o.max = 0;
    //     o.buff = nullptr;
    //   }

    //   return *this;
    // }

  public:
    Size capacity() const
    {
      return max;
    }

    Size size() const
    {
      return lgth;
    }

    bool empty() const
    {
      return size() == 0;
    }

  public:
    void push_back(const T &t)
    {
      if (lgth < max)
      {
        buff[lgth] = t;
        ++lgth;
      }
    }

    void push_back(T &&t)
    {
      if (lgth < max)
      {
        buff[lgth] = static_cast<T &&>(t);
        ++lgth;
      }
    }

    void pop_back()
    {
      --lgth;
    }

    FlushedBuffer<T> flush()
    {
      FlushedBuffer<T> fb{max, lgth, buff};

      max = 0;
      lgth = 0;
      buff = nullptr;

      return fb;
    }
  };
}

#endif