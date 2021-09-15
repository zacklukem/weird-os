#include "test/tests.h"
#include <kernel/hashmap.h>
#include <stdio.h>

struct hashmap_test_struct {
  struct hashmap_node test_node;
  int test_data;
};

static inline struct hashmap_test_struct create_test_struct(uint32_t key,
                                                            int data) {
  struct hashmap_test_struct out;
  init_hashmap_node(&out.test_node, key);
  out.test_data = data;
  return out;
}

TEST_CASE(hashmap_int_adds_keys) {
  struct hashmap map = make_int_hashmap();
  struct hashmap_test_struct str = create_test_struct(2532, 1234);
  hashmap_add(&map, &str.test_node);
  struct hashmap_test_struct str1 = create_test_struct(22532, 1234);
  hashmap_add(&map, &str1.test_node);
  struct hashmap_test_struct str2 = create_test_struct(2532, 1234);
  hashmap_add(&map, &str2.test_node);
}

TEST_CASE(hashmap_int_get) {
  struct hashmap map = make_int_hashmap();
  struct hashmap_test_struct str = create_test_struct(1, 1234);
  hashmap_add(&map, &str.test_node);
  struct hashmap_test_struct str1 = create_test_struct(2, 5678);
  hashmap_add(&map, &str1.test_node);

  ASSERT(hash_int(1) != hash_int(2));

  struct hashmap_test_struct *str_g = hashmap_entry(
      hashmap_get(&map, 1), struct hashmap_test_struct, test_node);
  ASSERT(str_g->test_data == 1234);

  struct hashmap_test_struct *str2_g = hashmap_entry(
      hashmap_get(&map, 2), struct hashmap_test_struct, test_node);
  ASSERT(str2_g->test_data == 5678);
}

TEST_CASE(hashmap_int_collision) {
  struct hashmap map = make_int_hashmap();

  // 352 & 1
  const int key_a = 1;
  const int key_b = 352;
  assert(hash_int(key_a) == hash_int(key_b) &&
         "change above to fix test when hash function changes");

  struct hashmap_test_struct str = create_test_struct(key_a, 1234);
  hashmap_add(&map, &str.test_node);
  struct hashmap_test_struct str1 = create_test_struct(key_b, 5678);
  hashmap_add(&map, &str1.test_node);

  ASSERT(hash_int(1) != hash_int(2));

  struct hashmap_test_struct *str_g = hashmap_entry(
      hashmap_get(&map, key_a), struct hashmap_test_struct, test_node);
  ASSERT(str_g->test_data == 1234);

  struct hashmap_test_struct *str2_g = hashmap_entry(
      hashmap_get(&map, key_b), struct hashmap_test_struct, test_node);
  ASSERT(str2_g->test_data == 5678);
}