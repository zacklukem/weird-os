#include <kernel/fs/fifo.h>
#include <kernel/fs/fs.h>
#include <kernel/fs/udev.h>

using namespace fs;

udev::udev(dev_t id) : fs_device(id) {

  root_inode = make_inode_dirent_pair<directory_like>(
      "", util::nullopt, get_inode_id(), S_IRWXO | S_IRWXG | S_IRWXU);

  root_inode->device = id;
  root_inode->rdevice = id;
  root_dirent = root_inode->m_dirent;

  auto tty0 = make_inode_dirent_pair<fifo>("tty0", root_dirent, get_inode_id(),
                                           S_IRWXO | S_IRWXG | S_IRWXU);
  inode_cache.set(tty0->id, tty0);

  root_inode->add_child(tty0->m_dirent);
}

udev::~udev() {}

optional<rc<inode>> udev::get_inode(ino_t inode) {
  return inode_cache.get(inode);
}

optional<rc<inode>> udev::resolve_path(const char *path) {
  return util::nullopt;
}

util::list_iterator<rc<dirent>> udev::opendir(rc<inode> inode) {
  return inode->opendir();
}
