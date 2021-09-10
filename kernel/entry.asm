[bits 32]

global start
extern __kernel_main__

start:
  mov esp, _sys_stack
  jmp stublet

align 4
mboot:
  MULTIBOOT_PAGE_ALIGN  equ 1<<0
  MULTIBOOT_MEMORY_INFO  equ 1<<1
  MULTIBOOT_AOUT_KLUDGE  equ 1<<16
  MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
  MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
  MULTIBOOT_CHECKSUM  equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
  extern code, bss, end

  ; This is the GRUB Multiboot header. A boot signature
  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM
  ; AOUT kludge - must be physical addresses. Make a note of these:
  ; The linker script fills in the data for these ones!
  dd mboot
  dd code
  dd bss
  dd end
  dd start

; This is an endless loop here. Make a note of this: Later on, we
; will insert an 'extern _main', followed by 'call _main', right
; before the 'jmp $'.
stublet:
  call __kernel_main__

  jmp $


; Shortly we will add code for loading the GDT right here!
; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();'
global gdt_flush     ; Allows the C code to link to this
extern gp            ; Says that '_gp' is in another file
gdt_flush:
  lgdt [gp]        ; Load the GDT with our '_gp' which is a special pointer
  mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
  ret               ; Returns back to the C code!

; In just a few pages in this tutorial, we will add our Interrupt
; Service Routines (ISRs) right here!
%include "kernel/idt.s"
%include "kernel/isr.s"
%include "kernel/irq.s"

; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '_sys_stack'
SECTION .bss
  resb 8192               ; This reserves 8KBytes of memory here
_sys_stack:
