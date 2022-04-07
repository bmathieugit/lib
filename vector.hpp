#ifndef __lib_vector_hpp__
#define __lib_vector_hpp__

#include <lib/basic_types.hpp>
#include <initializer_list>

namespace lib
{
  template <typename T>
  class VectorStorage
  {
  private:
    Size lgth;
    Size max;
    T *storage;

  public:
    VectorStorage();
    VectorStorage(std::initializer_list<T> init);
    explicit VectorStorage(Size _max);
    VectorStorage(const VectorStorage &);
    VectorStorage(VectorStorage &&);
    ~VectorStorage();

    VectorStorage &operator=(const VectorStorage &);
    VectorStorage &operator=(VectorStorage &&);

  public:
    Size size() const;
    Size capacity() const;
    T *data();
    const T *data() const;

  public:
    Size increase();
    Size clear();
    Size remove(Size i);

  public:
    Size push_back(const T &t);
    Size push_back(T &&);
    Size push_front(const T &t);
    Size push_front(T &&);

    Size pop_back();
    Size pop_front();

  public:
    T &operator[](Size i);
    const T &operator[](Size i) const;
  };

  template <typename T>
  class Vector : public VectorStorage<T>
  {

  public:
    explicit Vector(Size max);
    Vector(std::initializer_list<T> init);
    Vector() = default;
    Vector(const Vector &) = default;
    Vector(Vector &&) = default;
    ~Vector() = default;

    Vector &operator=(const Vector &) = default;
    Vector &operator=(Vector &&) = default;

  public:
    T *begin();
    T *end();
    const T *begin() const;
    const T *end() const;
    const T *cbegin() const;
    const T *cend() const;
  };
}

template <typename T>
lib::VectorStorage<T>::VectorStorage()
    : lgth(0), max(0), storage(nullptr) {}

template <typename T>
lib::VectorStorage<T>::VectorStorage(
    std::initializer_list<T> init)
    : lib::VectorStorage<T>(init.size())
{
  for (const T &t : init)
    push_back(t);
}

template <typename T>
lib::VectorStorage<T>::VectorStorage(lib::Size _max)
    : lgth(0), max(_max), storage{new T[max]} {}

template <typename T>
lib::VectorStorage<T>::VectorStorage(
    const lib::VectorStorage<T> &o)
    : lgth(o.lgth), max(o.max), storage(new T[max])
{
  for (lib::Size i = 0; i < lgth; ++i)
    storage[i] = o.storage[i];
}

template <typename T>
lib::VectorStorage<T>::VectorStorage(
    lib::VectorStorage<T> &&o)
    : lgth(o.lgth), max(o.max), storage(o.storage)
{
  o.lgth = 0;
  o.max = 0;
  o.storage = nullptr;
}

template <typename T>
lib::VectorStorage<T>::~VectorStorage()
{
  lgth = 0;
  max = 0;
  delete[] storage;
  storage = nullptr;
}

template <typename T>
lib::VectorStorage<T> &lib::VectorStorage<T>::operator=(
    const lib::VectorStorage<T> &o)
{
  if (this != &o)
  {
    lgth = o.lgth;
    max = o.max;
    storage = new T[max];

    for (lib::Size i = 0; i < lgth; ++i)
      storage[i] = o.storage[i];
  }

  return *this;
}

template <typename T>
lib::VectorStorage<T> &lib::VectorStorage<T>::operator=(
    lib::VectorStorage<T> &&o)
{
  if (this != &o)
  {
    lgth = o.lgth;
    max = o.max;
    storage = new T[max];

    for (lib::Size i = 0; i < lgth; ++i)
      storage[i] = static_cast<T &&>(o.storage[i]);
  }

  return *this;
}

template <typename T>
lib::Size lib::VectorStorage<T>::size() const
{
  return lgth;
}

template <typename T>
lib::Size lib::VectorStorage<T>::capacity() const
{
  return max;
}

template <typename T>
T *lib::VectorStorage<T>::data()
{
  return storage;
}

template <typename T>
const T *lib::VectorStorage<T>::data() const
{
  return storage;
}

template <typename T>
lib::Size lib::VectorStorage<T>::increase()
{
  T *tmp = storage;
  storage = new T[max * 2];

  for (Size i = 0; i < lgth; ++i)
    storage[i] = static_cast<T &&>(tmp[i]);

  delete[] tmp;
  max = max * 2;
  return max;
}

template <typename T>
lib::Size lib::VectorStorage<T>::clear()
{
  delete[] storage;
  storage = nullptr;
  max = 0;
  lgth = 0;
  return lgth;
}

template <typename T>
lib::Size lib::VectorStorage<T>::remove(lib::Size i)
{
  storage[i].~T();

  for (lib::Size j = i + 1; j < lgth; ++j)
    storage[j - 1] = static_cast<T &&>(storage[j]);

  lgth = lgth - 1;
  return lgth;
}

template <typename T>
lib::Size lib::VectorStorage<T>::push_back(const T &t)
{
  if (lgth >= max)
    increase();

  storage[size()] = t;
  lgth = lgth + 1;
  return lgth;
}

template <typename T>
lib::Size lib::VectorStorage<T>::push_back(T &&t)
{
  if (lgth >= max)
    increase();

  storage[size()] = t;
  lgth = lgth + 1;
  return lgth;
}

template <typename T>
lib::Size lib::VectorStorage<T>::push_front(const T &t)
{
  if (lgth >= max)
    increase();

  for (lib::Size i = lgth; i > 0; --i)
    storage[i] = static_cast<T &&>(storage[i - 1]);

  storage[0] = t;
  lgth = lgth + 1;
  return lgth;
}

template <typename T>
lib::Size lib::VectorStorage<T>::push_front(T &&t)
{
  if (lgth >= max)
    increase();

  for (lib::Size i = lgth; i > 0; --i)
    storage[i] = static_cast<T &&>(storage[i - 1]);

  storage[0] = static_cast<T &&>(t);
  lgth = lgth + 1;
  return lgth;
}

template <typename T>
lib::Size lib::VectorStorage<T>::pop_back()
{
  return remove(lgth - 1);
}

template <typename T>
lib::Size lib::VectorStorage<T>::pop_front()
{
  return remove(0);
}

template <typename T>
T &lib::VectorStorage<T>::operator[](lib::Size i)
{
  return storage[i];
}

template <typename T>
const T &lib::VectorStorage<T>::operator[](lib::Size i) const
{
  return storage[i];
}

template <typename T>
lib::Vector<T>::Vector(lib::Size max)
    : lib::VectorStorage<T>(max) {}

template <typename T>
lib::Vector<T>::Vector(std::initializer_list<T> init)
    : lib::VectorStorage<T>(init) {}

template <typename T>
T *lib::Vector<T>::begin()
{
  return lib::VectorStorage<T>::data();
}

template <typename T>
T *lib::Vector<T>::end()
{
  return lib::VectorStorage<T>::data() +
         lib::VectorStorage<T>::size();
}

template <typename T>
const T *lib::Vector<T>::begin() const
{
  return lib::VectorStorage<T>::data();
}

template <typename T>
const T *lib::Vector<T>::end() const
{
  return lib::VectorStorage<T>::data() +
         lib::VectorStorage<T>::size();
}

template <typename T>
const T *lib::Vector<T>::cbegin() const
{
  return lib::VectorStorage<T>::data();
}

template <typename T>
const T *lib::Vector<T>::cend() const
{
  return lib::VectorStorage<T>::data() +
         lib::VectorStorage<T>::size();
}

#endif