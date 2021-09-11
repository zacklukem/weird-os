#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/kb.h>
#include <kernel/kstl.h>
#include <kernel/shell.h>
#include <kernel/timer.h>
#include <stdio.h>

/**
 * Kernel main function.  Called by entry.asm
 */
void __kernel_main__() {
  k_init_stdout();

  cleark();

  printk("kernel start\n");

  gdt_install();
  printk("gdt installed\n");

  init_idt();
  printk("idt installed\n");

  irq_install();
  printk("irq installed\n");

  install_keyboard();
  printk("keyboard installed\n");

  // Enable interupts to start using the keyboard
  __asm__("sti");

  start_shell();

  while (1)
    __asm__("hlt");
}
