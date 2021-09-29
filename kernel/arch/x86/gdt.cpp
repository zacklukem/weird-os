#include <arch/x86/gdt.h>
#include <stdint.h>
#include <string.h>

/**
 * GDT entry structure.
 *
 * Contains data describing the locations and attributes of memory
 */
struct gdt_entry_t {
  uint16_t limit_low;  ///< Low word of the limit
  uint16_t base_low;   ///< low word of the base
  uint8_t base_middle; ///< middle byte of the base
  uint8_t access;      ///< access flags
  uint8_t granularity; ///< limit and granularity flags
  uint8_t base_high;   ///< high bytes of the base
} __attribute__((packed));
// Sanity check to make sure the structure is properly packed
static_assert(sizeof(gdt_entry_t) == 8, "Structure improperly packed");

// A struct describing a Task State Segment.
struct tss_entry_t {
  uint32_t prev_tss; // The previous TSS - if we used hardware task switching
                     // this would form a linked list.
  uint32_t esp0;     // The stack pointer to load when we change to kernel mode.
  uint32_t ss0;      // The stack segment to load when we change to kernel mode.
  uint32_t esp1;     // Unused...
  uint32_t ss1;
  uint32_t esp2;
  uint32_t ss2;
  uint32_t cr3;
  uint32_t eip;
  uint32_t eflags;
  uint32_t eax;
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebx;
  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;
  uint32_t es;  // The value to load into ES when we change to kernel mode.
  uint32_t cs;  // The value to load into CS when we change to kernel mode.
  uint32_t ss;  // The value to load into SS when we change to kernel mode.
  uint32_t ds;  // The value to load into DS when we change to kernel mode.
  uint32_t fs;  // The value to load into FS when we change to kernel mode.
  uint32_t gs;  // The value to load into GS when we change to kernel mode.
  uint32_t ldt; // Unused...
  uint32_t trap;
  uint32_t iomap_base;
} __attribute__((packed));

/**
 * GDT descriptor.  Limit describes the size of the GDT minus one and the base
 * is a pointer to the GDT.
 */
struct gdt_ptr {
  uint16_t limit; ///< Limit of the gdt
  uint32_t base;  ///< base of the gdt
} __attribute__((packed));

/**
 * The GDT itself, contining one null entry, the code segment entry, and the
 * data segment entry
 */
static gdt_entry_t gdt[6];

/**
 * The GDT descriptor to be passed to the lgdt instruction
 */
struct gdt_ptr gp;

/**
 * Implemented in assembly. Loads the gdt with the lgdt instruction and sets
 * the segment registers.
 */
extern "C" void gdt_flush();
extern "C" void tss_flush();

static tss_entry_t tss_entry;

/**
 * Initializes a gdt segment by spliting up the base and limit, then setting
 * flags
 */
static void gdt_set_gate(int num, uint32_t base, uint32_t limit,
                         uint32_t access, uint8_t gran) {
  // set base
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;

  // set limits
  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F);

  // set access flags
  gdt[num].granularity |= (gran & 0xF0);
  gdt[num].access = access;
}

static void write_tss(int32_t num, uint16_t ss0, uint32_t esp0) {
  // Firstly, let's compute the base and limit of our entry into the GDT.
  uint32_t base = (uint32_t)&tss_entry;
  uint32_t limit = base + sizeof(tss_entry);

  //  uint32_t cr3;
  // __asm__ __volatile__("mov %%cr3, %%eax" : "=a"(cr3));

  // Now, add our TSS descriptor's address to the GDT.
  gdt_set_gate(num, base, limit, 0xE9, 0x00);

  // Ensure the descriptor is initially zero.
  memset(&tss_entry, 0, sizeof(tss_entry));

  tss_entry.ss0 = ss0;   // Set the kernel stack segment.
  tss_entry.esp0 = esp0; // Set the kernel stack pointer.
  // tss_entry.cr3 = cr3;

  // Here we set the cs, ss, ds, es, fs and gs entries in the TSS. These specify
  // what segments should be loaded when the processor switches to kernel mode.
  // Therefore they are just our normal kernel code/data segments - 0x08 and
  // 0x10 respectively, but with the last two bits set, making 0x0b and 0x13.
  // The setting of these bits sets the RPL (requested privilege level) to 3,
  // meaning that this TSS can be used to switch to kernel mode from ring 3.
  tss_entry.cs = 0x0b;
  tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs =
      0x13;

  tss_entry.iomap_base = sizeof(tss_entry_t);
}

#define RW 0x2
#define DC 0x4
#define EXECUTABLE 0x8
#define NOT_SYS 0x10
#define PRVL_0 0x0
#define PRVL_1 0x20
#define PRVL_2 0x40
#define PRVL_3 0x60
#define PRESENT 0x80

void gdt_install() {
  // Setup GDT descriptor
  gp.limit = (sizeof(gdt_entry_t) * 6) - 1;
  gp.base = (size_t)&gdt;

  // NULL descriptor
  gdt_set_gate(0, 0, 0, 0, 0);

  // Kernel Code segment (0x8)
  gdt_set_gate(1, 0, 0xFFFFFFFF, PRESENT | PRVL_0 | NOT_SYS | RW | EXECUTABLE,
               0xCF);

  // Kernel Data segment (0x10)
  gdt_set_gate(2, 0, 0xFFFFFFFF, PRESENT | PRVL_0 | NOT_SYS | RW, 0xCF);

  // User Code segment (0x18)
  gdt_set_gate(3, 0, 0xFFFFFFFF, PRESENT | PRVL_3 | NOT_SYS | RW | EXECUTABLE,
               0xCF);

  // User Data segment (0x20)
  gdt_set_gate(4, 0, 0xFFFFFFFF, PRESENT | PRVL_3 | NOT_SYS | RW, 0xCF);

  write_tss(5, 0x10, 0x0);

  // Call assembly flush
  gdt_flush();
  tss_flush();
}
