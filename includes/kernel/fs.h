#ifndef INCLUDES_KERNEL_FS_H
#define INCLUDES_KERNEL_FS_H

#include <stdint.h>

typedef uint32_t uid_t;
typedef uint32_t ino_t;
typedef uint32_t gid_t;
typedef uint32_t off_t;

// TODO: move to unistd
ssize_t read(int fildes, void *buf, size_t nbyte, off_t offset);
ssize_t write(int fildes, const void *buf, size_t n, off_t offset);
int close(int fildes);
int open(const char *path, int oflag, ...);

struct fs_node;

struct dirent {
  char name[128]; // Filename.
  ino_t ino;      // Inode number. Required by POSIX.
};

typedef uint32_t (*read_type_t)(struct fs_node *node, void *buf, size_t n,
                                off_t offset);
typedef uint32_t (*write_type_t)(struct fs_node *node, const void *buf,
                                 size_t n, off_t offset);
typedef void (*open_type_t)(struct fs_node *node, int oflag, ...);
typedef void (*close_type_t)(struct fs_node *node);
typedef struct dirent *(*readdir_type_t)(struct fs_node *, uint32_t);
typedef struct fs_node *(*finddir_type_t)(struct fs_node *, char *name);

#define FS_FILE 0x01
#define FS_DIRECTORY 0x02
#define FS_CHARDEVICE 0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE 0x05
#define FS_SYMLINK 0x06
#define FS_MOUNTPOINT 0x08 // Is the file an active mountpoint?

struct fs_node {
  char name[128];  // The filename.
  uint32_t mask;   // The permissions mask.
  uid_t uid;       // The owning user.
  gid_t gid;       // The owning group.
  uint32_t flags;  // Includes the node type. See #defines above.
  ino_t inode;     // This is device-specific - provides a way for a
  uint32_t length; // Size of the file, in bytes.
  read_type_t read;
  write_type_t write;
  open_type_t open;
  close_type_t close;
  readdir_type_t readdir;
  finddir_type_t finddir;
  struct fs_node *ptr; // Used by mountpoints and symlinks.
};

uint32_t read_node(struct fs_node *node, void *buf, size_t n, off_t offset);
uint32_t write_node(struct fs_node *node, const void *buf, size_t n,
                    off_t offset);
void open_node(struct fs_node *node, int oflag);
void close_node(struct fs_node *node);
struct dirent *readdir_node(struct fs_node *, uint32_t);
struct fs_node *finddir_node(struct fs_node *, char *name);

#endif // INCLUDES_KERNEL_FS_H
