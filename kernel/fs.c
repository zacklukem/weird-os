#include <assert.h>
#include <kernel/fs.h>
#include <kernel/process.h>
#include <kernel/vector.h>

#define ROOT_UID 0
#define ROOT_GID 0
#define DEVICE_VFS 0
static ino_t inode_counter = 0;

int init_fs() { open_file_table = vector_create(struct file); }

ssize_t syscall_read(int fildes, void *buf, size_t nbyte, off_t offset) {
  struct fdtable *tbl = &current()->fdtable;
  struct file *file = tbl->data[fildes];
  file->inode->ops->read(file->inode, buf, nbyte, offset);
}

ssize_t syscall_write(int fildes, const void *buf, size_t n, off_t offset) {
  struct fdtable *tbl = &current()->fdtable;
  struct file *file = tbl->data[fildes];
  file->inode->ops->write(file->inode, buf, n, offset);
}

int syscall_close(int fildes) {
  struct fdtable *tbl = &current()->fdtable;
  struct file *file = tbl->data[fildes];
  file->references--;
  // Who thought we would see reference counting in c? nice...
  if (file->references == 0) {
    // TODO: free the old table entry so fdtable doesn't fill up hehe
    free(file);
  }
  return 0; // TODO: make this follow POSIX?
}

// only absolute paths for now
int syscall_open(const char *path, int oflag, ...) {
  // split path
  // find initial
  assert(path[0] == '/' && "Only absolute paths rn");
  struct file *new_file = (void *)kmalloc(sizeof(struct file));
  file->oflag = oflag;
  file->inode = 0; // TODO: get inode here properly
  file->references = 1;

  struct fdtable *tbl = &current()->fdtable;

  // TODO: get a new one and free the old one
  tbl->data[tbl->last_desc++] = new_file;

  return tbl->last_desc;
}