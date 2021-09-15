#ifndef INCLUDES_KERNEL_FS_H
#define INCLUDES_KERNEL_FS_H

#include <kernel/hashmap.h>
#include <kernel/kmalloc.h>
#include <kernel/list.h>
#include <kernel/llist.h>
#include <stdint.h>

// File mode masks
#define S_IRUSR 0400 // Read permission, owner.
#define S_IWUSR 0200 // Write permission, owner.
#define S_IXUSR 0100 // Execute/search permission, owner.
#define S_IRWXU                                                                \
  S_IRUSR | S_IWUSR | S_IXUSR // Read, write, execute/search by owner.

#define S_IRGRP 040 // Read permission, group.
#define S_IWGRP 020 // Write permission, group.
#define S_IXGRP 010 // Execute/search permission, group.
#define S_IRWXG                                                                \
  S_IRGRP | S_IWGRP | S_IXGRP // Read, write, execute/search by group.

#define S_IROTH 04 // Read permission, others.
#define S_IWOTH 02 // Write permission, others.
#define S_IXOTH 01 // Execute/search permission, others.
#define S_IRWXO                                                                \
  S_IROTH | S_IWOTH | S_IXOTH // Read, write, execute/search by others.

#define S_ISUID 04000 // Set-user-ID on execution.
#define S_ISGID 02000 // Set-group-ID on execution.
#define S_ISVTX 01000 // On directories, restricted deletion flag.

#define S_IFDIR 0040000  // Directory.
#define S_IFCHR 0020000  // Character device
#define S_IFBLK 0060000  // Block device.
#define S_IFREG 0100000  // Regular file.
#define S_IFIFO 0010000  // FIFO.
#define S_IFLNK 0120000  // Symbolic link.
#define S_IFSOCK 0140000 // Socket.

// Open flags
#define O_EXEC 0x1   ///< Exec only
#define O_RDONLY 0x2 ///< Read only
#define O_RDWR 0x4   ///< Read write only
#define O_SEARCH 0x8
#define O_WRONLY 0x10 ///< Write only
#define O_APPEND 0x20 ///< Append mode
#define O_CLOEXEC 0x40
#define O_CREAT 0x80      ///< TODO:Check the standard i have no clue
#define O_DIRECTORY 0x100 ///< Fail if not directory
#define O_DSYNC 0x200
#define O_EXCL 0x400
#define O_NOCTTY 0x800
#define O_NOFOLLOW 0x1000
#define O_NONBLOCK 0x2000
#define O_RSYNC 0x4000
#define O_SYNC 0x8000
#define O_TRUNC 0x10000
#define O_TTY_INIT 0x20000

#define S_ISBLK(m) ((m)&S_IFBLK == S_IFBLK) // Test for a block special file.
#define S_ISCHR(m)                                                             \
  ((m)&S_IFCHR == S_IFCHR) // Test for a character special file.
#define S_ISDIR(m) ((m)&S_IFDIR == S_IFDIR) // Test for a directory.
#define S_ISFIFO(m)                                                            \
  ((m)&S_IFIFO == S_IFIFO) // Test for a pipe or FIFO special file.
#define S_ISREG(m) ((m)&S_IFREG == S_IFREG)    // Test for a regular file.
#define S_ISLNK(m) ((m)&S_IFLNK == S_IFLNK)    // Test for a symbolic link.
#define S_ISSOCK(m) ((m)&S_IFSOCK == S_IFSOCK) // Test for a socket.

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

struct inode *create_fifo(const char *path);

// TODO: move to syscall
ssize_t syscall_read(int fildes, void *buf, size_t nbyte, off_t offset);
ssize_t syscall_write(int fildes, const void *buf, size_t n, off_t offset);
int syscall_close(int fildes);
int syscall_open(const char *path, int oflag, ...);
const struct list_head *fdsyscall_opendir(int fildes);
const struct list_head *syscall_opendir(const char *path);
struct dirent *syscall_readdir(struct list_head **head);

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
