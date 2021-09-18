#ifndef INCLUDES_UTIL_RC_H
#define INCLUDES_UTIL_RC_H

#include <stdint.h>

namespace util {

namespace __internal__ {

struct rc_counter {
  // Constructor
  constexpr rc_counter() : counter(1){};
  rc_counter(const rc_counter &) = delete;
  rc_counter &operator=(const rc_counter &) = delete;
  inline constexpr bool zero() { return counter == 0; };
  inline constexpr void operator++() { counter++; };
  inline constexpr void operator--() { counter--; };
  inline constexpr void operator++(int) { counter++; };
  inline constexpr void operator--(int) { counter--; };
  size_t counter;
};

} // namespace __internal__

template <class T> class rc {
public:
  rc(rc<T> &other) {
    ptr = other.ptr;
    counter = other.counter;
    (*counter)++;
  };

  ~rc() {
    (*counter)--;
    if (counter->zero()) {
      delete ptr;
      delete counter;
    }
  };

  T *get() { return ptr; }
  T &operator*() { return *ptr; }
  T *operator->() { return ptr; }

  size_t count() { return counter->counter; }

  explicit rc(T *ptr) : counter(new __internal__::rc_counter()), ptr(ptr){};

private:
  __internal__::rc_counter *counter;
  T *ptr;
};

template <class K, class... Args> rc<K> make_rc(Args... args) {
  return rc<K>(new K(args...));
};

} // namespace util

#endif // INCLUDES_UTIL_RC_H
