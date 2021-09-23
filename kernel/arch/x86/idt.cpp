#include <arch/x86/idt.h>
#include <arch/x86/regs.h>
#include <kernel/printk.h>
#include <stdio.h>
#include <string.h>

/**
 * An entry in the IDT table
 */
struct idt_entry {   // https://wiki.osdev.org/Interrupt_Descriptor_Table
  uint16_t offset_1; ///< offset bits 0..15
  uint16_t selector; ///< a code segment selector in GDT or LDT
  uint8_t zero;      ///< unused, set to 0
  uint8_t type_attr; ///< type and attributes, see below
  uint16_t offset_2; ///< offset bits 16..31
} __attribute__((packed));

static_assert(sizeof(struct idt_entry) == 8, "Packing error");

/**
 * The IDT descriptor that is passed to the lidt instruction in order to
 * activate the IDT.
 */
struct idt_desc {
  uint16_t size;    ///< size - 1
  uint32_t address; ///< idt_start
} __attribute__((packed));
static_assert(sizeof(struct idt_desc) == 6, "Packing error");

/**
 * The IDT
 */
static struct idt_entry idt[256];

/**
 * The IDT descriptor passed to the lidt intruction
 */
struct idt_desc idt_descriptor;

/**
 * Asm function implemented in idt.s which loads the idt from the idt_descriptor
 * with the lidt instruction
 */
extern "C" void asm_idt_load();

// ISR's 0-31 (exceptions)
extern "C" {
void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// ISR 80 (syscall)
void isr80();
}

// Set enable bit
#define ENABLED 0x80

// Set int gate bits
#define INT_GATE 0x0e
//#define TRAP_GATE 0xf
//#define TASK_GATE 0x5

/**
 * The list of exception messeges for isr's 0-31
 */
static const char *exception_messages[32] = {
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

/**
 * Initialize an IDT entry with the given values
 */
static void init_idt_entry(uint32_t offset, uint16_t selector, uint8_t type,
                           struct idt_entry *entry) {
  entry->offset_1 = offset & 0xffff;
  entry->offset_2 = (offset >> 16) & 0xffff;
  entry->selector = selector;
  entry->type_attr = type;
  entry->zero = 0x0;
}

/**
 * Set an IDT gate (called externally)
 */
void idt_set_gate(uint32_t offset, uint16_t selector, uint8_t type,
                  uint8_t index) {
  init_idt_entry(offset, selector, type, &idt[index]);
}

/**
 * Initialize the IDT
 */
void init_idt() {

  // Fill the IDT with disabled interupts
  for (int i = 0; i < 0xff; ++i) {
    init_idt_entry(0, 0x08, INT_GATE, &idt[i]);
  }

  // Initialize the idt descriptor
  idt_descriptor.size = (sizeof(struct idt_entry) * 256) - 1;
  idt_descriptor.address = (size_t)&idt;

  // Initialize the IDT exceptions
  init_idt_entry((uint32_t)isr0, 0x08, ENABLED | INT_GATE, &idt[0]);
  init_idt_entry((uint32_t)isr1, 0x08, ENABLED | INT_GATE, &idt[1]);
  init_idt_entry((uint32_t)isr2, 0x08, ENABLED | INT_GATE, &idt[2]);
  init_idt_entry((uint32_t)isr3, 0x08, ENABLED | INT_GATE, &idt[3]);
  init_idt_entry((uint32_t)isr4, 0x08, ENABLED | INT_GATE, &idt[4]);
  init_idt_entry((uint32_t)isr5, 0x08, ENABLED | INT_GATE, &idt[5]);
  init_idt_entry((uint32_t)isr6, 0x08, ENABLED | INT_GATE, &idt[6]);
  init_idt_entry((uint32_t)isr7, 0x08, ENABLED | INT_GATE, &idt[7]);
  init_idt_entry((uint32_t)isr8, 0x08, ENABLED | INT_GATE, &idt[8]);
  init_idt_entry((uint32_t)isr9, 0x08, ENABLED | INT_GATE, &idt[9]);
  init_idt_entry((uint32_t)isr10, 0x08, ENABLED | INT_GATE, &idt[10]);
  init_idt_entry((uint32_t)isr11, 0x08, ENABLED | INT_GATE, &idt[11]);
  init_idt_entry((uint32_t)isr12, 0x08, ENABLED | INT_GATE, &idt[12]);
  init_idt_entry((uint32_t)isr13, 0x08, ENABLED | INT_GATE, &idt[13]);
  init_idt_entry((uint32_t)isr14, 0x08, ENABLED | INT_GATE, &idt[14]);
  init_idt_entry((uint32_t)isr15, 0x08, ENABLED | INT_GATE, &idt[15]);
  init_idt_entry((uint32_t)isr16, 0x08, ENABLED | INT_GATE, &idt[16]);
  init_idt_entry((uint32_t)isr17, 0x08, ENABLED | INT_GATE, &idt[17]);
  init_idt_entry((uint32_t)isr18, 0x08, ENABLED | INT_GATE, &idt[18]);
  init_idt_entry((uint32_t)isr19, 0x08, ENABLED | INT_GATE, &idt[19]);
  init_idt_entry((uint32_t)isr20, 0x08, ENABLED | INT_GATE, &idt[20]);
  init_idt_entry((uint32_t)isr21, 0x08, ENABLED | INT_GATE, &idt[21]);
  init_idt_entry((uint32_t)isr22, 0x08, ENABLED | INT_GATE, &idt[22]);
  init_idt_entry((uint32_t)isr23, 0x08, ENABLED | INT_GATE, &idt[23]);
  init_idt_entry((uint32_t)isr24, 0x08, ENABLED | INT_GATE, &idt[24]);
  init_idt_entry((uint32_t)isr25, 0x08, ENABLED | INT_GATE, &idt[25]);
  init_idt_entry((uint32_t)isr26, 0x08, ENABLED | INT_GATE, &idt[26]);
  init_idt_entry((uint32_t)isr27, 0x08, ENABLED | INT_GATE, &idt[27]);
  init_idt_entry((uint32_t)isr28, 0x08, ENABLED | INT_GATE, &idt[28]);
  init_idt_entry((uint32_t)isr29, 0x08, ENABLED | INT_GATE, &idt[29]);
  init_idt_entry((uint32_t)isr30, 0x08, ENABLED | INT_GATE, &idt[30]);
  init_idt_entry((uint32_t)isr31, 0x08, ENABLED | INT_GATE, &idt[31]);

  // Syscall interupt
  init_idt_entry((uint32_t)isr80, 0x08, ENABLED | INT_GATE, &idt[0x80]);

  // Load the idt with the lidt instruction
  asm_idt_load();
}

/**
 * Handle syscalls (called from assembly isr)
 */
extern "C" void syscall_handler(int eax) {
  char data[16];
  itoa(eax, data, 10);
  printk("Handle syscall: ");
  printk(data);
  printk("\n");
}

/**
 * Handle cpu exceptions
 */
extern "C" void cpu_fault_handler(struct regs *r) {
  if (r->int_no == 14) { // page fault
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    __asm__ __volatile__("mov %%cr2, %0" : "=r"(faulting_address));

    // The error code gives us details of what happened.
    int present = !(r->err_code & 0x1); // Page not present
    int rw = r->err_code & 0x2;         // Write operation?
    int us = r->err_code & 0x4;         // Processor was in user-mode?
    int reserved =
        r->err_code & 0x8; // Overwritten CPU-reserved bits of page entry?
    // int id = r->err_code & 0x10; // Caused by an instruction fetch?

    // Output an error message.
    printk("Page fault! ( ");
    if (present) {
      printk("present ");
    }
    if (rw) {
      printk("read-only ");
    }
    if (us) {
      printk("user-mode ");
    }
    if (reserved) {
      printk("reserved ");
    }
    printf(") at 0x%x\n", faulting_address);
  }
  if (r->int_no < 32) {
    printk(exception_messages[r->int_no]);
    printk(" Exception. System Halted!\n");
    while (1)
      __asm__("hlt" ::);
  }
}
