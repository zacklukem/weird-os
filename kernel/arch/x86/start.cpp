#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
#include <arch/x86/irq.h>
#include <arch/x86/multiboot.h>
#include <assert.h>
#include <drivers/vga.h>
#include <kernel/fs/fs.h>
#include <kernel/fs/initrd.h>
#include <kernel/kb.h>
#include <kernel/kmalloc.h>
#include <kernel/page.h>
#include <kernel/printk.h>
#include <kernel/process.h>
#include <kernel/serial.h>
#include <kernel/timer.h>
#include <stdio.h>

#ifdef TEST_RUN_MODE
extern "C" void __run_kernel_tests__();
#endif

/**
 * Kernel main function.  Called by entry.asm
 */
extern "C" void __kernel_main__() {
  k_init_printk();

  cleark();

  printk("kernel start\n");

  gdt_install();
  printk("gdt installed\n");

  init_idt();
  printk("idt installed\n");

  irq_install();
  printk("irq installed\n");

  initialise_paging();
  printk("paging initialized\n");

  fs::initrd_location =
      *((uint32_t *)(get_multiboot_header().mods_addr + 0xc0000000)) +
      0xc0000000;
  fs::initrd_end =
      *(uint32_t *)((get_multiboot_header().mods_addr + 0xc0000000) + 4) +
      0xc0000000;

  // install_keyboard();
  // printk("keyboard installed\n");

  install_timer();

  __asm__("sti");

  // *((char *)0xffffffff) = 0;

  fs::init_fs();

  // init drivers

  vga::init_vga();

  fs::syscall_open_at(0, "/dev/vga", O_WRONLY);

  // run proc
  auto hi_de = fs::resolve_path("/hi");

  process::exec_process(hi_de.value());

#ifdef TEST_RUN_MODE
  // __run_kernel_tests__();
  // heap_info();
#endif

  while (1)
    __asm__("hlt");
}
