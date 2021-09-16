#ifndef ARCH_X86_IRQ_H
#define ARCH_X86_IRQ_H

#include <arch/x86/regs.h>
#include <stdint.h>

void irq_install();
void install_irq_routine(size_t port, void (*handler)(struct regs *r));

#endif // ARCH_X86_IRQ_H
