ENTRY(start)
SECTIONS {
  . = 0x00100000;
  .text ALIGN(0x1000) : {
    code_addr = . ;
    *(.text)
    *(.rodata)
  }

  .data ALIGN(0x1000) : {
    data_addr = . ;
    *(.data)
  }

  .bss ALIGN(0x1000) : {
    bss_addr = . ;
    *(COMMON)
    *(.bss)
  }
  . = ALIGN(0x1000);
  end_addr = . ;
}
