#ifndef KERNEL_FS_INITRD_H
#define KERNEL_FS_INITRD_H

#include <kernel/fs/fs.h>
#include <kernel/fs/fs_device.h>
#include <stdint.h>

namespace fs {

extern uint32_t initrd_location;
extern uint32_t initrd_end;

struct initrd_dirent {
  uint8_t name[0x88];
  uint8_t inode_id;
  uint8_t parent;      ///< index of parent or zero if is root node
  uint8_t first_child; ///< index of first child 0 if none
  uint8_t sibling;     ///< index of next sibling or 0 if none
};

struct initrd_inode {
  uint8_t id;
  uint32_t mode;
  uint32_t uid;
  uint32_t gid;
  uint32_t size;
  uint32_t
      data; ///< the offset in bytes from the start of the initrd to the data
};

struct initrd_header {
  uint8_t num_dirent;
  initrd_dirent dirents[0xff];
  initrd_inode inodes[0xff];
} __attribute__((packed));

class initrd : public fs_device {
public:
  initrd(dev_t id);
  virtual ~initrd() override;

  virtual optional<rc<inode>> get_inode(ino_t inode) override;
  virtual rc<dirent> mount(const char *name) override;

private:
  rc<dirent> root_dirent;
  rc<directory_like> root_inode;
  initrd_header *header;
  rc<dirent> cache(rc<dirent> parent, initrd_dirent &dirent);
  util::hashmap<ino_t, rc<inode>> inode_cache;
};

}; // namespace fs

#endif
