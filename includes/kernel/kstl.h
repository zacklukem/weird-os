#ifndef INCLUDES_KERNEL_KSTL
#define INCLUDES_KERNEL_KSTL

#include <stdint.h>

// Screen utilities
void k_init_stdout();
void printk(const char *text);
void cleark();
uint16_t get_cursor();
void set_cursor(uint16_t offset);

// String utils

// Port utils
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

struct regs {
  unsigned int gs, fs, es, ds;
  unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
  unsigned int int_no, err_code;
  unsigned int eip, cs, eflags, useresp, ss;
};

#endif // INCLUDES_KERNEL_KSTL
