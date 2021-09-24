

;
Macro to make creating irq tempates cleaner %
    macro                           exp_irq 1

    global irq %
    1

    irq %
    1 : cli push byte 0 push byte(% 1 + 32) jmp irq_common_stub

        %
        endmacro

        exp_irq 0 exp_irq 1 exp_irq 2 exp_irq 3 exp_irq 4 exp_irq 5 exp_irq
        6 exp_irq 7 exp_irq 8 exp_irq 9 exp_irq 10 exp_irq 11 exp_irq 12 exp_irq
        13 exp_irq 14 exp_irq 15

    ;
Handler function defined in irq.c extern irq_handler

    ;
Pushes registers to stack to call the irq handler irq_common_stub
    : pusha push ds push es push fs push gs mov   ax,
      0x10 mov                                    ds,
      ax mov                                      es,
      ax mov                                      fs,
      ax mov                                      gs,
      ax mov                                      eax,
      esp push eax mov                            eax,
      irq_handler call eax pop eax pop gs pop fs pop es pop ds popa add esp,
      8 iret
