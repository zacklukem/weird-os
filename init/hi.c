#include <unistd.h>

const char *hi = "hello, user!";
int a = 0;

void main() {
  // %eax for syscall_number. %ebx, %ecx, %edx, %esi, %edi, %ebp
  a = 2;
  __asm__ __volatile__("movl $0, %%esi   \n" // offset
                       "movl $12, %%edx  \n" // len
                       "movl %0, %%ecx  \n"  // ptr
                       "movl $0, %%ebx   \n" // fildes
                       "movl $3, %%eax   \n" // syscall write
                       "int $0x80      \n" ::"r"(hi));
  while (1)
    ;
}
