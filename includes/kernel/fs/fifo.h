#ifndef KERNEL_FS_FIFO_H
#define KERNEL_FS_FIFO_H

#include <kernel/fs/fs.h>

namespace fs {

class fifo : public inode {
public:
  fifo(rc<dirent> m_dirent, ino_t id, mode_t mode);
  virtual ~fifo() override;
  virtual ssize_t read(void *buf, size_t nbyte, off_t offset) override;
  virtual ssize_t write(const void *buf, size_t n, off_t offset) override;

private:
  char buffer[0xff];
  size_t buffer_index;
};

} // namespace fs

#endif
