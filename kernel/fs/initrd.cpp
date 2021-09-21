#include <kernel/fs/initrd.h>
#include <stdint.h>
#include <util/rc.h>

using namespace fs;
using util::rc;

uint32_t fs::initrd_location;
uint32_t fs::initrd_end;

class rdfs_inode : public inode {
public:
  rdfs_inode(rc<dirent> m_dirent, ino_t id, mode_t mode, char *data)
      : inode(m_dirent, id, mode), data(data) {}

  virtual ~rdfs_inode() override{};
  virtual ssize_t read(void *buf, size_t nbyte, off_t offset) override {
    memcpy(buf, data + offset, nbyte);
    return nbyte;
  };
  virtual ssize_t write(const void *buf, size_t n, off_t offset) override {
    return 0;
  };
  virtual const util::list_iterator<rc<dirent>> opendir() override {
    return util::list_iterator<rc<dirent>>{nullptr};
  };

private:
  char *data;
};

rc<dirent> initrd::cache(rc<dirent> parent, initrd_dirent &rd_dirent) {
  // Create inode etc.
  auto dir = util::make_rc<dirent>((const char *)rd_dirent.name,
                                   util::weak<inode>(), parent);

  auto &rd_inode = header->inodes[dir->inode_id];
  rc<inode> inode;
  if (S_ISDIR(rd_inode.mode)) {
    inode = util::make_rc<directory_like>(dir, rd_inode.id, rd_inode.mode);
  } else {
    // TODO: use the right data stuff here
    inode = util::make_rc<rdfs_inode>(dir, rd_inode.id, rd_inode.mode, nullptr);
  }

  inode->uid = rd_inode.uid;
  inode->gid = rd_inode.gid;

  dir->m_inode = inode;
  dir->inode_id = inode->id;

  inode_cache.set(inode->id, inode);

  if (parent.get() != nullptr)
    parent->add_child(dir);

  if (rd_dirent.sibling) {
    cache(parent, header->dirents[rd_dirent.sibling]);
  }

  if (rd_dirent.first_child) {
    cache(dir, header->dirents[rd_dirent.first_child]);
  }

  return dir;
}

initrd::initrd(dev_t id) : fs_device(id) {
  header = (initrd_header *)initrd_location;

  root_dirent = cache(rc<dirent>(), header->dirents[0]);
}

initrd::~initrd() {}

optional<rc<inode>> initrd::get_inode(ino_t inode) {
  return inode_cache.get(inode);
}
rc<dirent> initrd::mount(const char *name) {
  root_dirent->ident = name;
  return root_dirent;
}
