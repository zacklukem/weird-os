#ifndef KERNEL_FS_UDEV_H
#define KERNEL_FS_UDEV_H

#include <kernel/fs/fs.h>
#include <kernel/fs/fs_device.h>

namespace fs {

class udev : public fs_device {
public:
  udev(dev_t id);
  virtual ~udev() override;

  rc<inode> create_tty(int num);

  virtual optional<rc<inode>> get_inode(ino_t inode) override;
  virtual rc<dirent> mount(const char *name) override;

private:
  rc<dirent> root_dirent;
  rc<directory_like> root_inode;
  util::hashmap<ino_t, rc<inode>> inode_cache;
  inline size_t get_inode_id() { return inode_counter++; };
  size_t inode_counter = 0;
};

} // namespace fs

#endif // KERNEL_FS_UDEV_H
