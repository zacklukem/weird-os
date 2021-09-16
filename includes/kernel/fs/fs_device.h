#ifndef KERNEL_FS_FS_DEVICE_H
#define KERNEL_FS_FS_DEVICE_H

struct fs_device {
  dev_t id;

  struct inode *(*get_inode)(struct fs_device *dev, ino_t inode);
  struct node *(*resolve_path)(struct fs_device *dev, const char *path);
  const struct list_head *(*opendir)(struct fs_device *dev,
                                     struct inode *inode);
};

/**
 * @brief Get the inode object for a given inode id
 *
 * @param inode
 * @return struct inode*
 */
static inline struct inode *dev_get_inode(struct fs_device *dev, ino_t inode) {
  dev->get_inode(dev, inode);
}

/**
 * @brief Get the inode object from the given path
 *
 * @param path
 * @return struct inode*
 */
static inline struct inode *dev_resolve_path(struct fs_device *dev,
                                             const char *path) {
  dev->resolve_path(dev, path);
}

/**
 * @brief Cache the dir entries for a file
 *
 * @param inode
 * @return const struct list_head*
 */
static inline const struct list_head *dev_opendir(struct fs_device *dev,
                                                  struct inode *inode) {
  return dev->opendir(dev, inode);
}

#endif // KERNEL_FS_FS_DEVICE_H
