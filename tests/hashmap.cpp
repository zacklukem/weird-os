#include "test/tests.h"
#include <stdio.h>
#include <util/hashmap.h>

TEST_CASE(hashmap_int_adds_keys) {
  util::hashmap<int, int> map;
  map.set(1, 1234);
  map.set(2, 1234);
  map.set(3, 1234);
  map.set(4, 1234);
}

TEST_CASE(hashmap_int_get) {
  util::hashmap<int, int> map;
  map.set(1, 1234);
  map.set(2, 4568);
  ASSERT(util::hash(1) != util::hash(2));
  ASSERT(map.get(1).value() == 1234);
  ASSERT(map.get(2).value() == 4568);
}

TEST_CASE(hashmap_int_collision) {
  util::hashmap<int, int> map;

  // 352 & 1
  const int key_a = 1;
  const int key_b = 352;
  assert(util::hash(key_a) == util::hash(key_b) &&
         "change above to fix test when hash function changes");

  map.set(key_a, 1234);
  map.set(key_b, 4568);
  ASSERT(map.get(key_a).value() == 1234);
  ASSERT(map.get(key_b).value() == 4568);
}