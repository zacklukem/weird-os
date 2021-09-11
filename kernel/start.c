#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/kb.h>
#include <kernel/kmalloc.h>
#include <kernel/page.h>
#include <kernel/printk.h>
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

  initialise_paging();
  printk("paging initialized\n");

  // Enable interupts to start using the keyboard
  __asm__("sti");

  cleark();

  struct heap kheap = create_heap(0x2000, (void *)0xC0000000);
  void *a = alloc_internal(8, 0, &kheap);
  void *b = alloc_internal(8, 0, &kheap);
  void *c = alloc_internal(8, 0, &kheap);
  free(a);
  free(b);
  free(c);
  heap_info(&kheap);

  // int *ptr = (int *)0xA0000000;
  // int do_page_fault = *ptr;
  // printf("%d", do_page_fault);

  // start_shell();

  while (1)
    __asm__("hlt");
}
