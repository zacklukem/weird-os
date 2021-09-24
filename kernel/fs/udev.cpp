#include <kernel/fs/fifo.h>
#include <kernel/fs/fs.h>
#include <kernel/fs/udev.h>

using namespace fs;

udev::udev(dev_t id) : fs_device(id) {

  root_dirent = util::make_rc<dirent>("", weak<inode>(), util::nullopt);

  root_inode = util::make_rc<directory_like>(root_dirent, get_inode_id(),
                                             S_IRWXO | S_IRWXG | S_IRWXU);

  root_dirent->m_inode = root_inode;
  root_dirent->inode_id = root_inode->id;

  // Set block device
  root_inode->device = id;
  root_inode->rdevice = id;
  // tty0 entry
  auto tty0 = create_tty(0);

  inode_cache.set(root_inode->id, root_inode);
}

udev::~udev() {}

optional<rc<inode>> udev::get_inode(ino_t inode) {
  return inode_cache.get(inode);
}

rc<dirent> udev::mount(const char *name) {
  root_dirent->ident = name;
  return root_dirent;
}

rc<inode> udev::create_tty(int num) {
  // TODO: strings mean this will clean later...
  char *name = (char *)kmalloc(10);
  memcpy(name, "tty", 3);
  itoa(num, &name[3], 10);
  auto tty_dirent = util::make_rc<dirent>(name, weak<inode>(), root_dirent);

  auto tty_inode = util::make_rc<fifo>(tty_dirent, get_inode_id(),
                                       S_IRWXO | S_IRWXG | S_IRWXU);

  tty_dirent->inode_id = tty_inode->id;
  tty_dirent->m_inode = tty_inode;

  root_dirent->add_child(tty_dirent);

  inode_cache.set(tty_inode->id, tty_inode);

  return tty_inode;
}
