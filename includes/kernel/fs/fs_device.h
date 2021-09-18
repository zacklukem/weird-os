#ifndef KERNEL_FS_FS_DEVICE_H
#define KERNEL_FS_FS_DEVICE_H

#include <kernel/fs/fs.h>
#include <util/rc.h>
using util::rc;

namespace fs {

class fs_device {
public:
  dev_t id;

  /**
   * @brief Get the inode object for a given inode id
   *
   * @param inode
   * @return struct inode*
   */
  virtual rc<inode> get_inode(fs_device *dev, ino_t inode) = 0;

  /**
   * @brief Get the inode object from the given path
   *
   * @param path
   * @return struct inode*
   */
  virtual rc<inode> resolve_path(fs_device *dev, const char *path) = 0;

  /**
   * @brief Cache the dir entries for a file
   *
   * @param inode
   * @return const struct list_head*
   */
  virtual util::list_iterator<dirent> opendir(fs_device *dev,
                                              rc<inode> inode) = 0;
};

} // namespace fs

#endif // KERNEL_FS_FS_DEVICE_H
