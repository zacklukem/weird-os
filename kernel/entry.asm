[bits 32]

global _start
extern __kernel_main__

_start:
  call __kernel_main__
