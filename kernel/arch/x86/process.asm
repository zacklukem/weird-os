[bits 32]

global do_switch

; void do_switch(uint32_t *old_stack, uint32_t new_stack, uint32_t page);
do_switch:
  cli
  pusha

  mov eax, [esp + (8 + 1) * 4] ; old_stack
  mov [eax], esp ; store stack address (including pushed regs)

  mov eax, [esp + (8 + 2) * 4] ; new_stack
  mov ebx, [esp + (8 + 3) * 4] ; page
  mov esp, eax ; set new stack
  mov cr0, ebx ; set new page directory

  popa ; Load all the new registers into the cpu
  sti ; should this be above popa?
  ret

