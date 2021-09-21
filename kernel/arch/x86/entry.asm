[bits 32]

; Flags constants
MULTIBOOT_PAGE_ALIGN  equ 0x1
MULTIBOOT_MEMORY_INFO  equ 0x2
MULTIBOOT_AOUT_KLUDGE  equ 0x10000

; Magic ~~~~~~pow~~~~
MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO ; | MULTIBOOT_AOUT_KLUDGE
; More magical voodoo
MULTIBOOT_CHECKSUM  equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

global start
extern __kernel_main__

; Linker script defined addresses
extern code_addr
extern bss_addr
extern end_addr
extern mb_header_val

align 4
mboot:

  ; Actually put the headers  here
  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd MULTIBOOT_CHECKSUM

  ; AOUT kludge - must be physical addresses. Make a note of these:
  ; The linker script fills in the data for these ones!
  ;dd mboot
  ;dd code_addr
  ;dd bss_addr
  ;dd end_addr
  ;dd start

start:
  mov [mb_header_val], ebx

;  lea ebx, [higher_half]
;  jmp ebx

; higher_half:
  ; set address of the stack
  mov esp, _sys_stack
  jmp stublet

stublet:

  call __kernel_main__ ; call the kernel main in start.c
  jmp $                ; jump forever

; extern void gdt_flush();
global gdt_flush
extern gp           ; the GDT descriptor in gdt.c
gdt_flush:
  lgdt [gp]         ; Load the GDT with the gdt in gdt.c
  mov ax, 0x10      ; set the segment registers to our data segment at 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:flush2   ; jump to code segment and initialze segment register
flush2:
  ret

%include "kernel/arch/x86/idt.s"
%include "kernel/arch/x86/isr.s"
%include "kernel/arch/x86/irq.s"

section .bss
  resb 8192 ; reserved for the stack
_sys_stack: ; Stack grows downward, so stack overflows when it hits the start
            ; of the bss section
