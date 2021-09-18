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

  /**
   * @brief Get the inode object for a given inode id
   *
   * @param inode
   * @return struct inode*
   */
  virtual optional<rc<inode>> get_inode(ino_t inode) = 0;

  /**
   * @brief Get the inode object from the given path
   *
   * Regardless of if the input path has a leading slash, the path is relative
   * to the devices root.
   *
   * @param path
   * @return struct inode*
   */
  virtual optional<rc<inode>> resolve_path(const char *path) = 0;

  /**
   * @brief Cache the dir entries for a file
   *
   * @param inode
   * @return const struct list_head*
   */
  virtual util::list_iterator<rc<dirent>> opendir(rc<inode> inode) = 0;
};

} // namespace fs

#endif // KERNEL_FS_FS_DEVICE_H
