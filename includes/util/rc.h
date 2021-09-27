#ifndef INCLUDES_UTIL_RC_H
#define INCLUDES_UTIL_RC_H

#include <stdint.h>

namespace util {

template <class T> class rc;
template <class T> class weak;

namespace __internal__ {

struct rc_counter {
  // Constructor
  constexpr rc_counter() : count(0), weak_count(0){};
  rc_counter(const rc_counter &) = delete;
  rc_counter &operator=(const rc_counter &) = delete;
  size_t count;
  size_t weak_count;
};

} // namespace __internal__

template <class T> class rc {
public:
  typedef T *ptr_type;
  rc(__internal__::rc_counter *counter, T *ptr) : ptr(ptr), counter(counter) {
    counter->count++;
  };

  rc(const rc<T> &other) {
    ptr = (T *)other.ptr;
    counter = other.counter;
    counter->count++;
  };

  template <class Y> rc(rc<Y> &other) {
    ptr = (T *)other.ptr;
    counter = other.counter;
    counter->count++;
  };

  rc &operator=(const rc &r) {
    if (counter)
      counter->count--;
    ptr = (T *)r.ptr;
    counter = r.counter;
    if (counter)
      counter->count++;
    return *this;
  };

  template <class Y> rc<T> &operator=(const rc<Y> &r) {
    if (counter)
      counter->count--;
    ptr = (T *)r.ptr;
    counter = r.counter;
    if (counter)
      counter->count++;
    return *this;
  };

  ~rc() {
    if (ptr) {
      counter->count--;
      if (counter->count == 0) {
        delete ptr;
        delete counter;
      }
    }
  };

  T *get() { return ptr; }
  T &operator*() { return *ptr; }
  T *operator->() { return ptr; }

  size_t count() { return counter->count; }

  explicit rc() : ptr(nullptr), counter(new __internal__::rc_counter()){};

  constexpr rc(T *ptr) : ptr(ptr), counter(new __internal__::rc_counter()) {
    counter->count++;
  };

  inline constexpr __internal__::rc_counter *get_counter() { return counter; };

private:
  T *ptr;
  __internal__::rc_counter *counter;
  template <class Y> friend class rc;
  template <class Y> friend class weak;
};

template <class T> class weak {
public:
  constexpr weak() : ptr(nullptr), counter(new __internal__::rc_counter()){};

  weak(__internal__::rc_counter *counter, T *ptr) : counter(counter), ptr(ptr) {
    counter->weak_count++;
  };

  inline bool operator!() { return ptr == nullptr; }

  weak<T> &operator=(const rc<T> &r) {
    counter->weak_count--;
    ptr = r.ptr;
    counter = r.counter;
    counter->weak_count++;
    return *this;
  };

  weak<T> &operator=(const weak<T> &r) {
    counter->weak_count--;
    ptr = r.ptr;
    counter = r.counter;
    counter->weak_count++;
    return *this;
  };

  template <class Y> weak<T> &operator=(const rc<Y> &r) {
    counter->weak_count--;
    ptr = r.ptr;
    counter = r.counter;
    counter->weak_count++;
    return *this;
  };

  template <class Y> weak<T> &operator=(const weak<Y> &r) {
    counter->weak_count--;
    ptr = r.ptr;
    counter = r.counter;
    counter->weak_count++;
    return *this;
  };

  template <class Y> weak(rc<Y> &other) {
    ptr = other.get();
    counter = other.get_counter();
    counter->weak_count++;
  };

  template <class Y> weak(weak<Y> &other) {
    ptr = other.ptr;
    counter = other.counter;
    counter->weak_count++;
  };

  weak(weak<T> &other) {
    ptr = other.ptr;
    counter = other.counter;
    counter->weak_count++;
  };

  ~weak() {
    counter->weak_count--;
    if (counter->weak_count == 0 && counter->count == 0) {
      delete ptr;
      delete counter;
    }
  };

  bool expired() { return counter->count == 0; };

  rc<T> lock() const { return rc<T>(counter, ptr); };

  /**
   * The strong count of the pointer
   */
  size_t count() { return counter->count; }

private:
  T *ptr; // Must be first element
  __internal__::rc_counter *counter;
};

template <class K, class... Args> rc<K> make_rc(Args... args) {
  return rc<K>(new K(args...));
};

} // namespace util

#endif // INCLUDES_UTIL_RC_H
