/**
 * @brief Main kernel filesystem header.
 *
 * The filesystem is separated into several levels of abstraction:
 *
 * 1. C standard library
 * 2. Syscalls
 * 3. Virtual filesystem
 * 4. Filesystem device drivers
 *
 * The syscalls make calls to the virtual filesystem from userspace by
 * retrieving inodes referenced by process file descriptors. The VFS then
 * recursively traverses the directory tree and calls device driver code to get
 * the nesessary data.  The device drivers then query their respective devices
 * and manage their own inode and directory caches.
 */

#ifndef KERNEL_FS_FS_H
#define KERNEL_FS_FS_H

#include <kernel/fs/flags.h>
#include <kernel/kmalloc.h>
#include <stdint.h>
#include <util/hashmap.h>
#include <util/list.h>
#include <util/optional.h>
#include <util/rc.h>

using util::rc;
using util::weak;

namespace fs {

#define FD_MAX 512 // Max file descriptors

// Forward declarations
class inode;
struct dirent;
struct file;
struct fdtable;

int init_fs();

// Syscalls for filesystem
ssize_t syscall_read(int fildes, void *buf, size_t nbyte, off_t offset);
ssize_t syscall_write(int fildes, const void *buf, size_t n, off_t offset);
int syscall_close(int fildes);
int syscall_open(const char *path, int oflag, ...);
const util::list_iterator<dirent> *syscall_fdopendir(int fildes);
const util::list_iterator<dirent> *syscall_opendir(const char *path);

struct fdtable {
  file *data[FD_MAX];
  size_t last_desc;
};

/**
 * @brief An entry in the global open file table
 *
 * Contains a link to an inode, the current read flags and an offset
 */
struct file {
  int references;
  rc<inode> m_inode;
  int oflag;
  off_t offset;
};

/**
 * @brief The inode class maintains information about a file
 *
 * An inode contains the device id of a device driver and inode number unique
 * to each device.  The inode also contains virtual methods that are set by
 * a device driver to perform tasks on an inode.
 */
class inode {
public:
  inode(rc<dirent> m_dirent, ino_t id, mode_t mode)
      : id(id), mode(mode), m_dirent(m_dirent) {}

  dev_t device;       ///< Device ID of device containing file.
  ino_t id;           ///< File serial number.
  mode_t mode;        ///< Mode of file (see below).
  nlink_t nlink;      ///< Number of hard links to the file.
  uid_t uid;          ///< User ID of file.
  gid_t gid;          ///< Group ID of file.
  dev_t rdevice;      ///< Device ID (if file is character or block special).
  off_t size;         ///< For regular files, the file size in bytes.
  time_t access_time; ///< Time of last access.
  time_t mod_time;    ///< Time of last data modification.
  time_t stat_time;   ///< Time of last status change.

  virtual ~inode(){};
  virtual ssize_t read(void *buf, size_t nbyte, off_t offset) { return 0; };
  virtual ssize_t write(const void *buf, size_t n, off_t offset) { return 0; };
  virtual const util::list_iterator<rc<dirent>> opendir() {
    return util::list_iterator<rc<dirent>>{nullptr};
  };

  rc<dirent> m_dirent;
  int num_links; // Number of open instances of this file
};

/**
 * @brief directory entry containing string identifiers for inodes.
 *
 * The dirent maintains a cache of directory entries, their string identifiers
 * and their inode identifiers.
 */
struct dirent {
  dirent(const char *ident, weak<inode> m_inode,
         util::optional<rc<dirent>> parent)
      : ident(ident), inode_id(m_inode.lock()->id), m_inode(m_inode),
        parent(parent), children(){};
  const char *ident; ///< The string identifier of this direntry
  ino_t inode_id;    ///< The raw inode id of this entry

  weak<inode> m_inode; ///< A reference to the raw inode
  // For cacheing
  util::optional<rc<dirent>> parent; ///< A reference to this dirent's parent
  util::list<rc<dirent>> children;   ///< the dirent's children
};

class directory_like : public inode {
public:
  directory_like(rc<dirent> m_dirent, ino_t id, mode_t mode)
      : inode(m_dirent, id, mode | S_IFDIR){};

  void add_child(rc<dirent> dirent) { dirent->children.push_front(dirent); };

  virtual const util::list_iterator<rc<dirent>> opendir() override {
    return m_dirent->children.begin();
  };
};

template <class T, class... InoArgs>
rc<T> make_inode_dirent_pair(const char *ident,
                             util::optional<rc<dirent>> parent,
                             InoArgs... ino_args) {
  auto dir = util::make_rc<dirent>(ident, weak<inode>(), parent);
  auto ino = util::make_rc<T>(dir, ino_args...);
  dir->m_inode = ino;
  return ino;
}

} // namespace fs

#endif // KERNEL_FS_FS_H
