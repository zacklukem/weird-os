%macro exp_isr 2

global isr%1

isr%1:
  cli
%if %2 == 0
  push byte 0
%endif
  push byte %1
  jmp isr_common

%endmacro

exp_isr 0, 0
exp_isr 1, 0
exp_isr 2, 0
exp_isr 3, 0
exp_isr 4, 0
exp_isr 5, 0
exp_isr 6, 0
exp_isr 7, 0
exp_isr 8, 1
exp_isr 9, 0
exp_isr 10, 1
exp_isr 11, 1
exp_isr 12, 1
exp_isr 13, 1
exp_isr 14, 1
exp_isr 15, 0
exp_isr 16, 0
exp_isr 17, 0
exp_isr 18, 0
exp_isr 19, 0
exp_isr 20, 0
exp_isr 21, 0
exp_isr 22, 0
exp_isr 23, 0
exp_isr 24, 0
exp_isr 25, 0
exp_isr 26, 0
exp_isr 27, 0
exp_isr 28, 0
exp_isr 29, 0
exp_isr 30, 0
exp_isr 31, 0

extern syscall_handler
global isr80

isr80:
  push byte 0
  push byte 0x80

  pusha
  push ds
  push es
  push fs
  push gs
  mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, esp   ; Push us the stack
  cld
  push eax
  mov eax, syscall_handler
  call eax       ; A special call, preserves the 'eip' register
  pop eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8     ; Cleans up the pushed error code and pushed ISR number
  iret


extern cpu_fault_handler

isr_common:
  pusha
  push ds
  push es
  push fs
  push gs
  mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, esp   ; Push us the stack
  cld
  push eax
  mov eax, cpu_fault_handler
  call eax       ; A special call, preserves the 'eip' register
  pop eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8     ; Cleans up the pushed error code and pushed ISR number
  iret
