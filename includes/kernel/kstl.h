#ifndef KERNEL_KSTL_H
#define KERNEL_KSTL_H

#define assert(expr)                                                           \
  if (!(expr))                                                                 \
  __assert(#expr, __FILE__, __LINE__)

void printk(const char *text);
void __assert(const char *msg, const char *file, int line);
int itoa(int value, char *out, int base);

#endif
