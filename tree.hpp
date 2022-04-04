#ifndef __lib_tree_hpp__
#define __lib_tree_hpp__

#include <vector>

namespace lib
{
  template <typename T>
  class tree;

  template <typename T>
  struct tree_node;

  template <typename T>
  struct child_iterator
  {
    static const unsigned npos = unsigned(-1);

    tree<T> *tr;
    unsigned index = npos;

    tree_node<T> &operator*()
    {
      return (*tr)[index];
    }

    child_iterator &operator++()
    {
      if (index != npos)
        index = (**this).next;

      return *this;
    }

    child_iterator operator++(int)
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(
        const child_iterator &o)
    {
      return index == o.index;
    }

    bool operator!=(
        const child_iterator &o)
    {
      return not operator==(o);
    }
  };

  template <typename T>
  struct child_citerator
  {
    static const unsigned npos = unsigned(-1);

    const tree<T> *tr;
    unsigned index = npos;

    const tree_node<T> &operator*()
    {
      return (*tr)[index];
    }

    child_citerator &operator++()
    {
      if (index != npos)
        index = (**this).next;

      return *this;
    }

    child_citerator operator++(int)
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(
        const child_citerator &o)
    {
      return index == o.index;
    }

    bool operator!=(
        const child_citerator &o)
    {
      return not operator==(o);
    }
  };

  template <typename T>
  struct tree_node
  {
    static const unsigned npos = unsigned(-1);

    T obj;
    tree<T> *tr;

    unsigned next = npos;
    unsigned child = npos;
    unsigned lchild = npos;

  public:
    child_iterator<T> begin()
    {
      return {tr, child};
    }

    child_iterator<T> end()
    {
      return {tr, npos};
    }

    child_citerator<T> begin() const
    {
      return {tr, child};
    }

    child_citerator<T> end() const
    {
      return {tr, npos};
    }

    child_citerator<T> cbegin() const
    {
      return {tr, child};
    }

    child_citerator<T> cend() const
    {
      return {tr, npos};
    }
  };

  template <typename T>
  class tree
  {
    static const unsigned npos = unsigned(-1);

  public:
    std::vector<tree_node<T>> nodes;

  public:
    tree() = default;
    tree(const tree &) = default;
    tree(tree &&) = default;
    tree &operator=(const tree &) = default;
    tree &operator=(tree &&) = default;
    ~tree() = default;

    explicit tree(size_t n) : nodes(n) {}

  public:
    size_t size() const
    {
      return nodes.size();
    }

  public:
    unsigned push(T &&t)
    {
      if (size() == 0)
        nodes.push_back({std::move(t), this});

      return size() == 1 ? 0 : npos;
    }

    unsigned push(const T &t)
    {
      if (size() == 0)
        nodes.push_back({t, this});

      return size() == 1 ? 0 : npos;
    }

    unsigned push(
        T &&t,
        unsigned parent)
    {
      if (parent >= size())
        return npos;

      nodes[parent].lchild =
          *&(nodes[parent].child == npos
                 ? nodes[parent].child
                 : nodes[nodes[parent].lchild].next) =
              size();

      nodes.push_back({std::move(t), this});

      return size() - 1;
    }

    unsigned push(
        const T &t,
        unsigned parent)
    {
      return push(std::move(t), parent);
    }

    const tree_node<T> &
    operator[](unsigned i) const
    {
      return nodes[i];
    }

    tree_node<T> &
    operator[](unsigned i)
    {
      return nodes[i];
    }
  };
}

#endif