#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct initrd_dirent {
  uint8_t name[0x88];
  uint8_t inode_id;
  uint8_t parent;      ///< index of parent or zero if is root node
  uint8_t first_child; ///< index of first child 0 if none
  uint8_t sibling;     ///< index of next sibling or 0 if none
} __attribute__((packed));

struct initrd_inode {
  uint8_t id;
  uint32_t mode;
  uint32_t uid;
  uint32_t gid;
  uint32_t size;
  uint32_t
      data; ///< the offset in bytes from the start of the initrd to the data
} __attribute__((packed));

struct initrd_header {
  uint8_t num_dirent;
  struct initrd_dirent dirents[0xff];
  struct initrd_inode inodes[0xff];
} __attribute__((packed));

struct initrd_header *out;
uint8_t id = 0;

uint8_t convert_dirent(struct stat *stat, const char *name, uint8_t parent) {
  uint8_t cur_id = id++;

  memcpy(out->dirents[cur_id].name, name, strlen(name) + 1);

  out->dirents[cur_id].inode_id = cur_id;

  out->inodes[cur_id].id = cur_id;
  out->inodes[cur_id].mode = stat->st_mode;
  out->inodes[cur_id].uid = stat->st_uid;
  out->inodes[cur_id].gid = stat->st_gid;
  if (S_ISDIR(stat->st_mode))
    out->inodes[cur_id].size = 0;
  else
    out->inodes[cur_id].size = stat->st_size;
  out->inodes[cur_id].data = 12341234; // TODO: offset

  return cur_id;
}

uint8_t traverse_dir(const char *name, uint8_t parent) {
  DIR *dir = opendir(name);

  size_t len = strlen(name);

  uint8_t last = 0;

  for (struct dirent *current = readdir(dir); current; current = readdir(dir)) {
    if (strcmp(current->d_name, ".") == 0 || strcmp(current->d_name, "..") == 0)
      continue;

    char *full_path = (char *)malloc(len + 0xff);
    memcpy(full_path, name, len);
    full_path[len] = '/';
    memcpy(&full_path[len + 1], current->d_name, 0xff);

    // The file descriptor for the current file
    int current_fd = open(full_path, O_RDONLY);
    struct stat stat_buf;
    fstat(current_fd, &stat_buf);

    uint8_t my_dirent_id = convert_dirent(&stat_buf, current->d_name, 0);
    struct initrd_dirent *my_dirent = &out->dirents[my_dirent_id];
    my_dirent->parent = parent;
    my_dirent->sibling = last;
    last = my_dirent_id;

    if (S_ISDIR(stat_buf.st_mode)) {
      my_dirent->first_child = traverse_dir(full_path, my_dirent_id);
    } else {
      my_dirent->first_child = 0;
    }

    // printf("%4d %-20s %-7o %-6lu %-3u %-3u\n", my_dirent_id, full_path,
    //        stat_buf.st_mode, stat_buf.st_size, my_dirent->parent,
    //        my_dirent->sibling);

    close(current_fd);
    free(full_path);
  }

  closedir(dir);

  return last;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("usage: makefs <input_directory> <output_file>\n");
    return 1;
  }

  int current_fd = open(argv[1], O_RDONLY);
  if (current_fd == -1) {
    printf("Invalid input directory\n");
    return 1;
  }

  FILE *output_file = fopen(argv[2], "w");
  if (output_file == NULL) {
    printf("Invalid output file\n");
    return 1;
  }

  out = malloc(sizeof(struct initrd_header));
  memset(out, 0, sizeof(struct initrd_header));

  // printf("%4s %-20s %-7s %-6s %-3s %-3s\n", "ID", "PATH", "MODE", "SIZE",
  // "PRT",
  //        "SBL");

  struct stat stat_buf;
  fstat(current_fd, &stat_buf);

  int dir_id = convert_dirent(&stat_buf, "/", 0);
  struct initrd_dirent *dir = &out->dirents[dir_id];
  assert(dir_id == 0 && "Root dir must be zero");

  // printf("%4d %-20s %-7o %-6lu\n", dir_id, argv[1], stat_buf.st_mode,
  //       stat_buf.st_size);

  dir->first_child = traverse_dir(argv[1], dir_id);
  dir->sibling = 0;
  out->inodes[dir_id].uid = 0;
  out->inodes[dir_id].gid = 0;
  out->inodes[dir_id].mode = 0777;

  out->num_dirent = id;
  // LOAD TO FILE
  fwrite(out, sizeof(struct initrd_header), 1, output_file);

  free(out);
  close(current_fd);
  fclose(output_file);

  return 0;
}
