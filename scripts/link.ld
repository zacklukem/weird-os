ENTRY(start)
kernel_boot = 0x100000;

SECTIONS {
  kernel_boot = 0x00100000;
	kernel_higher_half = 0xc0000000;

  . = kernel_boot + kernel_higher_half ;
  kernel_start = . ;

  .text ALIGN(0x1000) : AT(ADDR(.text)-kernel_higher_half) {
    kernel_text_start = . ;
    *(.text)
    kernel_text_end = . ;
  }

  .rodata ALIGN(0x1000) : AT(ADDR(.rodata)-kernel_higher_half) {
    *(.rodata)
  }

  .data ALIGN(0x1000) : AT(ADDR(.data)-kernel_higher_half) {
    kernel_data_start = . ;
    *(.data)
    kernel_data_end = . ;
  }

  .bss ALIGN(0x1000) : AT(ADDR(.bss)-kernel_higher_half) {
    *(COMMON)
    *(.bss)
  }
  . = ALIGN(0x1000);

  kernel_end = .;

}
