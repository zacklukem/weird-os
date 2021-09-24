#include <kernel/elf.h>
#include <kernel/page.h>
#include <kernel/process.h>

using namespace process;

process_t process::kernel_process;
process_t *process::current_running_process = &process::kernel_process;

void process::exec_process(rc<fs::dirent> dirent) {
  auto ino = dirent->m_inode.lock();

  // alloc_frame(get_page(0, 1, kernel_directory), 0, 1);

  // switch_page_directory(kernel_directory);

  auto data = (void *)kmalloc(ino->size);

  ino->read(data, ino->size, 0);

  elf::elf_loader loader(data);

  loader.load(&current_allocator);

  free(data);

  __asm__ __volatile__("call %0" ::"r"(loader.entry()));
}
