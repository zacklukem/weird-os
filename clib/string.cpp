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
  int len;

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
  while (*str1 != 0 && *str2 != 0) {
    if (*str1++ != *str2++)
      return -1;
  }
  return 0;
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