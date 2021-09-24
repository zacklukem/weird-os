#ifndef INCLUDES_KERNEL_PROCESS_H
#define INCLUDES_KERNEL_PROCESS_H

#include <kernel/fs/fs.h>

namespace process {

struct process_t {
  fs::fdtable fdtable;
};

extern process_t kernel_process;
extern process_t *current_running_process;

void exec_process(rc<fs::dirent> dirent);

inline process_t *current() { return current_running_process; }

} // namespace process

// TODO: Multiprocessing

#endif // INCLUDES_KERNEL_PROCESS_H
