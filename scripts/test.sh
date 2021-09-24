#!/bin/bash

qemu-system-i386 $* \
  -display curses \
  -cdrom build/os.iso
