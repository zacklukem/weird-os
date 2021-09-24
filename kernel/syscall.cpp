#include <arch/x86/regs.h>
#include <kernel/fs/fs.h>
#include <kernel/printk.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

// ssize_t syscall_read(int fildes, void *buf, size_t nbyte, off_t offset);
// ssize_t syscall_write(int fildes, const void *buf, size_t n, off_t offset);
// int syscall_close(int fildes);
// int syscall_open(const char *path, int oflag);
//

#define A(i, ty) ((ty)args[i])

#define SYSCALL(id, func, args...)                                             \
  case id:                                                                     \
    func(args);                                                                \
    break

// %eax for syscall_number. %ebx, %ecx, %edx, %esi, %edi, %ebp

/**
 * Handle syscalls (called from assembly isr)
 */
extern "C" void syscall_handler(struct regs *r) {
  uint32_t syscall_num = r->eax;
  uint32_t args[] = {r->ebx, r->ecx, r->edx, r->esi, r->edi, r->ebp};

  switch (syscall_num) {
    SYSCALL(0, fs::syscall_open, A(0, const char *), A(1, int));
    SYSCALL(1, fs::syscall_close, A(0, int));
    SYSCALL(2, fs::syscall_read, A(0, int), A(1, void *), A(2, size_t),
            A(3, off_t));
    SYSCALL(3, fs::syscall_write, A(0, int), A(1, const void *), A(2, size_t),
            A(3, off_t));
  default:
    printf("Invalid syscall %d", syscall_num);
  }
}
