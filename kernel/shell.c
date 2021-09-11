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
      if (c == '\b') {
        putchar('\r');
        for (int j = 0; j < 80; ++j) {
          putchar(' ');
        }
        buf[--i] = 0;
        printf("\r$ %s", buf);
      } else {
        putchar(c);
        buf[i++] = c;
      }
    }
    printf("\n%s\n", buf);
    memset(buf, 0, 100);
    // if (strcmp(buf, "yaykernel") == 0) {
    //   printk("naylenrek\n");
    // }
  }
}