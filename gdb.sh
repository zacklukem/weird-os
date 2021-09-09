#!/bin/bash

gdb build/disk.img \
  -ex 'target remote localhost:1234' \
  -ex 'layout src' \
  -ex 'layout regs' \
