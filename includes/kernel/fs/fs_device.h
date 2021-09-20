#ifndef KERNEL_FS_FS_DEVICE_H
#define KERNEL_FS_FS_DEVICE_H

#include <kernel/fs/fs.h>
#include <util/rc.h>
using util::optional;
using util::rc;

namespace fs {

class fs_device {
public:
  dev_t id;

  fs_device(dev_t id) : id(id){};

  virtual ~fs_device(){};

  virtual rc<dirent> mount(const char *name) = 0;

  /**
   * @brief Get the inode object for a given inode id
   *
   * @param inode
   * @return struct inode*
   */
  virtual optional<rc<inode>> get_inode(ino_t inode) = 0;
};

} // namespace fs

#endif // KERNEL_FS_FS_DEVICE_H
