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
