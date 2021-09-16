#!/bin/bash

qemu-system-i386 $* -display curses -kernel build/test_kernel.elf
