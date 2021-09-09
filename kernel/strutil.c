#include <kernel/kstl.h>

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
    *tmp++ = i + '0';
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
