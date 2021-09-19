#ifndef UTIL_VECTOR_H
#define UTIL_VECTOR_H

#include <kernel/kmalloc.h>
#include <stdint.h>

namespace util {

template <class T> struct vector_iterator {
  T *p;
  T &operator*() { return *p; }
  bool operator!=(const vector_iterator<T> &rhs) { return p != rhs.p; }
  void operator++() { p++; }
};

// TODO: memory leaks suck
template <class T> class vec {
public:
  vec() : length(0), capacity(8), data((T *)kmalloc(capacity * sizeof(T))) {}
  vec(size_t cap)
      : length(0), capacity(cap), data((T *)kmalloc(cap * sizeof(T))) {}

  ~vec() {}

  const T &push_back(T value) {
    if (length >= capacity) {
      data = (T *)krealloc((size_t)data, length + 8 * sizeof(T));
      capacity = length + 8;
    }

    data[length++] = value;
    return data[length - 1];
  }

  T pop_back() { return data[--length]; }

  inline size_t len() { return length; };

  inline const T &at(size_t index) const { return data[index]; }
  inline T &at(size_t index) { return data[index]; }

  vector_iterator<T> begin() const { return vector_iterator<T>{&data[0]}; }
  vector_iterator<T> end() const {
    return vector_iterator<T>{&data[length - 1]};
  }

private:
  size_t length;
  size_t capacity;
  T *data;
};

} // namespace util

#endif
