; [org 0x7c00]
[bits 16]

global _start

extern disk_load
extern put_hex
extern puts
extern gdt_descriptor
extern CODE_SEG
extern DATA_SEG
extern check_a20

_start:

  mov [boot_drive], dl

  mov bp, 0x8000
  mov sp, bp

  mov bx, 0x1000
  mov dh, 15
  mov dl, [boot_drive]
  call disk_load

  mov bx, hello_world
  call puts

  call check_a20
  cmp ax, 1
  je .a20_enabled
  mov bx, disabled
  jmp .a20_end
.a20_enabled:
  mov bx, enabled
.a20_end:
  call puts

  call switch_to_pm

  jmp $

boot_drive db 0

hello_world db "Loaded disk successfully", 0
enabled db "; a20 is enabled", 0
disabled db "; a20 is disabled", 0

switch_to_pm:
  cli
  lgdt [gdt_descriptor]
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax

  jmp CODE_SEG:init_pm

[bits 32]

init_pm:
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x90000
  mov esp, ebp

  jmp 0x1000

;  SECTION .data


;times 510-($-$$) db 0
;
;dw 0xaa55
;
;times 256 dw 0xabcd
;times 256 dw 0xdcba
