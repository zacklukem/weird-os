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

  initrd_dev = util::make_rc<initrd>(0);
  virt_udev = util::make_rc<udev>(1);

  virt_fs->mount_device("/", initrd_dev);
  virt_fs->mount_device("/dev", virt_udev);
  return 0;
}

optional<rc<dirent>> fs::resolve_path(const char *path) {
  return virt_fs->resolve_path(path);
}

ssize_t fs::syscall_read(int fildes, void *buf, size_t nbyte, off_t offset) {
  return 0;
}
ssize_t fs::syscall_write(int fildes, const void *buf, size_t n, off_t offset) {
  return 0;
}
int fs::syscall_close(int fildes) { return 0; }
int fs::syscall_open(const char *path, int oflag) { return 0; }
