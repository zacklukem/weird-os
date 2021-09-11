#include <kernel/kstl.h>
#include <kernel/shell.h>
#include <stdio.h>
#include <string.h>

void start_shell() {
  while (1) {
    printf("$ ");
    char buf[100];
    int i = 0;
    char c;
    while ((c = getchar()) != '\n') {
      putchar(c);
      buf[i++] = c;
    }
    printf("\n%s\n", buf);
    memset(buf, 0, 100);
    // if (strcmp(buf, "yaykernel") == 0) {
    //   printk("naylenrek\n");
    // }
  }
}