#include <kernel/fs/fs.h>
#include <kernel/fs/initrd.h>
#include <kernel/fs/udev.h>
#include <kernel/fs/vfs.h>
#include <kernel/process.h>

using namespace fs;

rc<vfs> fs::virt_fs;
rc<udev> fs::virt_udev;
rc<initrd> fs::initrd_dev;

int fs::init_fs() {
  virt_fs = util::make_rc<vfs>();

  initrd_dev = util::make_rc<initrd>(0);
  virt_udev = util::make_rc<udev>(1);

  virt_fs->mount_device("/", initrd_dev);
  virt_fs->mount_device("/dev", virt_udev);

  return 0;
}

optional<rc<dirent>> fs::resolve_path(const char *path) {
  return fs::virt_fs->resolve_path(path);
}

// Syscalls

ssize_t fs::syscall_read(int fildes, void *buf, size_t nbyte, off_t offset) {
  // TODO: check permissions before calling inode func
  process::current()->fdtable.data[fildes]->m_inode->read(buf, nbyte, offset);
  return 0;
}

ssize_t fs::syscall_write(int fildes, const void *buf, size_t n, off_t offset) {
  process::current()->fdtable.data[fildes]->m_inode->write(buf, n, offset);
  return 0;
}

int fs::syscall_close(int fildes) { return 0; }

int fs::syscall_open_at(int fildes, const char *path, int oflag) {
  file *f = new file;

  process::current()->fdtable.data[fildes] = f;

  auto dir_opt = virt_fs->resolve_path(path);
  assert(dir_opt.has_value() && "bad open syscall");
  auto dir = dir_opt.value();

  f->m_inode = dir->m_inode.lock();
  f->references = 1;
  f->oflag = oflag;
  f->offset = 0;

  return fildes;
}

int fs::syscall_open(const char *path, int oflag) {
  return syscall_open_at(process::current()->fdtable.last_desc++, path, oflag);
}

const util::list_node<dirent *> *syscall_fdopendir(int fildes) {
  return process::current()
      ->fdtable.data[fildes]
      ->m_inode->m_dirent->opendir_weak();
}

const util::list_node<dirent *> *syscall_opendir(const char *path) {
  int fildes = syscall_open(path, O_RDONLY);
  return process::current()
      ->fdtable.data[fildes]
      ->m_inode->m_dirent->opendir_weak();
}
