#include <kernel/kstl.h>

void __kernel_main__() {
  assert(0);
  char tmp[16];
  itoa(-123, tmp, 10);
  printk(tmp);
  //  printk("Hello Clang!");
  while (1)
    ;
}
