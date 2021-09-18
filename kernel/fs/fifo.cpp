#include <kernel/fs/fifo.h>
using namespace fs;

fifo::fifo(ino_t id, mode_t mode, rc<dirent> m_dirent)
    : inode(id, m_dirent, mode) {
  mode |= S_IFIFO;
}

fifo::~fifo() {}

ssize_t fifo::read(void *buf, size_t nbyte, off_t offset) {
  if (buffer_index < nbyte)
    return 0;
  memcpy(buf, buffer, nbyte);
  // shift buffer left
  memcpy(buffer, &buffer[nbyte], buffer_index);
  buffer_index -= nbyte;
  return nbyte;
}

ssize_t fifo::write(const void *buf, size_t n, off_t offset) {
  memcpy(&buffer[buffer_index], buf, n);
  buffer_index += n;
  return n;
}
