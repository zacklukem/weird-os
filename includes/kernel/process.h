#ifndef INCLUDES_KERNEL_PROCESS_H
#define INCLUDES_KERNEL_PROCESS_H

#include <kernel/fs.h>

struct process {
  struct fdtable fdtable;
};

extern struct process kernel_process;

// TODO: Multiprocessing
#define current() (&kernel_process)

#endif // INCLUDES_KERNEL_PROCESS_H
