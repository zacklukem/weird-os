[bits 32]

    global asm_idt_load extern idt_descriptor

        asm_idt_load : lidt[idt_descriptor] ret
