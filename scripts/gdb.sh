#!/bin/bash

gdb kernel/build/test_kernel.elf \
  -ex 'target remote localhost:1234' \
  -ex 'b kernel/assertk.cpp:19' 
