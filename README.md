# OS Experiments

My having fun making a 32-bit (for now) kernel and maybe some other stuff to go with it.

## Build Requirements
 * nasm
 * gcc toolchain for elf32

## Building on linux

Just run `make -s`. If you have qemu, you can run the script `qemu.sh` which will build the repo and start it with
qemu in curses mode.

## Debugging

Run `qemu.sh -s -S` in one terminal and `gdb.sh` in the other

## With lots of help from
 * [Writing a Simple Operating System — from Scratch](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
 * [OS Dev Wiki](https://wiki.osdev.org/Main_Page)

