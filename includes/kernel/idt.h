#ifndef KERNEL_IDT_H
#define KERNEL_IDT_H

#include <stdint.h>

void init_idt();

void idt_set_gate(uint32_t offset, uint16_t selector, uint8_t type,
                  uint8_t index);

#endif
