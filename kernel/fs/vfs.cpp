#include <assert.h>
#include <kernel/fs/fs_device.h>
#include <kernel/fs/vfs.h>
#include <util/string.h>
using namespace fs;

vfs::vfs() {}

void vfs::mount_device(string path, rc<fs_device> device) {
  if (!fs_devices.get(device->id).has_value()) {
    fs_devices.set(device->id, device);
  }
  mount_entry entry{device};
  mounts.set(path, entry);
};

optional<rc<inode>> vfs::resolve_path(const char *path) {
  assert(path[0] == '/' && "Path names must be relative to root dir");

  // Get the mount dir of the given path
  char buf[0xff]; // the current path relative to the current mount
  size_t buf_i = 0;
  rc<fs_device> current = root_device;
  for (size_t i = 1; path[i]; i++) {
    if (path[i] == '/' || !path[i + 1]) {
      auto bs = string(buf);
      auto mount = mounts.get(bs);
      if (mount.has_value()) {
        current = mount.value().device;
        buf_i = 0;
      }
    }
    buf[buf_i++] = path[i];
    buf[buf_i] = 0;
  }

  return current->resolve_path(buf);
}
