#include <kernel/elf.h>
#include <kernel/page.h>
#include <kernel/process.h>

// from process.asm
extern "C" void do_switch(uint32_t *old_stack, uint32_t new_stack,
                          uint32_t page);

extern "C" void enter_usermode();

using namespace process;

static tid_t next_tid = 0;
static pid_t next_pid = 0;

process_t::process_t(page_directory *directory)
    : id(next_pid++), directory(directory), allocator(directory),
      thread(new thread_t(this)) {
  memset(&fdtable, 0, sizeof(fs::fdtable));
}

process_t *process::kernel_process;
process_t *process::current_running_process;
thread_t *process::current_running_thread;

void process::init_kernel_process(page_directory *directory) {
  kernel_process = new process_t(directory);
  current_running_process = kernel_process;
  current_running_thread = kernel_process->thread;
}

void process::exec_process(rc<fs::dirent> dirent) {
  auto ino = dirent->m_inode.lock();

  // alloc_frame(get_page(0, 1, kernel_directory), 0, 1);

  // switch_page_directory(kernel_directory);

  auto data = (void *)kmalloc(ino->size);

  ino->read(data, ino->size, 0);

  elf::elf_loader loader(data);

  // TODO: create new process for exec
  auto layout = loader.load(&current()->allocator);

  free(data);

  // __asm__ __volatile__("mov %%esp, %0" : "=r"(current()->stack));

  // enter_usermode();
  __asm__ __volatile__("mov %0, %%esp" ::"r"(layout.stack));
  __asm__ __volatile__("call *%0" ::"r"(layout.entry));
}

thread_t::thread_t(process_t *parent, uint32_t esp)
    : id(next_tid++), state(thread_state::NEW), parent(parent) {
  // todo: allocate stack position
  this->esp = esp;
  auto frame = (trap_frame_t *)esp;
  memset(frame, 0, sizeof(trap_frame_t));
  frame->eax = 0xeaf;
  frame->ebx = 0xebf;
  frame->ecx = 0xecf;
  frame->edx = 0xedf;
}

thread_t::thread_t(process_t *parent)
    : id(next_tid++), state(thread_state::NEW), parent(parent) {
  // todo: allocate stack position
  esp = kmalloc(0x2000);
  auto frame = (trap_frame_t *)esp;
  memset(frame, 0, sizeof(trap_frame_t));
  frame->eax = 0xeaf;
  frame->ebx = 0xebf;
  frame->ecx = 0xecf;
  frame->edx = 0xedf;
}

uint32_t temp_stack;

void thread_t::switch_to() {
  //  thread_t *old = current()->thread;
  current_running_thread = this;
  current_running_process = this->parent;
  do_switch(&temp_stack, this->esp, this->parent->directory->physical_addr);
}

void syscall_exit(int retval) {
  // For now go to kernel code
}
