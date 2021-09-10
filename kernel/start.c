#include <kernel/idt.h>
#include <kernel/kstl.h>

void gdt_install();

void __kernel_main__() {
  cleark();
  printk("kernel start\n");
  gdt_install();
  printk("gdt installed\n");
  init_idt();
  printk("idt installed\n");

  __asm__("sti" ::);
  __asm__("int $0x80" ::);
#pragma GCC diagnostic ignored "-Wdiv-by-zero"
  // int a = 23 / 0;
  while (1)
    ;
}
