#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include <kernel/kmalloc.h>
#include <string.h>
#include <util/rc.h>
#include <util/vector.h>

namespace util {
namespace __internal__ {
constexpr size_t __strlen_cexpr__(const char *str) {
  size_t len = 0;
  for (; str[len]; len++)
    ;
  return len;
}
} // namespace __internal__

class string {
public:
  string &operator=(string &other) {
    // TODO: decrement other counter?
    counter = other.counter;
    counter->count++;
    length = other.length;
    data = other.data;
    return *this;
  }

  string &operator=(const string &other) {
    counter = other.counter;
    counter->count++;
    length = other.length;
    data = other.data;
    return *this;
  }

  string(string &other)
      : counter(other.counter), data(other.data), length(other.length) {
    counter->count++;
  }

  string(const string &other)
      : counter(other.counter), data(other.data), length(other.length) {
    counter->count++;
  }

  string(const char *cstr)
      : counter(new __internal__::rc_counter()),
        data((char *)kmalloc(__internal__::__strlen_cexpr__(cstr) + 1)),
        length(__internal__::__strlen_cexpr__(cstr)) {
    memcpy(data, cstr, length);
    data[length] = 0;
  }

  string(size_t len)
      : counter(new __internal__::rc_counter()), data((char *)kmalloc(len + 1)),
        length(len) {
    data[0] = 0;
    data[len] = 0;
  }

  ~string() {
    counter->count--;
    if (counter->count == 0) {
      free((void *)data);
      delete counter;
    }
  }

  inline const char *cstr() const { return data; }

  inline size_t len() const { return length; }

  string operator+(const char *other) const {
    auto olen = strlen(other);
    string out(length + olen);
    memcpy(out.data, data, length);
    memcpy(&out.data[length], other, olen);
    out.data[out.length] = 0;
    return out;
  }

  string operator+(const string &other) const {
    string out(length + other.length);
    memcpy(out.data, data, length);
    memcpy(&out.data[length], other.data, other.length);
    out.data[out.length] = 0;
    return out;
  }

  string &operator+=(const char *other) {
    auto olen = strlen(other);
    data = (char *)krealloc((size_t)data, length + olen + 1);
    memcpy(&data[length], other, olen);
    length += olen;
    data[length] = 0;
    return *this;
  }

  string &operator+=(const string &other) {
    data = (char *)krealloc((size_t)data, length + other.length + 1);
    memcpy(&data[length], other.data, other.length);
    length += other.length;
    data[length] = 0;
    return *this;
  }

  string substr(size_t start, size_t len) const {
    string out(len);
    memcpy(out.data, &data[start], len);
    out.data[len] = 0;
    return out;
  };

  string slice(size_t start, size_t end) const {
    string out(end - start);
    memcpy(out.data, &data[start], end - start);
    out.data[end - start] = 0;
    return out;
  };

  // TODO: leaks memory like crazy
  vec<string> split(char sep) const {
    int count = 1;

    for (size_t i = 0; i < length; i++) {
      if (data[i] == sep) {
        count++;
      }
    }

    vec<string> out(count);

    size_t sec_size = 0;
    size_t sec_start = 0;
    for (size_t i = 0; i < length; i++) {
      if (data[i] == sep) {
        out.push_back(this->substr(sec_start, sec_size));
        sec_size = 0;
        sec_start = i + 1;
      } else {
        sec_size++;
      }
    }

    out.push_back(this->substr(sec_start, sec_size));

    return out;
  }

  bool operator==(const string &other) const {
    if (data == other.data)
      return true;
    if (length != other.length)
      return false;
    for (size_t i = 0; i < length; i++) {
      if (data[i] != other.data[i])
        return false;
    }
    return true;
  }

  bool operator==(const char *other) const {
    if (data == other)
      return true;
    if (length != strlen(other))
      return false;
    for (size_t i = 0; i < length; i++) {
      if (data[i] != other[i])
        return false;
    }
    return true;
  }

  inline bool operator!=(const string &other) const {
    return !(*this == other);
  }

  inline bool operator!=(const char *other) const { return !(*this == other); }

private:
  __internal__::rc_counter *counter;
  char *data = nullptr;
  size_t length;
};

} // namespace util

#endif
