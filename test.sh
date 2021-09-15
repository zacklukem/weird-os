#!/bin/bash

qemu-system-i386 $* -display curses -serial file:file.txt -kernel build/test_kernel.elf
