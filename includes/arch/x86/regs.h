#ifndef ARCH_X86_REGS_H
#define ARCH_X86_REGS_H

struct regs {
  unsigned int gs, fs, es, ds;
  unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
  unsigned int int_no, err_code;
  unsigned int eip, cs, eflags, useresp, ss;
};

#endif // ARCH_X86_REGS_H
