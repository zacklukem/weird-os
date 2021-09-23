#ifndef INCLUDES_KERNEL_ELF_H
#define INCLUDES_KERNEL_ELF_H

#include <stdint.h>

namespace elf {

enum e_type_t : uint16_t {
  ET_NONE = 0,        // No file type
  ET_REL = 1,         // Relocatablefile
  ET_EXEC = 2,        // Executablefile
  ET_DYN = 3,         // Shared object file
  ET_CORE = 4,        // Corefile
  ET_LOPROC = 0xff00, // Processor-specific
  ET_HIPROC = 0xffff, // Processor-specific
};

enum e_machine_t : uint16_t {
  EM_NONE = 0,  // No machine
  EM_M32 = 1,   // AT &T WE 32100
  EM_SPARC = 2, // SPARC
  EM_386 = 3,   // Intel 80386
  EM_68K = 4,   // Motorola 68000
  EM_88K = 5,   // Motorola 88000
  EM_860 = 7,   // Intel 80860
  EM_MIPS = 8,  // MIPS RS3000
};

enum e_class_t : uint8_t {
  ELFCLASSNONE = 0, // Invalid class
  ELFCLASS32 = 1,   // 32-bit objects
  ELFCLASS64 = 2    // 64-bit objects
};

enum e_data_encoding_t : uint8_t {
  ELFDATANONE = 0, // Invalid data encoding
  ELFDATA2LSB = 1, // LSB twos complement
  ELFDATA2MSB = 2  // MSB twos complement
};

struct elf_header_t {
  union {
    struct {
      uint32_t magic;
      e_class_t file_class;
      e_data_encoding_t data_encoding;
      uint8_t file_version;
    };
    uint8_t ident[16];
  };
  e_type_t type;
  e_machine_t machine;
  uint32_t version;
  uint32_t entry;
  uint32_t phoff;

  uint32_t shoff; ///< byte offset from the start of the file to the section
                  ///< header table
  uint32_t flags;
  uint16_t ehsize;
  uint16_t phentsize;
  uint16_t phnum;
  uint16_t shentsize; ///< size in bytes of each entry
  uint16_t shnum;     ///< how many entries the section header table contains
  uint16_t shstrndx;
} __attribute__((packed));

enum sh_type_t : uint32_t {
  SHT_NULL = 0,
  SHT_PROGBITS = 1,
  SHT_SYMTAB = 2,
  SHT_STRTAB = 3,
  SHT_RELA = 4,
  SHT_HASH = 5,
  SHT_DYNAMIC = 6,
  SHT_NOTE = 7,
  SHT_NOBITS = 8,
  SHT_REL = 9,
  SHT_SHLIB = 10,
  SHT_DYNSYM = 11,
  SHT_LOPROC = 0x70000000,
  SHT_HIPROC = 0x7fffffff,
  SHT_LOUSER = 0x80000000,
  SHT_HIUSER = 0x8fffffff
};

struct section_header_t {
  uint32_t name;
  sh_type_t type;
  uint32_t flags;
  uint32_t addr;
  uint32_t offset;
  uint32_t size;
  uint32_t link;
  uint32_t info;
  uint32_t addralign;
  uint32_t entsize;
};

} // namespace elf

#endif
