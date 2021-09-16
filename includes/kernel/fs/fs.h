#ifndef INCLUDES_KERNEL_FS_H
#define INCLUDES_KERNEL_FS_H

#include <kernel/fs/flags.h>
#include <kernel/hashmap.h>
#include <kernel/kmalloc.h>
#include <kernel/list.h>
#include <kernel/llist.h>
#include <stdint.h>

#define FD_MAX 512 // Max file descriptors

typedef uint32_t uid_t;
typedef uint32_t ino_t;
typedef uint32_t gid_t;
typedef uint32_t off_t;
typedef uint32_t dev_t;
typedef uint32_t mode_t;
typedef uint32_t nlink_t;
typedef uint32_t time_t;
typedef uint32_t blksize_t;
typedef uint32_t blkcnt_t;

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
void init_inode(struct inode *inode, mode_t mode);

void disuse_file(struct file *file);

struct inode;

struct dirent {
  // POSIX compliance
  char ident[0xff];
  ino_t inode_id;
  struct inode *inode;

  // For cacheing
  struct list_head siblings;
  struct dirent *parent;      // Must be cached if exists
  struct list_head *children; // Children (if cached)
};

struct inode_ops {
  ssize_t (*read)(struct inode *inode, void *buf, size_t nbyte, off_t offset);
  ssize_t (*write)(struct inode *inode, const void *buf, size_t n,
                   off_t offset);
  const struct list_head *(*opendir)(struct inode *inode);
};

// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/stat.h.html
struct inode {
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
  struct inode_ops ops;
  struct dirent *dirent;
  int num_links; // Number of open instances of this file
  struct hashmap_node cache;
  off_t buf_size;
  void *buf;
};

#endif // INCLUDES_KERNEL_FS_H
