#!/bin/bash

qemu-system-i386 $* \
  -display curses \
  -cdrom os.iso
