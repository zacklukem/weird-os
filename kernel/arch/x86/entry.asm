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

KERNEL_VIRTUAL_BASE equ 0xc0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)

global start
global boot_page_directory
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

section .data
align 0x1000
boot_page_directory:
  dd 0x00000083
  dd 0x00400083
  dd 0x00800083
  dd 0x00c00083
  times (KERNEL_PAGE_NUMBER - 4) dd 0
  dd 0x00000083
  dd 0x00400083
  dd 0x00800083
  dd 0x00c00083
  times (1024 - KERNEL_PAGE_NUMBER) dd 0

section .text
start:
  mov ecx, (boot_page_directory - KERNEL_VIRTUAL_BASE)
  mov cr3, ecx                                        ; Load Page Directory Base Register.

  mov ecx, cr4
  or ecx, 0x00000010                          ; Set PSE bit in CR4 to enable 4MB pages.
  mov cr4, ecx

  mov ecx, cr0
  or ecx, 0x80000000                          ; Set PG bit in CR0 to enable paging.
  mov cr0, ecx

  lea ecx, [higher_half]
  jmp ecx

higher_half:
  ; Unmap the identity-mapped first 4MB of physical address space. It should not be needed
	; anymore.
	mov dword [boot_page_directory], 0
	mov dword [boot_page_directory + 4], 0
	mov dword [boot_page_directory + 8], 0
	mov dword [boot_page_directory + 12], 0
	invlpg [0]

  ; setup stack
  mov esp, _sys_stack

  add ebx, KERNEL_VIRTUAL_BASE
  mov [mb_header_val], ebx
  ; set address of the stack

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

global tss_flush
tss_flush:
   mov ax, 0x2B      ; Load the index of our TSS structure - The index is
                     ; 0x28, as it is the 5th selector and each is 8 bytes
                     ; long, but we set the bottom two bits (making 0x2B)
                     ; so that it has an RPL of 3, not zero.
   ltr ax            ; Load 0x2B into the task state register.
   ret

%include "arch/x86/idt.s"
%include "arch/x86/isr.s"
%include "arch/x86/irq.s"

section .bss
  resb 8192 ; reserved for the stack
_sys_stack: ; Stack grows downward, so stack overflows when it hits the start
            ; of the bss section
