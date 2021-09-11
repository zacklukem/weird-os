#include <assert.h>
#include <kernel/kmalloc.h>
#include <kernel/page.h>
#include <kernel/printk.h>
#include <stdint.h>
#include <stdio.h>

extern uint32_t end;
uint32_t _internal_placement_address = (uint32_t)&end;

#define MAGIC 0x5d6d9cd6
#define MIN_BLOCK_SIZE 4

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
  first->next->previous = first;
  first->size += second->size + sizeof(struct block_header);
}

void heap_info(struct heap *heap) {
  printf("\nBlk Addr | Mem Addr | Stat | Size\n");
  struct block_header *current;
  for (current = heap->first_block; current; current = current->next) {
    printf("%x | %x | %s | %d\n", (uint32_t)current,
           (uint32_t)current + sizeof(struct block_header),
           is_used(current->magic) ? "used" : "free", current->size);
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

// TODO: Page align
static struct block_header *get_suitable_block(size_t size, int page_align,
                                               struct heap *heap) {
  struct block_header *best = heap->first_block;
  struct block_header *current = heap->first_block;
  while (is_used(best->magic)) {
    assert(best->next && "No unused blocks");
    best = best->next;
  }
  while (current->next) {
    // Get smallest unused block with size at least equal to given size
    if (!is_used(current->magic) && current->size >= size &&
        current->size < best->size) {
      best = current;
    }
    current = current->next;
  }
  assert(best->size >= size && "Out of heap space");
  return best;
}

// TODO: page align
void *alloc_internal(size_t size, int page_align, struct heap *heap) {
  struct block_header *block = get_suitable_block(size, page_align, heap);
  // See if we should split the block
  if (block->size - size > sizeof(struct block_header) + MIN_BLOCK_SIZE) {
    // create a new block header
    struct block_header *new_block =
        (struct block_header *)(((void *)block) + size +
                                sizeof(struct block_header));
    if (block->next) {
      block->next->previous = new_block;
    }
    new_block->next = block->next;
    new_block->previous = block;
    new_block->size = block->size - size - sizeof(struct block_header);
    new_block->magic = MAGIC & 0xfffffffe;

    block->size = size;
    block->next = new_block;
    block->magic |= 0x1;
  }
  // Return pointer to the data not the block header
  return ((void *)(block) + sizeof(struct block_header));
}

/**
 * Given an address to a block header, merge the next blocks until the next
 * block is used.
 */
void free(void *mem) {
  // Get pointer to the block header relative to the data
  struct block_header *header =
      (struct block_header *)(mem - sizeof(struct block_header));
  assert(is_valid(header->magic) && "Heap block is invalid!");
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

static uint32_t kmalloc_internal(uint32_t sz, int align, uint32_t *phys) {
  // If the address is not already page-aligned
  if (align == 1 && (_internal_placement_address & 0xFFFFF000)) {
    // Align it.
    _internal_placement_address &= 0xFFFFF000;
    _internal_placement_address += 0x1000;
  }
  if (phys) {
    *phys = _internal_placement_address;
  }
  uint32_t tmp = _internal_placement_address;
  _internal_placement_address += sz;
  return tmp;
}

uint32_t kmalloc_a(uint32_t sz) { return kmalloc_internal(sz, 1, 0); }

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys) {
  return kmalloc_internal(sz, 0, phys);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys) {
  return kmalloc_internal(sz, 1, phys);
}

uint32_t kmalloc(uint32_t sz) { return kmalloc_internal(sz, 0, 0); }