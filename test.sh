#!/bin/bash

qemu-system-i386 $* -nographic -kernel build/test_kernel.elf
