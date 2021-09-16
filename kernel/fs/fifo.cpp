#include <kernel/fs/fs.h>

static ssize_t fifo_read(struct inode *inode, void *buf, size_t nbyte,
                         off_t offset) {
  memcpy(buf, inode->buf, nbyte);
  memcpy(inode->buf, inode->buf + nbyte, inode->buf_size);
  inode->buf_size -= nbyte;
  return 0;
};

static ssize_t fifo_write(struct inode *inode, const void *buf, size_t n,
                          off_t offset) {
  memcpy(inode->buf + inode->buf_size, buf, n);
  inode->buf_size += n;
  return 0;
};

struct inode *create_fifo(const char *path) {
  struct inode *out = (inode *)kmalloc(sizeof(struct inode));
  init_inode(out, S_IRWXG | S_IRWXO | S_IRWXU | S_IFIFO);
  out->buf = (inode *)kmalloc(0xff);
  out->ops.read = fifo_read;
  out->ops.write = fifo_write;

  return 0;
}