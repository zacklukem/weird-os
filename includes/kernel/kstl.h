#ifndef KERNEL_KSTL_H
#define KERNEL_KSTL_H

#define assert(expr)                                                           \
  if (!(expr))                                                                 \
  __assert(#expr, __FILE__, __LINE__)

void printk(const char *text);
void cleark();
void __assert(const char *msg, const char *file, int line);
int itoa(int value, char *out, int base);

static inline unsigned char port_byte_in(unsigned short port) {
  unsigned char result;
  __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
  return result;
}

static inline void port_byte_out(unsigned short port, unsigned char data) {
  __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

static inline unsigned short port_word_in(unsigned short port) {
  unsigned short result;
  __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
  return result;
}

static inline void port_word_out(unsigned short port, unsigned short data) {
  __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

#endif
