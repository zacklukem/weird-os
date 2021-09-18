#ifndef UTIL_STRING_H
#define UTIL_STRING_H

#include <kernel/kmalloc.h>
#include <string.h>

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
  constexpr string(const char *cstr)
      : data(cstr), length(__internal__::__strlen_cexpr__(cstr)) {}

  constexpr string(char *cstr)
      : data(cstr), length(__internal__::__strlen_cexpr__(cstr)) {}

  inline const char *cstr() const { return data; }

  inline size_t len() const { return length; }

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

private:
  const char *data;
  size_t length;
};

} // namespace util

#endif
