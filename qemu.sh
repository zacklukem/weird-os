#!/bin/bash

# make -s &&\
# qemu-system-i386 $* -display none -nographic -serial mon:stdio -kernel build/kernel.elf
qemu-system-i386 $* -display curses -kernel build/kernel.elf
