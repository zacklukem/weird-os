#ifndef KERNEL_FS_FS_H
#define KERNEL_FS_FS_H

#include <kernel/fs/flags.h>
#include <kernel/kmalloc.h>
#include <stdint.h>
#include <util/hashmap.h>
#include <util/list.h>
#include <util/rc.h>

namespace fs {

#define FD_MAX 512 // Max file descriptors

// Forward declarations
class inode;
struct dirent;
struct file;
struct fdtable;

int init_fs();

inode *create_fifo(const char *path);

// Syscalls for filesystem
ssize_t syscall_read(int fildes, void *buf, size_t nbyte, off_t offset);
ssize_t syscall_write(int fildes, const void *buf, size_t n, off_t offset);
int syscall_close(int fildes);
int syscall_open(const char *path, int oflag, ...);
const util::list_iterator<dirent> *syscall_fdopendir(int fildes);
const util::list_iterator<dirent> *syscall_opendir(const char *path);

void open_inode(inode *inode);

struct fdtable {
  file *data[FD_MAX];
  size_t last_desc;
};

struct file {
  size_t references;
  inode *m_inode;
  int oflag;
  off_t offset;
};

class inode;

struct dirent {
  // POSIX compliance
  char ident[0xff];
  ino_t inode_id;

  inode &m_inode;
  // For cacheing
  util::list<dirent &> siblings;
  dirent &parent;
  util::list<dirent &> children;
};

// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/stat.h.html
class inode {
public:
  inode(dirent &m_dirent, mode_t mode) : mode(mode), m_dirent(m_dirent) {}
  dev_t device;       // Device ID of device containing file.
  ino_t id;           // File serial number.
  mode_t mode;        // Mode of file (see below).
  nlink_t nlink;      // Number of hard links to the file.
  uid_t uid;          // User ID of file.
  gid_t gid;          // Group ID of file.
  dev_t rdevice;      // Device ID (if file is character or block special).
  off_t size;         // For regular files, the file size in bytes.
  time_t access_time; // Time of last access.
  time_t mod_time;    // Time of last data modification.
  time_t stat_time;   // Time of last status change.

  virtual ssize_t read(void *buf, size_t nbyte, off_t offset);
  virtual ssize_t write(const void *buf, size_t n, off_t offset);
  virtual const util::list_iterator<dirent> opendir();

private:
  dirent &m_dirent;
  int num_links; // Number of open instances of this file
  off_t buf_size;
  void *buf;
};

} // namespace fs

#endif // KERNEL_FS_FS_H
