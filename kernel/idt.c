#include <kernel/idt.h>
#include <kernel/kstl.h>

struct idt_entry {   // https://wiki.osdev.org/Interrupt_Descriptor_Table
  uint16_t offset_1; // offset bits 0..15
  uint16_t selector; // a code segment selector in GDT or LDT
  uint8_t zero;      // unused, set to 0
  uint8_t type_attr; // type and attributes, see below
  uint16_t offset_2; // offset bits 16..31
} __attribute__((packed));

_Static_assert(sizeof(struct idt_entry) == 8, "Packing error");

struct idt_desc {
  uint16_t size;    // size - 1
  uint32_t address; // idt_start
} __attribute__((packed));
_Static_assert(sizeof(struct idt_desc) == 6, "Packing error");

struct regs {
  unsigned int gs, fs, es, ds;
  unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
  unsigned int int_no, err_code;
  unsigned int eip, cs, eflags, useresp, ss;
};

struct idt_entry idt[0x100];

struct idt_desc idt_descriptor;

extern void asm_idt_load();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

#define INT_GATE 0xe
#define TRAP_GATE 0xf
#define TASK_GATE 0x5

unsigned char *exception_messages[32] = {
    "Division By Zero Exception",
    "Debug Exception",
    "Non Maskable Interrupt Exception",
    "Breakpoint Exception",
    "Into Detected Overflow Exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",
    "Double Fault Exception",
    "Coprocessor Segment Overrun Exception",
    "Bad TSS Exception",
    "Segment Not Present Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Exception",
    "Unknown Interrupt Exception",
    "Coprocessor Fault Exception",
    "Alignment Check Exception",
    "Machine Check Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

void init_idt_entry(uint32_t offset, uint16_t selector, uint8_t type,
                    struct idt_entry *entry) {
  entry->offset_1 = offset & 0xffff;
  entry->offset_2 = (offset & 0xffff0000) >> 16;
  entry->selector = selector;
  entry->type_attr = type;
  entry->zero = 0x0;
}

void init_idt() {

  for (int i = 0; i < 0xff; ++i) {
    init_idt_entry(0, 0x08, INT_GATE, &idt[i]);
  }

  init_idt_entry((uint32_t)isr0, 0x08, INT_GATE, &idt[0]);
  init_idt_entry((uint32_t)isr1, 0x08, INT_GATE, &idt[1]);
  init_idt_entry((uint32_t)isr2, 0x08, INT_GATE, &idt[2]);
  init_idt_entry((uint32_t)isr3, 0x08, INT_GATE, &idt[3]);
  init_idt_entry((uint32_t)isr4, 0x08, INT_GATE, &idt[4]);
  init_idt_entry((uint32_t)isr5, 0x08, INT_GATE, &idt[5]);
  init_idt_entry((uint32_t)isr6, 0x08, INT_GATE, &idt[6]);
  init_idt_entry((uint32_t)isr7, 0x08, INT_GATE, &idt[7]);
  init_idt_entry((uint32_t)isr8, 0x08, INT_GATE, &idt[8]);
  init_idt_entry((uint32_t)isr9, 0x08, INT_GATE, &idt[9]);
  init_idt_entry((uint32_t)isr10, 0x08, INT_GATE, &idt[10]);
  init_idt_entry((uint32_t)isr11, 0x08, INT_GATE, &idt[11]);
  init_idt_entry((uint32_t)isr12, 0x08, INT_GATE, &idt[12]);
  init_idt_entry((uint32_t)isr13, 0x08, INT_GATE, &idt[13]);
  init_idt_entry((uint32_t)isr14, 0x08, INT_GATE, &idt[14]);
  init_idt_entry((uint32_t)isr15, 0x08, INT_GATE, &idt[15]);
  init_idt_entry((uint32_t)isr16, 0x08, INT_GATE, &idt[16]);
  init_idt_entry((uint32_t)isr17, 0x08, INT_GATE, &idt[17]);
  init_idt_entry((uint32_t)isr18, 0x08, INT_GATE, &idt[18]);
  init_idt_entry((uint32_t)isr19, 0x08, INT_GATE, &idt[19]);
  init_idt_entry((uint32_t)isr20, 0x08, INT_GATE, &idt[20]);
  init_idt_entry((uint32_t)isr21, 0x08, INT_GATE, &idt[21]);
  init_idt_entry((uint32_t)isr22, 0x08, INT_GATE, &idt[22]);
  init_idt_entry((uint32_t)isr23, 0x08, INT_GATE, &idt[23]);
  init_idt_entry((uint32_t)isr24, 0x08, INT_GATE, &idt[24]);
  init_idt_entry((uint32_t)isr25, 0x08, INT_GATE, &idt[25]);
  init_idt_entry((uint32_t)isr26, 0x08, INT_GATE, &idt[26]);
  init_idt_entry((uint32_t)isr27, 0x08, INT_GATE, &idt[27]);
  init_idt_entry((uint32_t)isr28, 0x08, INT_GATE, &idt[28]);
  init_idt_entry((uint32_t)isr29, 0x08, INT_GATE, &idt[29]);
  init_idt_entry((uint32_t)isr30, 0x08, INT_GATE, &idt[30]);
  init_idt_entry((uint32_t)isr31, 0x08, INT_GATE, &idt[31]);

  idt_descriptor.size = (sizeof(struct idt_entry) * 0x100) - 1;
  idt_descriptor.address = (size_t)&idt;

  asm_idt_load();
}

void cpu_fault_handler(struct regs *r) {
  /* Is this a fault whose number is from 0 to 31? */
  if (r->int_no < 32) {
    /* Display the description for the Exception that occurred.
     *         *  In this tutorial, we will simply halt the system using an
     *                 *  infinite loop */
    printk(exception_messages[r->int_no]);
    printk(" Exception. System Halted!\n");
    while (1)
      ;
  }
}
