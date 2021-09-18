#include <kernel/fs/fs.h>
#include <kernel/fs/udev.h>

using namespace fs;

udev::udev(dev_t id) : fs_device(id) {
  root_dirent = util::make_rc<dirent>("", weak<inode>(), util::nullopt);
  root_inode = util::make_rc<directory_like>(
      get_inode_id(), S_IRWXG | S_IRWXO | S_IRWXU, root_dirent);
  root_dirent->m_inode = root_inode;
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
