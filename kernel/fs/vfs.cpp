#include <assert.h>
#include <kernel/fs/fs_device.h>
#include <kernel/fs/vfs.h>
#include <util/string.h>
using namespace fs;

vfs::vfs() {}

void vfs::mount_device(const char *path, rc<fs_device> device) {
  auto mount_dirent = device->mount(path); // TODO: only get basename of his

  if (path[0] == '/' && strlen(path) == 1) {
    root_dirent = mount_dirent;
    mount_dirent->parent = util::nullopt;
  } else {
    assert(false && "no good");
  }

  // Place this dirent in the proper place
  // set parent dirent of mount_dirent
};

optional<rc<dirent>> vfs::resolve_path(const char *path) {
  assert(path[0] == '/' && "Path names must be relative to root dir");

  char buf[0xff];
  size_t i = 0;

  rc<dirent> current = root_dirent;

  for (size_t j = 1; path[j]; ++j) {
    if (path[j] == '/') {
      buf[i] = 0; // NULL terminate c string
      auto next = current->get_child(buf);
      if (next.has_value())
        current = next.value();
      else
        return util::nullopt;
      i = 0;
    } else {
      buf[i++] = path[j];
    }
  }

  buf[i] = 0; // NULL terminate c string
  return current->get_child(buf);
}
