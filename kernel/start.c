#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/kb.h>
#include <kernel/kmalloc.h>
#include <kernel/page.h>
#include <kernel/printk.h>
#include <kernel/shell.h>
#include <kernel/timer.h>
#include <kernel/vector.h>
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

  struct vector vec = vector_create(uint32_t);
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  size_t a = kmalloc(8);
  // size_t b =
  kmalloc(8);

  free((void *)a);
  heap_info();
  vector_push(uint32_t, &vec, 1234);
  vector_push(uint32_t, &vec, 1234);
  heap_info();

  // char *a = (void *)kmalloc(8);
  // char *b = (void *)kmalloc_a(8);
  // char *c = (void *)kmalloc(8);
  // a[2] = 3;
  // heap_info();
  // free(a);
  // free(b);
  // free(c);
  // heap_info();

  // int *ptr = (int *)0xA0000000;
  // int do_page_fault = *ptr;
  // printf("%d", do_page_fault);

  // start_shell();

  while (1)
    __asm__("hlt");
}
