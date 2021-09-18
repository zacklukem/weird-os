#include "test/tests.h"
#include <string.h>

TEST_CASE(string_strlen) {
  const char *str = "12345678";
  ASSERT_EQ(strlen(str), 8);
}
