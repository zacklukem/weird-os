#include "test/tests.h"
#include <kernel/fs/fs.h>

TEST_CASE(fs_basic_udev) {
  auto dirent_opt = fs::resolve_path("/test.txt");
  ASSERT(dirent_opt.has_value());
  auto dirent = dirent_opt.value();
  auto ino = dirent->m_inode.lock();
  char buf[32];
  memset(buf, 0, sizeof(buf));
  int l = ino->read(buf, 10, 0);
  ASSERT(strcmp(buf, "hi\n"));
  ASSERT(l == 3);
}
