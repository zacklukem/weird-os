#ifndef INCLUDES_KERNEL_PROCESS_H
#define INCLUDES_KERNEL_PROCESS_H

#include <kernel/fs/fs.h>
#include <kernel/page.h>

namespace process {

class process_t {
public:
  process_t(page_directory *directory);
  process_t(process_t &other) = delete;
  process_t(process_t &&other) = delete;

public:
  fs::fdtable fdtable;
  page_directory *directory;
  page_allocator allocator;
  void *stack;
};

extern process_t *kernel_process;
extern process_t *current_running_process;

void exec_process(rc<fs::dirent> dirent);

void syscall_exit(int retval);

inline process_t *current() { return current_running_process; }

void init_kernel_process(page_directory *directory);

} // namespace process

// TODO: Multiprocessing

#endif // INCLUDES_KERNEL_PROCESS_H
