#ifndef INCLUDES_KERNEL_PRINTK_H
#define INCLUDES_KERNEL_PRINTK_H

#include <stdint.h>

// Screen utilities
void k_init_stdout();
void printk(const char *text);
void cleark();
uint16_t get_cursor();
void set_cursor(uint16_t offset);

#endif // INCLUDES_KERNEL_PRINTK_H
