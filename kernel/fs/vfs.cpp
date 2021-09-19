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

  string p(path);

  auto s_path = p.split('/');

  string current = "";

  for (auto &ent : s_path) {
    current += "/";
    current += ent;
    auto mount = mounts.get(current);
    if (mount.has_value()) {
      // gota find a way to give device dirents their proper parentage
    }
  }

  return root_device->resolve_path("");
}
