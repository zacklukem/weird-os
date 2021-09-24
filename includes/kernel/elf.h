#ifndef INCLUDES_KERNEL_ELF_H
#define INCLUDES_KERNEL_ELF_H

#include <assert.h>
#include <kernel/page.h>
#include <stdint.h>
#include <string.h>

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

enum sh_flags_t : uint32_t {
  SHF_WRITE = 0x1,
  SHF_ALLOC = 0x2,
  SHF_EXECINSTR = 0x4,
  SHF_MASKPROC = 0xf0000000
};

struct section_header_t {
  uint32_t name;
  sh_type_t type;
  sh_flags_t flags;
  uint32_t addr;
  uint32_t offset;
  uint32_t size;
  uint32_t link;
  uint32_t info;
  uint32_t addralign;
  uint32_t entsize;
} __attribute__((packed));

enum st_shndx_t : uint32_t {
  STB_LOCAL = 0,
  STB_GLOBAL = 1,
  STB_WEAK = 2,
  STB_LOPROC = 13,
  STB_HIPROC = 15
};

enum st_info_t : uint8_t {
  STT_NOTYPE = 0,
  STT_OBJECT = 1,
  STT_FUNC = 2,
  STT_SECTION = 3,
  STT_FILE = 4,
  STT_LOPROC = 13,
  STT_HIPROC = 15
};

struct symbol_t {
  uint32_t name;
  uint32_t value;
  uint32_t size;
  st_info_t info;
  uint8_t other;
  st_shndx_t shndx;
} __attribute__((packed));

enum p_type_t : uint32_t {
  PT_NULL = 0,
  PT_LOAD = 1,
  PT_DYNAMIC = 2,
  PT_INTERP = 3,
  PT_NOTE = 4,
  PT_SHLIB = 5,
  PT_PHDR = 6,
  PT_LOPROC = 0x70000000,
  PT_HIPROC = 0x7fffffff
};

enum p_flags_t : uint32_t {
  PF_X = 0x1,
  PF_W = 0x2,
  PF_R = 0x4,
  PF_MASKPROC = 0xf0000000,
};

struct program_t {
  p_type_t type;
  uint32_t offset;
  uint32_t vaddr;
  uint32_t paddr;
  uint32_t filesz;
  uint32_t memsz;
  uint32_t flags;
  uint32_t align;
};

class elf_loader {
public:
  elf_loader(void *addr);
  /**
   * Get a pointer to the program's elf header
   */
  inline elf_header_t *header();

  inline program_t *program_header(uint32_t i);
  inline section_header_t *section_header(uint32_t i);

  template <class T> inline T *offset(uint32_t off);
  template <class T> inline T *offset(uint32_t off, uint32_t i);

  void load(page_allocator *allocator);
  inline uint32_t entry();

private:
  void *addr;
};

template <class T> inline T *elf_loader::offset(uint32_t off) {
  return (T *)((uint32_t)addr + off);
}

template <class T> inline T *elf_loader::offset(uint32_t off, uint32_t i) {
  return &((T *)((uint32_t)addr + off))[i];
}

elf_loader::elf_loader(void *addr) : addr(addr) {
  auto hdr = header();
  assert(hdr->magic == 0x464c457f && "Elf read error");
  assert(hdr->version == 1);
  assert(hdr->ehsize == sizeof(elf_header_t));
  assert(hdr->phentsize == sizeof(program_t));
  assert(hdr->shentsize == sizeof(section_header_t));
}

inline elf_header_t *elf_loader::header() { return (elf_header_t *)addr; }

inline program_t *elf_loader::program_header(uint32_t i) {
  return offset<program_t>(header()->phoff, i);
}

inline section_header_t *elf_loader::section_header(uint32_t i) {
  return offset<section_header_t>(header()->shoff, i);
}

inline uint32_t elf_loader::entry() { return header()->entry; }

void elf_loader::load(page_allocator *allocator) {
  assert(header()->phnum > 0 && "Not an executable");
  for (uint32_t i = 0; i < header()->phnum; i++) {
    auto ph = program_header(i);
    if (ph->type != p_type_t::PT_LOAD)
      continue;
    allocator->allocate(ph->vaddr, ph->memsz, 0, ph->flags & p_flags_t::PF_W);
    memcpy((void *)ph->vaddr, offset<void>(ph->offset), ph->filesz);
  }
}

} // namespace elf

#endif
