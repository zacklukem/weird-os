#include "test/tests.h"
#include <util/rc.h>
using util::rc;

TEST_CASE(rc_does_count) {
  auto r = util::make_rc<int>();
  ASSERT_EQ(r.count(), 1);
  {
    auto p = r;
    ASSERT_EQ(r.count(), 2);
  }
  ASSERT_EQ(r.count(), 1);
}

TEST_CASE(rc_reassign) {
  auto r = util::make_rc<int>();
  auto s = util::make_rc<int>();
  ASSERT_EQ(r.count(), 1);
  ASSERT_EQ(s.count(), 1);
  rc<int> tmp = r;
  ASSERT_EQ(r.count(), 2);
  tmp = s;
  ASSERT_EQ(r.count(), 1);
  ASSERT_EQ(s.count(), 2);
}

TEST_CASE(rc_ptr_cast) {
  struct v {
    int a;
  };
  auto r = util::make_rc<v>();
  r->a = 32;
  auto r_ptr = &r;
  auto r_ptt = (v **)r_ptr;
  ASSERT_EQ((*r_ptt)->a, 32);
}
