#!/bin/bash

make -s &&\
qemu-system-i386 $* -display curses -drive format=raw,file=build/disk.img
