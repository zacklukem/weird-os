#include "test/tests.h"
#include <kernel/kmalloc.h>
#pragma GCC diagnostic ignored "-Wpointer-arith"

static inline struct block_header *get_header(const void *mem) {
  return (struct block_header *)(mem - sizeof(struct block_header));
}

/**
 * @brief Test if a single kmalloc returns the expected result
 */
TEST_CASE(kernel_heap_kmalloc_single) {
  void *mem = (void *)kmalloc(32);
  struct block_header *blk = get_header(mem);
  ASSERT(blk->size == 32);
  ASSERT(blk->magic == (MAGIC | 0x1));
  ASSERT(blk->next->previous == blk);
  ASSERT(blk->next->next == 0);
  ASSERT(blk->next->magic == (MAGIC & 0xfffffffe));
  free(mem);
}

/**
 * @brief Test if multiple successive mallocs produces the desired result
 */
TEST_CASE(kernel_heap_kmalloc_multi_ordered) {
  struct block_header *head[10];
  void *ptr[10];
  for (int i = 0; i < 10; i++) {
    void *mem = (void *)kmalloc(32);
    head[i] = get_header(mem);
    ptr[i] = mem;
  }
  for (int i = 0; i < 9; i++) {
    ASSERT(head[i]->size == 32);
    ASSERT(head[i]->magic == (MAGIC | 0x1));
    ASSERT(head[i]->next->previous == head[i]);
    ASSERT(head[i]->next->magic == (MAGIC | 0x1));
  }
  ASSERT(head[9]->size == 32);
  ASSERT(head[9]->magic == (MAGIC | 0x1));
  ASSERT(head[9]->next->previous == head[9]);
  ASSERT(head[9]->next->magic == (MAGIC & 0xfffffffe));
  for (int i = 0; i < 10; i++) {
    free(ptr[i]);
  }
}

TEST_CASE(kernel_heap_merge_free) {
  void *a = (void *)kmalloc(32);
  void *b = (void *)kmalloc(16);
  void *c = (void *)kmalloc(64);
  void *d = (void *)kmalloc(64);
  free(b);
  free(c);
  struct block_header *a_header = get_header(a);
  struct block_header *d_header = get_header(d);

  ASSERT(a_header->next->size == 64 + 16 + sizeof(struct block_header));
  ASSERT(a_header->next->previous == a_header);
  ASSERT(a_header->next->next == d_header);
  ASSERT(a_header->next->magic == (MAGIC & 0xfffffffe));
  ASSERT(a_header->next == get_header(b));

  free(a);
  free(d);
}
