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
  rc(__internal__::rc_counter *counter, T *ptr) : counter(counter), ptr(ptr) {
    counter->count++;
  };

  rc(rc<T> &other) {
    ptr = other.ptr;
    counter = other.counter;
    counter->count++;
  };

  rc<T> &operator=(const rc<T> &r) {
    counter->count--;
    ptr = r.ptr;
    counter = r.counter;
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

  explicit rc() : counter(new __internal__::rc_counter()), ptr(nullptr){};

  constexpr rc(T *ptr) : counter(new __internal__::rc_counter()), ptr(ptr) {
    counter->count++;
  };

  inline constexpr __internal__::rc_counter *get_counter() { return counter; };

private:
  __internal__::rc_counter *counter;
  T *ptr;
  template <class Y> friend class weak;
};

template <class T> class weak {
public:
  constexpr weak() : counter(new __internal__::rc_counter()), ptr(nullptr){};

  weak(__internal__::rc_counter *counter, T *ptr) : counter(counter), ptr(ptr) {
    counter->weak_count++;
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
  __internal__::rc_counter *counter;
  T *ptr;
};

template <class K, class... Args> rc<K> make_rc(Args... args) {
  return rc<K>(new K(args...));
};

} // namespace util

#endif // INCLUDES_UTIL_RC_H
