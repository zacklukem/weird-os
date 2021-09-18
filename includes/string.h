#ifndef INCLUDES_STRING
#define INCLUDES_STRING

#include <stdint.h>

#define KERNEL

int itoa(int value, char *out, int base);

void memcpy(void *dest, const void *src, size_t n);

void *memset(void *str, int c, size_t n);

int strcmp(const char *str1, const char *str2);

int memcmp(const void *s1, const void *s2, size_t n);

size_t strlen(const char *str);

#endif // INCLUDES_STRING
