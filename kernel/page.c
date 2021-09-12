#include <assert.h>
#include <kernel/kmalloc.h>
#include <kernel/page.h>
#include <string.h>

// Bit set of frames and their status as free or not
uint32_t *frames;
uint32_t nframes;

struct page_directory *kernel_directory;
struct page_directory *current_directory;

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
          return i * 4 * 8 + j;
        }
      }
    }
  }
  return 0;
}

// Function to allocate a frame.
void alloc_frame(struct page *page, int is_kernel, int is_writeable) {
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
void free_frame(struct page *page) {
  uint32_t frame;
  if (!(frame = page->frame)) {
    return; // The given page didn't actually have an allocated frame!
  } else {
    clear_frame(frame); // Frame is now free again.
    page->frame = 0x0;  // Page now doesn't have a frame.
  }
}

#define KHEAP_START 0xc0000000
#define KHEAP_SIZE 0x10000

/**
 * Sets up the environment, page directories etc and
 * enables paging.
 */
void initialise_paging() {
  // The size of physical memory. For the moment we
  // assume it is 16MB big.
  uint32_t mem_end_page = 0x1000000;

  nframes = mem_end_page / 0x1000;
  frames = (uint32_t *)kmalloc(INDEX_FROM_BIT(nframes));
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  // Let's make a page directory.
  kernel_directory =
      (struct page_directory *)kmalloc_a(sizeof(struct page_directory));
  memset(kernel_directory, 0, sizeof(struct page_directory));
  current_directory = kernel_directory;

  // We need to identity map (phys addr = virt addr) from
  // 0x0 to the end of used memory, so we can access this
  // transparently, as if paging wasn't enabled.
  // NOTE that we use a while loop here deliberately.
  // inside the loop body we actually change placement_address
  // by calling kmalloc(). A while loop causes this to be
  // computed on-the-fly rather than once at the start.
  int i = 0;
  while (i < _internal_placement_address) {
    // Kernel code is readable but not writeable from userspace.
    alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
    i += 0x1000;
  }

  // Now allocate those pages we mapped earlier.
  for (i = KHEAP_START; i < KHEAP_START + KHEAP_SIZE; i += 0x1000)
    alloc_frame(get_page(i, 1, kernel_directory), 0, 0);

  // Before we enable paging, we must register our page fault handler.
  // register_interrupt_handler(14, page_fault);

  // Now, enable paging!
  switch_page_directory(kernel_directory);

  init_kernal_heap(KHEAP_SIZE, (void *)KHEAP_START);
}

/**
 * Causes the specified page directory to be loaded into the
 * CR3 register.
 */
void switch_page_directory(struct page_directory *dir) {
  current_directory = dir;
  __asm__ __volatile__("mov %0, %%cr3" ::"r"(&dir->tables_physical));
  uint32_t cr0;
  __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  __asm__ __volatile__("mov %0, %%cr0" ::"r"(cr0));
}

struct page *get_page(uint32_t address, int make, struct page_directory *dir) {
  // Turn the address into an index.
  address /= 0x1000;
  // Find the page table containing this address.
  uint32_t table_idx = address / 1024;
  if (dir->tables[table_idx]) // If this table is already assigned
  {
    return &dir->tables[table_idx]->pages[address % 1024];
  } else if (make) {
    uint32_t tmp;
    dir->tables[table_idx] =
        (struct page_table *)kmalloc_ap(sizeof(struct page_table), &tmp);
    memset(dir->tables[table_idx], 0, 0x1000);
    dir->tables_physical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
    return &dir->tables[table_idx]->pages[address % 1024];
  } else {
    return 0;
  }
}

/**
 * Handler for page faults.
 */
void page_fault(struct regs *regs) {}