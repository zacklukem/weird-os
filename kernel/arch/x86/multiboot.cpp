#include <arch/x86/multiboot.h>

extern "C" {
multiboot_header *mb_header_val;
}

multiboot_header &get_multiboot_header() { return *mb_header_val; }
