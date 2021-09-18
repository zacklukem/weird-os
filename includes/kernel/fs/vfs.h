#ifndef KERNEL_FS_VFS_H
#define KERNEL_FS_VFS_H

#include <kernel/fs/fs.h>
#include <kernel/fs/fs_device.h>
#include <util/rc.h>
#include <util/string.h>

using util::optional;
using util::rc;
using util::string;

namespace fs {

class vfs {
public:
  vfs();
  /**
   * @brief Resolves pathname to an inode
   *
   * Given a path, the resolve path function checks existing inodes and
   * recursively resolves the device id and inode id of a file.  It then queries
   * the required device to return an inode.
   *
   * @param path the path to resolve
   * @return struct inode* a pointer to a cached inode
   */
  optional<rc<inode>> resolve_path(const char *path);

  void mount_device(string path, rc<fs_device> device);

  struct mount_entry {
    rc<fs_device> device;
  };

private:
  rc<fs_device> root_device;
  util::hashmap<dev_t, rc<fs_device>> fs_devices;
  util::hashmap<string, mount_entry> mounts;
};

} // namespace fs

#endif // KERNEL_FS_VFS_H
