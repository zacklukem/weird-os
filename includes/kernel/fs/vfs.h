#ifndef KERNEL_FS_VFS_H
#define KERNEL_FS_VFS_H

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
struct inode *resolve_path(const char *path);

#endif // KERNEL_FS_VFS_H
