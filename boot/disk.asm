[bits 16]

global disk_load
global DISK_ERR_MSG_C
global DISK_ERR_MSG_NE

extern puts
extern put_hex

; load DH sectors to ES:BX from drive DL
disk_load:
  push dx

  mov ah, 0x02 ; disk load bios function
  mov al, dh ; n sectors
  mov ch, 0 ; cylinder 0
  mov dh, 0 ; head 0
  mov cl, 0x02 ; second sector (after boot sector)
  int 0x13  ; call interupt
  jc .disk_err_c

  pop dx
  cmp dh, al
  jne .disk_err_ne
  ret

.disk_err_c:
  mov bx, DISK_ERR_MSG_C
  call puts
  mov bx, ax
  call put_hex
  jmp $
.disk_err_ne:
  mov bx, DISK_ERR_MSG_NE
  call puts
  mov bx, ax
  call put_hex
  jmp $

; constants
;  SECTION .data

DISK_ERR_MSG_C db "Disk read err c", 0
DISK_ERR_MSG_NE db "Disk read err ne", 0

