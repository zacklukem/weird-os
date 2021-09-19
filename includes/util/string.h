#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include <kernel/kmalloc.h>
#include <string.h>
#include <util/list.h>
#include <util/rc.h>

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
  string() {
    length = 0;
    data = (char *)kmalloc(5);
  }

  string &operator=(string &other) {
    length = other.length;
    data = (char *)kmalloc(length + 1);
    memcpy(data, other.data, length);
    data[length] = 0;
    return *this;
  }

  string &operator=(const string &other) {
    length = other.length;
    data = (char *)kmalloc(length + 1);
    memcpy(data, other.data, length);
    data[length] = 0;
    return *this;
  }

  string(string &other) : data(other.data), length(other.length) {}

  string(const string &other) : data(other.data), length(other.length) {}

  string(const char *cstr)
      : data((char *)kmalloc(__internal__::__strlen_cexpr__(cstr) + 1)),
        length(__internal__::__strlen_cexpr__(cstr)) {
    memcpy(data, cstr, length);
    data[length] = 0;
  }

  string(size_t len) : data((char *)kmalloc(len + 1)), length(len) {
    data[0] = 0;
    data[len] = 0;
  }

  ~string() { free((void *)data); }

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

  list<string> split(char sep) const {
    int count = 1;

    for (size_t i = 0; i < length; i++) {
      if (data[i] == sep) {
        count++;
      }
    }

    list<string> out;

    size_t sec_size = 0;
    size_t sec_start = 0;
    for (size_t i = 0; i < length; i++) {
      if (data[i] == sep) {
        auto s = this->substr(sec_start, sec_size);
        out.push_back(s);
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
  char *data = nullptr;
  size_t length;
};

} // namespace util

#endif
