#ifndef INCLUDES_KERNEL_PROCESS_H
#define INCLUDES_KERNEL_PROCESS_H

#include <kernel/fs/fs.h>
#include <kernel/page.h>
#include <stdint.h>

namespace process {

class process_t;
class thread_t;

struct trap_frame_t {
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
  uint32_t eip; // eip is saved on stack by the caller's "CALL" instruction
  uint32_t return_address;
  uint32_t parameter1, parameter2, parameter3;
};

enum thread_state { READY, WAITING, RUNNING, TERMINATED, NEW };

class thread_t {
public:
  thread_t(process_t *parent);
  thread_t(process_t *parent, uint32_t esp);
  thread_t(thread_t &other) = delete;
  thread_t(thread_t &&other) = delete;

  tid_t id;
  thread_state state;
  // uint32_t eip;
  uint32_t esp;
  // regs r;
  process_t *parent;
  void switch_to();
};

class process_t {
public:
  process_t(page_directory *directory);
  process_t(process_t &other) = delete;
  process_t(process_t &&other) = delete;

public:
  pid_t id;
  fs::fdtable fdtable;
  page_directory *directory;
  page_allocator allocator;
  rc<process_t> parent;
  thread_t *thread;
};

extern process_t *kernel_process;
extern process_t *current_running_process;
extern thread_t *current_running_thread;

void exec_process(rc<fs::dirent> dirent);

void syscall_exit(int retval);

inline process_t *current() { return current_running_process; }

void init_kernel_process(page_directory *directory);

} // namespace process

// TODO: Multiprocessing

#endif // INCLUDES_KERNEL_PROCESS_H
