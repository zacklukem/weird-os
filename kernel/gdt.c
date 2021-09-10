#include <kernel/kstl.h>
#include <stdint.h>
/* Defines a GDT entry. We say packed, because it prevents the
 * *  compiler from doing things that it thinks is best: Prevent
 * *  compiler "optimization" by packing */
struct gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed));

/* Special pointer which includes the limit: The max bytes
 * *  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct gdt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[3];
struct gdt_ptr gp;

/* This will be a function in start.asm. We use this to properly
 * *  reload the new segment registers */
extern void gdt_flush();

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint32_t access,
                  uint8_t gran) {
  /* Setup the descriptor base address */
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;

  /* Setup the descriptor limits */
  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F);

  /* Finally, set up the granularity and access flags */
  gdt[num].granularity |= (gran & 0xF0);
  gdt[num].access = access;
}
/* Should be called by main. This will setup the special GDT * *  pointer, set
 * up the first 3 entries in our GDT, and then
 * *  finally call gdt_flush() in our assembler file in order
 * *  to tell the processor where the new GDT is and update the
 * *  new segment registers */
void gdt_install() {
  /* Setup the GDT pointer and limit */
  gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
  gp.base = (size_t)&gdt;

  // NULL descriptor
  gdt_set_gate(0, 0, 0, 0, 0);

  // The second entry is our Code Segment. The base address
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

  // The third entry is our Data Segment. It's EXACTLY the
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

  /* Flush out the old GDT and install the new changes! */
  gdt_flush();
}
