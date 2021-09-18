#!/bin/bash

gdb build/test_kernel.elf \
  -ex 'target remote localhost:1234' \
