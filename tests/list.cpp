#include "test/tests.h"
#include <kernel/kmalloc.h>
#include <util/list.h>

TEST_CASE(list_push_pop_back) {
  util::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  l.push_back(4);
  l.push_back(5);
  //  ASSERT(l.pop_back() == 5);
  //  ASSERT(l.pop_back() == 4);
  //  ASSERT(l.pop_back() == 3);
  //  ASSERT(l.pop_back() == 2);
  //  ASSERT(l.pop_back() == 1);
}

TEST_CASE(list_push_pop_front) {
  util::list<int> l;
  l.push_front(1);
  l.push_front(2);
  l.push_front(3);
  l.push_front(4);
  l.push_front(5);
  ASSERT(l.pop_front() == 5);
  ASSERT(l.pop_front() == 4);
  ASSERT(l.pop_front() == 3);
  ASSERT(l.pop_front() == 2);
  ASSERT(l.pop_front() == 1);
}

TEST_CASE(list_at) {
  util::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  l.push_back(4);
  l.push_back(5);
  ASSERT_EQ(l.at(0), 1);
  ASSERT_EQ(l.at(1), 2);
  ASSERT_EQ(l.at(2), 3);
  ASSERT_EQ(l.at(3), 4);
  ASSERT_EQ(l.at(4), 5);
}
