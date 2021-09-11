#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/port_io.h>

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/**
 * the list of function pointers to irq routines
 */
static void *irq_routines[16] = {0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * Set an IRQ routine to a particular function
 */
void install_irq_routine(size_t port, void (*handler)(struct regs *r)) {
  irq_routines[port] = (void *)handler;
}

/**
 * Remap the real mode irq's to not cause double fault
 */
static void irq_remap(void) {
  port_byte_out(0x20, 0x11);
  port_byte_out(0xA0, 0x11);
  port_byte_out(0x21, 0x20);
  port_byte_out(0xA1, 0x28);
  port_byte_out(0x21, 0x04);
  port_byte_out(0xA1, 0x02);
  port_byte_out(0x21, 0x01);
  port_byte_out(0xA1, 0x01);
  port_byte_out(0x21, 0x0);
  port_byte_out(0xA1, 0x0);
}

/**
 * Initialize the IRQ by adding them tp the IDT
 */
void irq_install() {
  irq_remap();

  idt_set_gate((uint32_t)irq0, 0x08, 0x8e, 32 + 0);
  idt_set_gate((uint32_t)irq1, 0x08, 0x8e, 32 + 1);
  idt_set_gate((uint32_t)irq2, 0x08, 0x8e, 32 + 2);
  idt_set_gate((uint32_t)irq3, 0x08, 0x8e, 32 + 3);
  idt_set_gate((uint32_t)irq4, 0x08, 0x8e, 32 + 4);
  idt_set_gate((uint32_t)irq5, 0x08, 0x8e, 32 + 5);
  idt_set_gate((uint32_t)irq6, 0x08, 0x8e, 32 + 6);
  idt_set_gate((uint32_t)irq7, 0x08, 0x8e, 32 + 7);
  idt_set_gate((uint32_t)irq8, 0x08, 0x8e, 32 + 8);
  idt_set_gate((uint32_t)irq9, 0x08, 0x8e, 32 + 9);
  idt_set_gate((uint32_t)irq10, 0x08, 0x8e, 32 + 10);
  idt_set_gate((uint32_t)irq11, 0x08, 0x8e, 32 + 11);
  idt_set_gate((uint32_t)irq12, 0x08, 0x8e, 32 + 12);
  idt_set_gate((uint32_t)irq13, 0x08, 0x8e, 32 + 13);
  idt_set_gate((uint32_t)irq14, 0x08, 0x8e, 32 + 14);
  idt_set_gate((uint32_t)irq15, 0x08, 0x8e, 32 + 15);
}

/**
 * Handle IRQ's
 */
void irq_handler(struct regs *r) {
  void (*handler)(struct regs * r);

  // See if handler has been set and if so, call it
  handler = irq_routines[r->int_no - 32];
  if (handler) {
    handler(r);
  }

  // See if we need to forward IRQ to the slave controller
  if (r->int_no >= 40) {
    port_byte_out(0xA0, 0x20);
  }

  // Send to mater controller
  port_byte_out(0x20, 0x20);
}