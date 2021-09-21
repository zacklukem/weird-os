#include <kernel/fs/fs.h>
#include <kernel/fs/initrd.h>
#include <kernel/fs/udev.h>
#include <kernel/fs/vfs.h>

using namespace fs;

rc<vfs> virt_fs;
rc<udev> virt_udev;
rc<initrd> initrd_dev;

int fs::init_fs() {
  virt_fs = util::make_rc<vfs>();

  // virt_udev = util::make_rc<udev>(0);
  initrd_dev = util::make_rc<initrd>(0);

  virt_fs->mount_device("/", initrd_dev);
  return 0;
}

optional<rc<dirent>> fs::resolve_path(const char *path) {
  return virt_fs->resolve_path(path);
}
