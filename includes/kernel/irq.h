#ifndef INCLUDES_KERNEL_IRQ
#define INCLUDES_KERNEL_IRQ

#include <kernel/regs.h>
#include <stdint.h>

void irq_install();
void install_irq_routine(size_t port, void (*handler)(struct regs *r));

#endif // INCLUDES_KERNEL_IRQ
