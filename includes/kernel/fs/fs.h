#ifndef KERNEL_FS_FS_H
#define KERNEL_FS_FS_H

#include <kernel/fs/flags.h>
#include <kernel/kmalloc.h>
#include <stdint.h>
#include <util/hashmap.h>
#include <util/list.h>
#include <util/llist.h>

namespace fs {

#define FD_MAX 512 // Max file descriptors

int init_fs();

struct inode *create_fifo(const char *path);

// Syscalls for filesystem
ssize_t syscall_read(int fildes, void *buf, size_t nbyte, off_t offset);
ssize_t syscall_write(int fildes, const void *buf, size_t n, off_t offset);
int syscall_close(int fildes);
int syscall_open(const char *path, int oflag, ...);
const struct list_head *syscall_fdopendir(int fildes);
const struct list_head *syscall_opendir(const char *path);
struct dirent *syscall_readdir(struct list_head **head);

void open_inode(struct inode *inode);

ino_t get_new_inode_id();

struct fdtable {
  struct file *data[FD_MAX];
  size_t last_desc;
};

struct file {
  size_t references;
  struct inode *inode;
  int oflag;
  off_t offset;
};

struct inode;

struct dirent {
  // POSIX compliance
  char ident[0xff];
  ino_t inode_id;

  struct inode &inode;
  // For cacheing
  util::list<dirent &> siblings;
  dirent &parent;
  util::list<dirent &> children;
};

// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/stat.h.html
class inode {
public:
  inode(struct dirent &dirent, mode_t mode) : mode(mode), dirent(dirent) {}
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
  virtual const struct list_head opendir();

private:
  struct dirent &dirent;
  int num_links; // Number of open instances of this file
  off_t buf_size;
  void *buf;
};

} // namespace fs

#endif // KERNEL_FS_FS_H
