#include <kernel/elf.h>
#include <kernel/page.h>
#include <kernel/process.h>

using namespace process;

process_t::process_t(page_directory *directory)
    : directory(directory), allocator(directory) {}

process_t *process::kernel_process;
process_t *process::current_running_process;

void process::init_kernel_process(page_directory *directory) {
  kernel_process = new process_t(directory);
  current_running_process = kernel_process;
}

void process::exec_process(rc<fs::dirent> dirent) {
  auto ino = dirent->m_inode.lock();

  // alloc_frame(get_page(0, 1, kernel_directory), 0, 1);

  // switch_page_directory(kernel_directory);

  auto data = (void *)kmalloc(ino->size);

  ino->read(data, ino->size, 0);

  elf::elf_loader loader(data);

  auto layout = loader.load(&current_allocator);

  free(data);

  // __asm__ __volatile__("mov %%esp, %0" : "=r"(current()->stack));

  __asm__ __volatile__("mov %0, %%esp" ::"r"(layout.stack));
  __asm__ __volatile__("call *%0" ::"r"(layout.entry));
}

void syscall_exit(int retval) {
  // For now go to kernel code
}
