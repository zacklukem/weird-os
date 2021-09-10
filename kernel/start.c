#include <kernel/idt.h>
#include <kernel/kstl.h>

void __kernel_main__() {
  init_idt();
  __asm__("sti" ::);
#pragma GCC diagnostic ignored "-Wdiv-by-zero"
  int a = 23 / 0;
  while (1)
    ;
}
