#!/bin/bash

gdb build/test_kernel.elf \
  -ex 'target remote localhost:1234' \
  -ex 'b kernel/arch/x86/idt.cpp:254' \
  -ex 'b kernel/assertk.cpp:19' 
