#ifndef INCLUDES_KERNEL_PORT_IO_H
#define INCLUDES_KERNEL_PORT_IO_H

// Port utils
static inline unsigned char inb(unsigned short port) {
  unsigned char result;
  __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
  return result;
}

static inline void outb(unsigned short port, unsigned char data) {
  __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

static inline unsigned short inw(unsigned short port) {
  unsigned short result;
  __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
  return result;
}

static inline void outw(unsigned short port, unsigned short data) {
  __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

#endif // INCLUDES_KERNEL_PORT_IO_H
