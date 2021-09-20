#include <string.h>
#pragma GCC diagnostic ignored "-Wpointer-arith"

/**
 * Convert a int to a string
 */
int itoa(int value, char *out, int base) {
  char buffer[16];
  char *tmp = buffer;
  unsigned int v = value;
  int i;
  int len = 0;

  // FIXME: make this code make sense
  if (value == 0) {
    out[0] = '0';
    out[1] = 0;
    return 1;
  }

  int sign = base == 10 && value < 0;
  if (sign) {
    v = (unsigned int)(-value);
  } else {
    v = (unsigned int)value;
  }

  while (v) {
    i = v % base;
    v /= base;
    *tmp++ = i + (i < 10 ? '0' : 'a');
    len++;
  }

  if (sign) {
    *tmp++ = '-';
    len++;
  }

  while (tmp > buffer)
    *out++ = *--tmp;

  *out++ = 0;

  return len;
}

void memcpy(void *dest, const void *src, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    ((char *)dest)[i] = ((char *)src)[i];
  }
}

void *memset(void *str, int c, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    ((char *)str)[i] = c;
  }
  return str;
}

// TODO: implement this properly
int strcmp(const char *str1, const char *str2) {
  if (str1 == str2)
    return true;
  auto len1 = strlen(str1);
  if (len1 != strlen(str2))
    return false;
  for (size_t i = 0; i < len1; i++) {
    if (str1[i] != str2[i])
      return false;
  }
  return true;
}

// https://opensource.apple.com/source/tcl/tcl-5/tcl/compat/memcmp.c
int memcmp(const void *s1, const void *s2, size_t n) {
  unsigned char u1, u2;
  for (; n--; s1++, s2++) {
    u1 = *(unsigned char *)s1;
    u2 = *(unsigned char *)s2;
    if (u1 != u2) {
      return (u1 - u2);
    }
  }
  return 0;
}

size_t strlen(const char *str) {
  size_t len;
  for (len = 0; str[len]; len++) {
  }
  return len;
}
