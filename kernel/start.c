#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/kb.h>
#include <kernel/kstl.h>
#include <kernel/timer.h>

void gdt_install();

void __kernel_main__() {
  set_cursor(0);
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

  __asm__("sti" ::);

#pragma GCC diagnostic ignored "-Wdiv-by-zero"
  while (1)
    ;
}
