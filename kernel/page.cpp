#include <assert.h>
#include <kernel/fs/initrd.h>
#include <kernel/kernel_info.h>
#include <kernel/kmalloc.h>
#include <kernel/page.h>
#include <kernel/printk.h>
#include <kernel/process.h>
#include <string.h>

// Bit set of frames and their status as free or not
uint32_t *frames;
uint32_t nframes;

page_directory *kernel_directory;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr) {
  uint32_t frame = frame_addr / 0x1000;
  uint32_t idx = INDEX_FROM_BIT(frame);
  uint32_t off = OFFSET_FROM_BIT(frame);
  frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr) {
  uint32_t frame = frame_addr / 0x1000;
  uint32_t idx = INDEX_FROM_BIT(frame);
  uint32_t off = OFFSET_FROM_BIT(frame);
  frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
// static uint32_t test_frame(uint32_t frame_addr) {
//   uint32_t frame = frame_addr / 0x1000;
//   uint32_t idx = INDEX_FROM_BIT(frame);
//   uint32_t off = OFFSET_FROM_BIT(frame);
//   return (frames[idx] & (0x1 << off));
// }

// Static function to find the first free frame.
static uint32_t first_frame() {
  for (uint32_t i = 0; i < INDEX_FROM_BIT(nframes); i++) {
    if (frames[i] != 0xFFFFFFFF) {
      // at least one bit is free here.
      for (uint32_t j = 0; j < 32; j++) {
        uint32_t toTest = 0x1 << j;
        if (!(frames[i] & toTest)) {
          return i * 32 + j;
        }
      }
    }
  }
  return 0;
}

// Function to allocate a frame.
void alloc_frame(page *page, int is_kernel, int is_writeable) {
  if (page->frame != 0) {
    return; // Frame was already allocated, return straight away.
  }
  uint32_t idx = first_frame(); // idx is now the index of the first free frame.
  assert(idx != (uint32_t)-1 && "No free frames!");
  set_frame(idx * 0x1000);           // this frame is now ours!
  page->present = 1;                 // Mark it as present.
  page->rw = (is_writeable) ? 1 : 0; // Should the page be writeable?
  page->user = (is_kernel) ? 0 : 1;  // Should the page be user-mode?
  page->frame = idx;
}

// Function to deallocate a frame.
void free_frame(page *page) {
  uint32_t frame;
  if (!(frame = page->frame)) {
    return; // The given page didn't actually have an allocated frame!
  } else {
    clear_frame(frame); // Frame is now free again.
    page->frame = 0x0;  // Page now doesn't have a frame.
  }
}

#define KHEAP_START 0xd0000000
// 4MB for kheap
#define KHEAP_SIZE 0x400000

page_directory test_dir __attribute__((aligned(0x1000)));

#define KERNEL_PAGE_NUMBER (KERNEL_HIGHER_HALF >> 22)
#define KHEAP_PAGE_NUMBER (KHEAP_START >> 22)

page_directory *fork_page() {
  uint32_t phys;

  auto dir = (page_directory *)kmalloc_ap(sizeof(page_directory), &phys);

  memset(dir, 0, sizeof(page_directory));

  // setup 4MB for now
  // 4MB = 1024 pages
  dir->tables_physical[KERNEL_PAGE_NUMBER] = 0x83;
  dir->tables_physical[KHEAP_PAGE_NUMBER] = 0x400083;

  dir->physical_addr = phys;

  return dir;
}

/**
 * Sets up the environment, page directories etc and
 * enables paging.
 */
void initialise_paging() {
  _internal_placement_address = 0xc0200000;

  // The size of physical memory. For the moment we
  // assume it is 64MB big.
  uint32_t mem_end_page = 0x4000000;

  // Setup our frame bitset
  // TODO: get memory data from multiboot header
  nframes = mem_end_page / 0x1000;
  frames = (uint32_t *)kmalloc(INDEX_FROM_BIT(nframes));
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  // Setup kernel directory

  kernel_directory = fork_page();

  // Lock low 2048 pages
  for (uint32_t i = 0; i < INDEX_FROM_BIT(0x800); i++) {
    frames[i] = 0xffffffff;
  }

  switch_page_directory(kernel_directory);

  init_kernal_heap(KHEAP_SIZE, (void *)KHEAP_START);

  process::init_kernel_process(kernel_directory);
}

extern uint32_t boot_page_directory;

/**
 * Causes the specified page directory to be loaded into the
 * CR3 register.
 */
void switch_page_directory(page_directory *dir) {
  __asm__ __volatile__("mov %0, %%cr3" ::"r"(dir->physical_addr));
}

uint32_t get_phys(uint32_t address, page_directory *dir) {
  auto table_idx = (address / 0x1000) / 0x400;
  uint32_t is_4mb =
      dir->tables_physical[table_idx] & 0x00000080; // bit 7 is 4mb
  if (is_4mb) {
    auto offset = address % 0x400000;
    return (dir->tables_physical[table_idx] & 0xfffff000) + offset;
  }
  assert(false && "todo: 4kb page phys addres");
  return 0;
}

/**
 * get the page entry describing this address's page
 */
page *get_page(uint32_t address, int make, page_directory *dir) {
  // Turn the address into an index.
  address /= 0x1000;
  // Find the page table containing this address.
  uint32_t table_idx = address / 1024;
  // If this table is already assigned
  if (dir->tables[table_idx]) {
    return &dir->tables[table_idx]->pages[address % 1024];
  } else if (make) {
    uint32_t addr;
    dir->tables[table_idx] =
        (page_table *)kmalloc_ap(sizeof(page_table), &addr);
    assert(!(addr & 0xfff) && "invalid page aligned malloc");
    memset(dir->tables[table_idx], 0, 0x1000);
    dir->tables_physical[table_idx] = addr | 0x7; // PRESENT, RW, US.
    return &dir->tables[table_idx]->pages[address % 1024];
  } else {
    return 0;
  }
}

page_allocator::page_allocator() : directory(nullptr){};

page_allocator::page_allocator(page_directory *pd) : directory(pd) {}
int page_allocator::allocate(uint32_t virt_addr, uint32_t size, int is_kernel,
                             int is_writable) {
  virt_addr &= 0xfffff000;
  if (size & 0xfff)
    size = (size & 0xfffff000) + 0x1000;

  for (uint32_t i = virt_addr; i < virt_addr + size; i += 0x1000) {
    alloc_frame(get_page(i, 1, directory), is_kernel, is_writable);
  }

  return 0;
}
