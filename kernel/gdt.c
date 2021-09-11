#include <kernel/gdt.h>
#include <stdint.h>

/**
 * GDT entry structure.
 *
 * Contains data describing the locations and attributes of memory
 */
struct gdt_entry {
  uint16_t limit_low;  ///< Low word of the limit
  uint16_t base_low;   ///< low word of the base
  uint8_t base_middle; ///< middle byte of the base
  uint8_t access;      ///< access flags
  uint8_t granularity; ///< limit and granularity flags
  uint8_t base_high;   ///< high bytes of the base
} __attribute__((packed));
// Sanity check to make sure the structure is properly packed
_Static_assert(sizeof(struct gdt_entry) == 8, "Structure improperly packed");

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
static struct gdt_entry gdt[3];

/**
 * The GDT descriptor to be passed to the lgdt instruction
 */
struct gdt_ptr gp;

/**
 * Implemented in assembly. Loads the gdt with the lgdt instruction and sets
 * the segment registers.
 */
extern void gdt_flush();

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

void gdt_install() {
  // Setup GDT descriptor
  gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
  gp.base = (size_t)&gdt;

  // NULL descriptor
  gdt_set_gate(0, 0, 0, 0, 0);

  // Code segment
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

  // Data segment
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

  // Call assembly flush
  gdt_flush();
}
