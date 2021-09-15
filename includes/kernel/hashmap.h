#ifndef INCLUDES_KERNEL_HASHMAP_Hj
#define INCLUDES_KERNEL_HASHMAP_H

#include <assert.h>
#include <kernel/macros.h>
#include <stdint.h>
#include <string.h>

#define HASH_RANGE 0xff

struct hashmap_node {
  size_t key;
  struct hashmap_node *next;
};

struct hashmap {
  struct hashmap_node *table[HASH_RANGE];
  int (*pair_eq)(size_t a, size_t b);
  int (*get_hash)(size_t key);
};

static inline uint8_t hash_str(unsigned char *str) {
  uint32_t hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; // hash*33 + c
  return (uint8_t)(hash % 0x100);
}

static inline uint8_t hash_int(size_t i) {
  unsigned char ci[sizeof(size_t) + 1];
  ci[sizeof(size_t)] = 0;
  *((size_t *)ci) = i;
  return hash_str(ci);
}

static int __get_int_hash(size_t key) { return hash_int(key); }

static int __int_pair_eq(size_t a, size_t b) { return a == b; }

static inline struct hashmap make_int_hashmap() {
  struct hashmap map;
  memset(map.table, 0, HASH_RANGE);
  map.get_hash = __get_int_hash;
  map.pair_eq = __int_pair_eq;
  return map;
}

static inline void init_hashmap_node(struct hashmap_node *node, size_t key) {
  node->key = key;
  node->next = (void *)0;
}

static inline void hashmap_add(struct hashmap *map, struct hashmap_node *node) {
  int hash = map->get_hash(node->key);
  if (!map->table[hash]) {
    map->table[hash] = node;
    return;
  }
  struct hashmap_node *terminal_n = map->table[hash];
  while (terminal_n->next) {
    terminal_n = terminal_n->next;
  }
  terminal_n->next = node;
}

static inline struct hashmap_node *hashmap_get(struct hashmap *map,
                                               size_t key) {
  int hash = map->get_hash(key);
  struct hashmap_node *node = map->table[hash];
  while (node && !map->pair_eq(key, node->key))
    node = node->next;
  if (!node) // Maybe unnessesary
    return (void *)0;
  assert(map->pair_eq(key, node->key) && "Key failure");
  return node;
}

#define hashmap_entry(ptr, type, member) container_of(ptr, type, member)

#endif // INCLUDES_KERNEL_HASHMAP_H
