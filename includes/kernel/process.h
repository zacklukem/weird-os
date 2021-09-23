#ifndef INCLUDES_KERNEL_PROCESS_H
#define INCLUDES_KERNEL_PROCESS_H

#include <kernel/fs/fs.h>

struct process {
  fs::fdtable fdtable;
};

extern struct process kernel_process;

void exec_process(rc<fs::dirent> dirent);

// TODO: Multiprocessing
#define current() (&kernel_process)

#endif // INCLUDES_KERNEL_PROCESS_H
