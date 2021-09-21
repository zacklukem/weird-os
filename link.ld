ENTRY(start)
phys_addr = 0x0;
SECTIONS {
  . = phys_addr + 0x100000;
  kernel_virt_start = . ;
  kernel_phys_start = . - phys_addr ;

  .text ALIGN(0x1000) : AT(ADDR(.text)-phys_addr) {
    code_addr = . ;
    *(.text)
  }

  .rodata ALIGN(0x1000) : AT(ADDR(.rodata)-phys_addr) {
    *(.rodata)
  }

  .data ALIGN(0x1000) : AT(ADDR(.data)-phys_addr) {
    data_addr = . ;
    *(.data)
  }

  .bss ALIGN(0x1000) : AT(ADDR(.bss)-phys_addr) {
    bss_addr = . ;
    *(COMMON)
    *(.bss)
  }
  . = ALIGN(0x1000);

  end_addr = . ;
  kernel_virt_end = . ;
  kernel_phys_end = . - phys_addr ;

}
