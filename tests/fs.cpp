#include "test/tests.h"
#include <kernel/fs/fs.h>

TEST_CASE(fs_basic_udev) {
  auto dirent_opt = fs::resolve_path("/test.txt");
  ASSERT(dirent_opt.has_value());
  auto dirent = dirent_opt.value();
}
