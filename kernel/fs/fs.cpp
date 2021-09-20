#include <kernel/fs/fs.h>
#include <kernel/fs/udev.h>
#include <kernel/fs/vfs.h>

using namespace fs;

rc<vfs> virt_fs;
rc<udev> virt_udev;

int fs::init_fs() {
  virt_fs = util::make_rc<vfs>();
  virt_udev = util::make_rc<udev>(0);

  virt_fs->mount_device("/", virt_udev);
  return 0;
}

optional<rc<dirent>> fs::resolve_path(const char *path) {
  return virt_fs->resolve_path(path);
}
