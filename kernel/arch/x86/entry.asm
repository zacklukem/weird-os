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
_sys_stack: