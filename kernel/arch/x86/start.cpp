#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
#include <arch/x86/irq.h>
#include <assert.h>
#include <kernel/kb.h>
#include <kernel/kmalloc.h>
#include <kernel/page.h>
#include <kernel/printk.h>
#include <kernel/serial.h>
#include <kernel/timer.h>
#include <stdio.h>

#ifdef TEST_RUN_MODE
extern "C" void __run_kernel_tests__();
#endif

/**
 * Kernel main function.  Called by entry.asm
 */
extern "C" void __kernel_main__() {
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

  initialise_paging();
  printk("paging initialized\n");

  install_timer();

  // Enable interupts to start using the keyboard

  __asm__("sti");

#ifdef TEST_RUN_MODE
  __run_kernel_tests__();
  heap_info();
#endif

  while (1)
    __asm__("hlt");
}
