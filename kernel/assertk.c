#include <kernel/kstl.h>

void __assert(const char *msg, const char *file, int line) {
  // Assertion failed: p != NULL, file main.c, line 5
  printk("Assertion failed: ");
  printk(msg);
  printk(", file ");
  printk(file);
  printk(", line ");
  char buf[5];
  itoa(line, buf, 10);
  printk(buf);
  printk("\n");
  while (1)
    ;
}
