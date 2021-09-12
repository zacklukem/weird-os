#ifndef INCLUDES_KERNEL_KMALLOC_H
#define INCLUDES_KERNEL_KMALLOC_H

#include <stdint.h>

extern uint32_t _internal_placement_address;

struct block_header {
  /**
   * 0x5d6d9cd6, where last bit is set if the block is
   * used
   */
  uint32_t magic;
  /**
   * The size of the block
   */
  uint32_t size;
  /**
   * Links to the next block
   */
  struct block_header *next;
  /**
   * Link to the previous block
   */
  struct block_header *previous;
};

struct heap {
  struct block_header *first_block;
  uint32_t size;
};

void heap_info();

/**
 * Page aligned kernel malloc
 */
uint32_t kmalloc_a(uint32_t sz);

/**
 * Unaligned kernel malloc with physical address
 */
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);

/**
 * Page aligned kernel malloc with physical address
 */
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);

/**
 * Unaligned kernel malloc
 */
uint32_t kmalloc(uint32_t sz);

/**
 * @brief Reallocate an already allocated chunk of memory
 *
 * @param ptr
 * @param sz
 * @return uint32_t
 */
uint32_t krealloc(uint32_t ptr, uint32_t sz);

void init_kernal_heap(size_t size, void *start);
struct heap create_heap(size_t size, void *start);
void *alloc_internal(size_t size, int page_align, struct heap *heap);
void free(void *mem);

#endif // INCLUDES_KERNEL_KMALLOC_H
