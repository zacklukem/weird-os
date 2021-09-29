[bits 32]

global enter_usermode

enter_usermode:
  cli
  mov ax, 0x23
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  push 0x23
  push esp
  pushfd
  push 0x1b
  push .a

  iretd
.a:
  add esp, 4
  ret
