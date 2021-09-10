#!/bin/bash

gdb build/kernel.elf \
  -ex 'target remote localhost:1234' \
  -ex 'layout src' \
  -ex 'layout regs' \
