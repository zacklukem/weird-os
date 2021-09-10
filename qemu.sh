#!/bin/bash

make -s &&\
qemu-system-i386 $* -display curses -kernel build/kernel.elf
