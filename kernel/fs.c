#include <assert.h>
#include <kernel/fs.h>

uint32_t read_node(struct fs_node *node, void *buf, size_t n, off_t offset) {
  assert(node && "Null node ptr");
  assert(node->read && "Null read ptr");
  return node->read(node, buf, n, offset);
}

uint32_t write_node(struct fs_node *node, const void *buf, size_t n,
                    off_t offset) {
  assert(node && "Null node ptr");
  assert(node->write && "Null write ptr");
  return node->write(node, buf, n, offset);
}

void open_node(struct fs_node *node, int oflag) {
  assert(node && "Null node ptr");
  assert(node->open && "Null open ptr");
  node->open(node, oflag);
}

void close_node(struct fs_node *node) {
  assert(node && "Null node ptr");
  assert(node->close && "Null close ptr");
  node->close(node);
}
struct dirent *readdir_node(struct fs_node *node, uint32_t index) {
  assert(node && "Null node ptr");
  assert(node->readdir && "Null readdir ptr");
  return node->readdir(node, index);
}
struct fs_node *finddir_node(struct fs_node *node, char *name) {
  assert(node && "Null node ptr");
  assert(node->finddir && "Null finddir ptr");
  return node->finddir(node, name);
}
