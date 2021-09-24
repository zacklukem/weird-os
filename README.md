#OS Experiments

My having fun making a 32-bit (for now) kernel and maybe some other stuff to go with it.

## Features
 * [x] Boot
 * [x] Screen output
 * [x] Keyboard input
 * [-] printf and other clib (a bit)
 * [x] Paging
 * [x] Dynamic memory
 * [x] Virtual file system
 * [ ] Driver API
 * [ ] multiprocessing
 * [ ] user mode

## Build Requirements

 * nasm
 * gcc toolchain for i386
 * genisoimage

## About

### Boot
The OS boots with grub legacy bootloader with the kernel loaded as an elf
binary and the initrd image loaded as multiboot module.

### IDT/IRQs
The OS has a minimal idt to handle cpu faults.  No syscalls yet (no user mode
haha).  The OS just has irqs for keyboard input.

### Memory Allocation
The OS uses a custom design for memory allocation. I didn't really research
established algorithms much so the implementation is pretty naive. The heap
is just a doubly linked list where each block header points to the next.
Each block header contains the size of the block and if it is used or not.
When a block is freed, it merges with any free neighbors to become a larger
free block.  To allocate memory, the list is traversed until the best
possible block is found.

### Initial Ram Disk
The initrd uses a custom read-only filesystem.  The filesystem consists of
a static 256 entry table of dirents, and a 256 entry table of inodes followed
by the file data.


### Virtual filesystem
The virtual filesystem is based on abstract dirent classes which are generated
by a filesystem driver and then the vfs recursively traverses the directory
tree to resolve a path.

## With lots of help from

 * [Writing a Simple Operating System — from Scratch](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
 * [OS Dev Wiki](https://wiki.osdev.org/Main_Page)
 * [Bran](http://www.osdever.net/bkerndev/Docs/whatsleft.htm)
 * [James](http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html)
