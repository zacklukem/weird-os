[bits 16]

global puts
global put_hex

; put string
; bx = address of string
puts:
  pusha

.loop:
  mov cl, [bx] ; cl = *bx
  cmp cl, 0
  je .loop_end

  mov ah, 0x0e
  mov al, cl
  int 0x10

  inc bx
  jmp .loop
.loop_end:

  popa
  ret

; put_hex
; bx = word to print
put_hex:
  pusha

  mov bp, sp

  sub sp, 7 ; allocate 7 bytes for string (one for terminator)

  mov byte [bp], '0'
  mov byte [bp + 1], 'x'

  mov ax, 0
.loop:
  cmp ax, 4
  jge .loop_end

  mov cx, bx
  and cx, 0xf000 ; first 4 bits
  shr cx, 12     ; move first 4 bits to end

  cmp cx, 10
  jge .if_char
; if_digit
  add cx, '0'
  jmp .then
.if_char:
  add cx, ('a' - 10)
.then:
  mov si, ax
  mov byte [bp + si + 2], cl ; add char

  shl bx, 4 ; shift 4 bits right
  inc ax
  jmp .loop
.loop_end:
  mov byte [bp + 6], 0 ; add terminator

  mov bx, bp
  call puts

  ret
