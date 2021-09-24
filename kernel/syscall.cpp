#include <kernel/fs/fs.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

// ssize_t syscall_read(int fildes, void *buf, size_t nbyte, off_t offset);
// ssize_t syscall_write(int fildes, const void *buf, size_t n, off_t offset);
// int syscall_close(int fildes);
// int syscall_open(const char *path, int oflag);
//

#define A(ty) va_arg(args, ty)

#define SYSCALL(id, func, args...)                                             \
  case id:                                                                     \
    func(args);                                                                \
    break

/**
 * Handle syscalls (called from assembly isr)
 */
extern "C" void syscall_handler(uint32_t syscall_num, ...) {
  va_list args;
  va_start(args, syscall_num);

  switch (syscall_num) {
    SYSCALL(0, fs::syscall_open, A(const char *), A(int));
    SYSCALL(1, fs::syscall_close, A(int));
    SYSCALL(2, fs::syscall_read, A(int), A(void *), A(size_t), A(off_t));
    SYSCALL(3, fs::syscall_write, A(int), A(const void *), A(size_t), A(off_t));
  default:
    printf("Invalid syscall");
  }

  va_end(args);
}
