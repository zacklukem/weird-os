#ifndef KERNEL_FS_FLAGS_H
#define KERNEL_FS_FLAGS_H

// File mode masks
#define S_IRUSR 0400 ///< Read permission, owner.
#define S_IWUSR 0200 ///< Write permission, owner.
#define S_IXUSR 0100 ///< Execute/search permission, owner.
#define S_IRWXU                                                                \
  S_IRUSR | S_IWUSR | S_IXUSR ///< Read, write, execute/search by owner.

#define S_IRGRP 040 ///< Read permission, group.
#define S_IWGRP 020 ///< Write permission, group.
#define S_IXGRP 010 ///< Execute/search permission, group.
#define S_IRWXG                                                                \
  S_IRGRP | S_IWGRP | S_IXGRP // Read, write, execute/search by group.

#define S_IROTH 04 // Read permission, others.
#define S_IWOTH 02 // Write permission, others.
#define S_IXOTH 01 // Execute/search permission, others.
#define S_IRWXO                                                                \
  S_IROTH | S_IWOTH | S_IXOTH // Read, write, execute/search by others.

#define S_ISUID 04000 // Set-user-ID on execution.
#define S_ISGID 02000 // Set-group-ID on execution.
#define S_ISVTX 01000 // On directories, restricted deletion flag.

#define S_IFDIR 0040000  // Directory.
#define S_IFCHR 0020000  // Character device
#define S_IFBLK 0060000  // Block device.
#define S_IFREG 0100000  // Regular file.
#define S_IFIFO 0010000  // FIFO.
#define S_IFLNK 0120000  // Symbolic link.
#define S_IFSOCK 0140000 // Socket.

// Open flags
#define O_EXEC 0x1   ///< Exec only
#define O_RDONLY 0x2 ///< Read only
#define O_RDWR 0x4   ///< Read write only
#define O_SEARCH 0x8
#define O_WRONLY 0x10 ///< Write only
#define O_APPEND 0x20 ///< Append mode
#define O_CLOEXEC 0x40
#define O_CREAT 0x80      ///< TODO:Check the standard i have no clue
#define O_DIRECTORY 0x100 ///< Fail if not directory
#define O_DSYNC 0x200
#define O_EXCL 0x400
#define O_NOCTTY 0x800
#define O_NOFOLLOW 0x1000
#define O_NONBLOCK 0x2000
#define O_RSYNC 0x4000
#define O_SYNC 0x8000
#define O_TRUNC 0x10000
#define O_TTY_INIT 0x20000

#define S_ISBLK(m) ((m)&S_IFBLK == S_IFBLK) // Test for a block special file.
#define S_ISCHR(m)                                                             \
  ((m)&S_IFCHR == S_IFCHR) // Test for a character special file.
#define S_ISDIR(m) ((m)&S_IFDIR == S_IFDIR) // Test for a directory.
#define S_ISFIFO(m)                                                            \
  ((m)&S_IFIFO == S_IFIFO) // Test for a pipe or FIFO special file.
#define S_ISREG(m) ((m)&S_IFREG == S_IFREG)    // Test for a regular file.
#define S_ISLNK(m) ((m)&S_IFLNK == S_IFLNK)    // Test for a symbolic link.
#define S_ISSOCK(m) ((m)&S_IFSOCK == S_IFSOCK) // Test for a socket.

#endif // KERNEL_FS_FLAGS_H
