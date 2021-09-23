// Expect lots of pointer math here
#pragma GCC diagnostic ignored "-Wpointer-arith"

#include <assert.h>
#include <kernel/kmalloc.h>
#include <kernel/page.h>
#include <kernel/printk.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t _internal_placement_address = 0;

struct heap kheap_s;
struct heap *kheap = 0;

static inline int is_used(uint32_t magic) { return magic & 0x1; }

static inline int is_valid(uint32_t magic) {
  return (magic & 0xfffffffe) == MAGIC;
}

/**
 * Merge two adjacent blocks
 */
static void merge_blocks(struct block_header *first,
                         struct block_header *second) {
  assert(first->next == second && "Headers must be in adjacent");

  first->next = second->next;
  if (first->next) {
    first->next->previous = first;
  }
  first->size += second->size + sizeof(struct block_header);
}

void heap_info() {
  printf("\nBlk Addr | Mem Addr | Stat | Size     | Space Taken\n");
  struct block_header *current;
  for (current = kheap->first_block; current; current = current->next) {
    printf("%x | %x | %s | %x | %x\n", (uint32_t)current,
           (uint32_t)current + sizeof(struct block_header),
           is_used(current->magic) ? "used" : "free", current->size,
           current->size + sizeof(struct block_header));
  }
}

struct heap create_heap(size_t size, void *start) {
  struct heap ret_val;
  ret_val.size = size;
  ret_val.first_block = (struct block_header *)start;
  ret_val.first_block->magic = MAGIC & 0xfffffffe; // Set last bit to 0
  ret_val.first_block->size = size;
  ret_val.first_block->size = size - sizeof(struct block_header);
  ret_val.first_block->next = 0;
  ret_val.first_block->previous = 0;
  return ret_val;
}

void init_kernal_heap(size_t size, void *start) {
  kheap_s = create_heap(size, start);
  kheap = &kheap_s;
}

static inline void *get_memory_address(struct block_header *block) {
  return (void *)((size_t)block) + sizeof(block_header);
}

static inline uint32_t get_distance_to_next_page(uint32_t addr) {
  return 0x1000 - (addr & 0xfff);
}

static inline uint32_t get_next_page(uint32_t addr) {
  return (addr & 0xfffff000) + 0x1000;
}

static struct block_header *make_aligned_block(size_t size,
                                               struct block_header *block) {
  //
  uint32_t mem_address = (uint32_t)get_memory_address(block);
  uint32_t distance =
      get_distance_to_next_page(mem_address); // last 12 bits are the distance
  uint32_t nearest = get_next_page(mem_address);
  uint32_t header_pos = nearest - sizeof(struct block_header);
  // for now just move the block and resize previous block

  block->size -= distance;
  block->previous->next = (block_header *)header_pos;
  block->previous->size += distance;

  // move the header to its new home
  memcpy((void *)header_pos, block, sizeof(struct block_header));

  return (block_header *)header_pos;
}

static struct block_header *get_suitable_block(size_t size, int page_align,
                                               struct heap *heap) {
  struct block_header *best = heap->first_block;
  struct block_header *current = heap->first_block;

  while (is_used(best->magic)) {
    assert(best->next && "No unused blocks");
    best = best->next;
  }

  while (current) {
    // Get smallest unused block with size at least equal to given size
    if (!is_used(current->magic)) {
      uint32_t required_size = size;
      if (page_align) {
        uint32_t mem_address = (uint32_t)get_memory_address(current);
        uint32_t distance =
            mem_address & 0xfff; // last 12 bits are the distance
        required_size += distance;
      }
      // if not page aligned, we only need the smallest unused block big
      // enough for our data
      if (current->size >= required_size &&
          (current->size < best->size || best->size < required_size)) {
        best = current;
      }
    }
    current = current->next;
  }
  if (page_align) {
    uint32_t mem_address = (uint32_t)get_memory_address(best);
    uint32_t distance = mem_address & 0xfff; // last 12 bits are the distance
    assert(best->size + distance >= size &&
           "Out of heap space for aligned malloc");
    return make_aligned_block(size, best);
  } else {
    if (best->size < size) {
      heap_info();
    }
    assert(best->size >= size && "Out of heap space");
    return best;
  }
}

static void create_new_block_header(struct block_header *block, size_t size) {
  // create a new block header
  struct block_header *new_block =
      (struct block_header *)(((size_t)block) + size +
                              sizeof(struct block_header));
  if (block->next) {
    block->next->previous = new_block;
  }
  new_block->next = block->next;
  new_block->previous = block;
  new_block->size = block->size - size - sizeof(struct block_header);
  new_block->magic = MAGIC & 0xfffffffe;

  block->next = new_block;
  block->size = size;
  block->magic |= 0x1;
}

void *alloc_internal(size_t size, int page_align, struct heap *heap) {
  struct block_header *block = get_suitable_block(size, page_align, heap);
  // See if we should split the block
  if (block->size - size > sizeof(struct block_header) + MIN_BLOCK_SIZE) {
    create_new_block_header(block, size);
  }
  block->magic = MAGIC | 0x1;
  // Return pointer to the data not the block header
  return get_memory_address(block);
}

/**
 * Given an address to a block header, merge the next blocks until the next
 * block is used.
 */
void free(void *mem) {
  // Get pointer to the block header relative to the data
  struct block_header *header =
      (struct block_header *)((size_t)mem - sizeof(struct block_header));
  assert(is_valid(header->magic) && "Heap block is invalid!");
  if (!is_used(header->magic))
    return;
  header->magic &= 0xfffffffe;

  // Merge forwards
  while (1) {
    struct block_header *next_header = header->next;
    // Break if this is the end of the heap
    if (next_header == 0)
      break;

    assert(is_valid(next_header->magic) && "Heap block is invalid!");
    // Stop at used header
    if (is_used(next_header->magic))
      break;

    // The next block is valid and unused, so we merge with it
    merge_blocks(header, next_header);
  }

  struct block_header *current = header;

  // Merge backward
  while (1) {
    struct block_header *previous_header = current->previous;
    // Break if this is the start of the heap
    if (previous_header == 0)
      break;

    assert(is_valid(previous_header->magic) && "Heap block is invalid!");
    // Stop at used header
    if (is_used(previous_header->magic))
      break;

    // The next block is valid and unused, so we merge with it
    merge_blocks(previous_header, current);
    current = previous_header;
  }
}

uint32_t krealloc(uint32_t ptr, uint32_t sz) {
  struct block_header *header =
      (struct block_header *)(ptr - sizeof(struct block_header));
  assert(is_valid(header->magic) && "Heap block is invalid!");
  if (header->size >= sz) { // No malloc needed
    return ptr;
  }
  if (!header->next ||                // End of heap space
      is_used(header->next->magic) || // Next header is used
      // Next header is not big enough to merge and realloc
      (header->next->size + header->size + sizeof(struct block_header) < sz)) {
    // We start this block from scratch
    uint32_t new_ptr = kmalloc(sz);
    memcpy((void *)new_ptr, (void *)ptr, header->size);
    free((void *)ptr);
    return new_ptr;
  }
  size_t merge_size =
      header->next->size + header->size + sizeof(struct block_header);
  // Sanity check
  assert(merge_size >= sz && "Realloc error");
  merge_blocks(header, header->next);
  if (merge_size - sz > (MIN_BLOCK_SIZE + sizeof(struct block_header))) {
    create_new_block_header(header, sz);
  }
  return ptr;
}

static uint32_t kmalloc_internal(uint32_t sz, int align, uint32_t *phys) {
  if (kheap != 0) {
    void *addr = alloc_internal(sz, align, kheap);
    if (phys != 0) {
      struct page *page = get_page((uint32_t)addr, 0, kernel_directory);
      *phys = page->frame * 0x1000 + ((uint32_t)addr & 0xfff);
    }
    return (uint32_t)addr;
  } else {
    if (align == 1 && (_internal_placement_address & 0xFFFFF000)) {
      // Align the placement address;
      _internal_placement_address &= 0xFFFFF000;
      _internal_placement_address += 0x1000;
    }
    if (phys) {
      *phys = _internal_placement_address - 0xc0000000;
    }

    uint32_t tmp = _internal_placement_address;
    _internal_placement_address += sz;
    return tmp;
  }
}

uint32_t kmalloc_a(uint32_t sz) { return kmalloc_internal(sz, 1, 0); }

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys) {
  return kmalloc_internal(sz, 0, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys) {
  return kmalloc_internal(sz, 1, phys);
}

uint32_t kmalloc(uint32_t sz) { return kmalloc_internal(sz, 0, 0); }

// Define c++ new and delete ops
void *operator new(size_t sz) {
  assert(kheap &&
         "Kernel heap must be initialized before using the new operator");
  if (sz == 0)
    ++sz; // avoid std::malloc(0) which may return nullptr on success

  void *ptr = (void *)kmalloc(sz);
  assert(ptr && "New error");
  return ptr;
}

void operator delete(void *ptr) noexcept {
  assert(kheap && "Kernel heap must be initialized before deleting");
  if (ptr)
    free(ptr);
}

void operator delete(void *ptr, size_t sz) noexcept {
  assert(kheap && "Kernel heap must be initialized before deleting");
  if (ptr)
    free(ptr);
}

extern "C" {
void __cxa_pure_virtual() { assert(false && "virtual death"); }

void *__dso_handle;

int __cxa_atexit(void (*destructor)(void *), void *arg, void *dso) {
  return 0;
};
}

#ifdef TEST_RUN_MODE
void reset_kmalloc() {
  //  kheap->first_block->magic = MAGIC & 0xfffffffe; // Set last bit to 0
  //  kheap->first_block->size = kheap->size - sizeof(struct block_header);
  //  kheap->first_block->next = 0;
  //  kheap->first_block->previous = 0;
}
#endif
