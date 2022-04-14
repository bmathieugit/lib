#ifndef __lib_string_buffer_hpp__
#define __lib_string_buffer_hpp__

#include <lib/basic_types.hpp>

namespace lib
{
  template <typename T>
  class Buffer
  {
    T *buff;
    Size max;
    Size lgth;

  public:
    Buffer(Size _lgth) : lgth(_lgth), buff(new T[lgth]) {}
    Buffer(const Buffer &o)
        : Buffer(o.lgth)
    {
      for (Size i = 0; i < o.lgth; ++i)
        buff[i] = o.buff[i];
    }

    Buffer(Buffer &&)
        : lgth(o.lgth), buff(o.buff)
    {
      o.lgth = 0;
      o.buff = nullptr;
    }

    ~Buffer()
    {
      lgth = 0;
      delete[] buff;
    }

    Buffer &operator=(const Buffer &o)
    {
      if (this != &o)
      {
        for (Size i = 0; i < o.lgth; ++i)
          buff[i] = o.buff[i];
      }
    }

    Buffer &operator=(Buffer &&o)
    {
    }
  };

  template <typename C>
  class StringBuffer
  {
  };
}

#endif