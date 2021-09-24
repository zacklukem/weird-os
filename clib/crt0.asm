[bits 32]

global _start
extern main

_start:
  ; TODO: do some things here
  call main
  jmp $
  ; TODO: put stuff into this to exit
  int 0x80
