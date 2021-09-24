#ifndef INCLUDES_KERNEL_PRINTK_H
#define INCLUDES_KERNEL_PRINTK_H

#include <stdint.h>

// Screen utilities
void k_init_printk();
void printk(const char *text);
void cleark();

#endif // INCLUDES_KERNEL_PRINTK_H
