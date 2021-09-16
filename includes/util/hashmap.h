#ifndef KERNEL_UTIL_HASHMAP_H
#define KERNEL_UTIL_HASHMAP_H
#ifndef INCLUDES_KERNEL_HASHMAP_Hj
#define INCLUDES_KERNEL_HASHMAP_H

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <util/macros.h>
#include <util/optional.h>

#define HASH_RANGE 0xff

namespace util {

template <class K, class V> struct hashmap_node {
  hashmap_node(const K &key, const V &value)
      : key(key), value(value), next(nullptr) {}
  K key;
  V value;
  hashmap_node<K, V> *next;
};

static inline uint8_t hash(unsigned char *str) {
  uint32_t hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; // hash*33 + c
  return (uint8_t)(hash % 0x100);
}

static inline uint8_t hash(size_t i) {
  unsigned char ci[sizeof(size_t) + 1];
  ci[sizeof(size_t)] = 0;
  *((size_t *)ci) = i;
  return hash(ci);
}

template <class K, class V> class hashmap {
public:
  hashmap() { memset(table, 0, HASH_RANGE * sizeof(void *)); };

  util::optional<V> get(const K &key) {
    int hashed = hash(key);
    struct hashmap_node<K, V> *node = table[hashed];
    while (node && !(key == node->key))
      node = node->next;
    if (!node) // Maybe unnessesary
      return util::nullopt;
    assert(key == node->key && "Key failure");
    return node->value;
  };

  void set(const K &key, const V &value) {
    int hashed = hash(key);
    if (table[hashed] == nullptr) {
      table[hashed] = new hashmap_node<K, V>(key, value);
      return;
    }
    struct hashmap_node<K, V> *terminal_n = table[hashed];
    while (terminal_n->next) {
      terminal_n = terminal_n->next;
    }
    terminal_n->next = new hashmap_node<K, V>(key, value);
  };

private:
  struct hashmap_node<K, V> *table[HASH_RANGE];
};

} // namespace util

#endif // INCLUDES_KERNEL_HASHMAP_H

#endif // KERNEL_UTIL_HASHMAP_H
