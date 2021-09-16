#ifndef INCLUDES_UTIL_OPTIONAL_H
#define INCLUDES_UTIL_OPTIONAL_H

namespace util {
template <class T> class optional;

struct nullopt_t {};

inline constexpr nullopt_t nullopt = nullopt_t{};

template <class T> class optional {
public:
  constexpr optional(const T &value) : data(value), _has_value(true){};
  constexpr optional(nullopt_t) : _has_value(false){};
  constexpr optional &operator=(nullopt_t) {
    this->_has_value = false;
    return *this;
  };

  constexpr optional &operator=(T &value) {
    this->_has_value = true;
    this->data = value;
    return *this;
  };

  constexpr optional &operator=(optional<T> &other) {
    this->_has_value = other._has_value;
    this->data = other.value;
    return *this;
  };

  constexpr bool has_value() { return this->_has_value; };

  constexpr T &value() {
    assert(this->_has_value && "Illegal optional access");
    return data;
  };

  constexpr T &value_or(T &def) const { return has_value() ? data : def; }

private:
  T data;
  bool _has_value;
};
} // namespace util

#endif // INCLUDES_UTIL_OPTIONAL_H
