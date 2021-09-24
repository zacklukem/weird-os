#include <kernel/page.h>
#include <kernel/process.h>

using namespace process;

process_t process::kernel_process;
process_t *process::current_running_process;

void process::exec_process(rc<fs::dirent> dirent) {
  auto ino = dirent->m_inode.lock();

  alloc_frame(get_page(0, 1, kernel_directory), 0, 1);

  switch_page_directory(kernel_directory);

  ino->read((void *)0, ino->size, 0);

  __asm__ __volatile__("call 0");
}
