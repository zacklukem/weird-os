#include <assert.h>
#include <kernel/fs/fs.h>
#include <kernel/process.h>
#include <kernel/vector.h>
#include <string.h>

#define ROOT_UID 0
#define ROOT_GID 0
#define DEVICE_VFS 0

static struct list_head blank_list = {0, 0};
static ino_t inode_counter = 0;

ino_t get_new_inode_id() { return inode_counter++; }

struct inode *root_inode;
struct dirent *root_dirent;

struct hashmap inode_cache;

static ssize_t blank_read(struct inode *inode, void *buf, size_t nbyte,
                          off_t offset) {
  return 0;
};
static ssize_t blank_write(struct inode *inode, const void *buf, size_t n,
                           off_t offset) {
  return 0;
};
static const struct list_head *blank_opendir(struct inode *inode) {
  return &blank_list;
};

static const struct list_head *root_opendir(struct inode *inode) {
  return inode->dirent->children;
};

void init_inode(struct inode *inode, mode_t mode) {
  memset(root_inode, 0, sizeof inode); // Make zero
  root_inode->mode = mode;
  root_inode->id = get_new_inode_id();
  root_inode->device = DEVICE_VFS;
  root_inode->uid = ROOT_UID;
  root_inode->gid = ROOT_GID;
  root_inode->buf_size = 0;
  // ops
  root_inode->ops.read = blank_read;
  root_inode->ops.write = blank_write;
  root_inode->ops.opendir = blank_opendir;
}

void open_inode(struct inode *inode) {
  init_hashmap_node(&inode->cache, inode->id);
  hashmap_add(&inode_cache, &inode->cache);
}

static int init_root() {
  root_inode = (inode *)kmalloc(sizeof(struct inode));
  root_dirent = (dirent *)kmalloc(sizeof(struct dirent));
  memset(root_inode, 0, sizeof(struct inode));   // Make zero
  memset(root_dirent, 0, sizeof(struct dirent)); // Make zero
  init_inode(root_inode, S_IRWXG | S_IRWXO | S_IRWXU | S_IFDIR);
  open_inode(root_inode);
  root_inode->dirent = root_dirent;
  root_dirent->inode = root_inode;
  root_dirent->inode_id = root_inode->id;
  root_inode->ops.opendir = root_opendir;
  return 0;
}

int init_fs() {
  inode_cache = make_int_hashmap();
  init_root();
  return 0;
}

void disuse_inode(struct inode *inode) {
  inode->num_links--;
  if (inode->num_links == 0) {
    // TODO: inode cache remove this inode?
    free(inode);
  }
}

void disuse_file(struct file *file) {
  file->references--;
  // Who thought we would see reference counting in c? nice...
  if (file->references == 0) {
    disuse_inode(file->inode);
    // TODO: free the old table entry so fdtable doesn't fill up hehe
    free(file);
  }
}

ssize_t syscall_read(int fildes, void *buf, size_t nbyte, off_t offset) {
  struct fdtable *tbl = &current()->fdtable;
  struct file *file = tbl->data[fildes];
  return file->inode->ops.read(file->inode, buf, nbyte, offset);
}

ssize_t syscall_write(int fildes, const void *buf, size_t n, off_t offset) {
  struct fdtable *tbl = &current()->fdtable;
  struct file *file = tbl->data[fildes];
  return file->inode->ops.write(file->inode, buf, n, offset);
}

int syscall_close(int fildes) {
  // struct fdtable *tbl = &current()->fdtable;
  // struct file *file = tbl->data[fildes];
  return 0; // TODO: make this follow POSIX?
}

// only absolute paths for now
int syscall_open(const char *path, int oflag, ...) {
  // split path
  // find initial
  assert(path[0] == '/' && "Only absolute paths rn");
  struct file *new_file = (file *)kmalloc(sizeof(struct file));
  new_file->oflag = oflag;
  new_file->inode = 0; // TODO: get inode here properly
  new_file->inode->num_links++;
  new_file->references = 1;
  new_file->offset = 0;

  struct fdtable *tbl = &current()->fdtable;

  // TODO: get a new one and free the old one
  tbl->data[tbl->last_desc++] = new_file;

  return tbl->last_desc;
}

const struct list_head *fdsyscall_opendir(int fildes) {
  struct fdtable *tbl = &current()->fdtable;
  struct file *file = tbl->data[fildes];
  return file->inode->ops.opendir(file->inode);
}

const struct list_head *syscall_opendir(const char *path) {
  int fildes = syscall_open(path, O_RDWR | O_DIRECTORY);
  return fdsyscall_opendir(fildes);
}

struct dirent *syscall_readdir(struct list_head **head) {
  if (!head)
    return nullptr;
  struct list_head *old_head = *head;
  *head = old_head->next;
  return list_entry(old_head, struct dirent, siblings);
}
