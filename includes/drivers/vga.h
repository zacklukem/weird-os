#ifndef INCLUDES_KERNEL_VGA_H
#define INCLUDES_KERNEL_VGA_H

#include <stdint.h>

namespace vga {

void init_vga();
uint16_t get_cursor();
void set_cursor(uint16_t offset);

} // namespace vga

#endif
