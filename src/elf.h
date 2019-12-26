#pragma once

#include <stdint.h>

enum {
    ET_NONE,
    ET_REL,  
    ET_EXEC,
    ET_DYN,
    ET_CORE,
};

#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

enum {
    EM_NONE,
    EM_M32,
    EM_SPARC,
    EM_386,
    EM_68K,
    EM_88K,
    EM_860 = 7,
    EM_MIPS,
};

enum {
    EV_NONE,
    EV_CURRENT,
};

enum {
    EI_MAG0,
    EI_MAG1,
    EI_MAG2,
    EI_MAG3,
    EI_CLASS,
    EI_DATA,
    EI_VERSION,
    EI_PAD,
};

#define EI_NIDENT 16


#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

enum {
    ELFCLASSNONE,
    ELFCLASS32,
    ELFCLASS64,
};

enum {
    ELFDATANONE,
    ELFDATA2LSB,
    ELFDATA2MSB,
};

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    uint32_t      e_entry;
    uint32_t      e_phoff;
    uint32_t      e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    uint16_t      e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
} elf32_hdr;

#define SHN_UNDEF     0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC    0xff00
#define SHN_HIPROC    0xff1f
#define SHN_ABS       0xfff1
#define SHN_COMMON    0xfff2
#define SHN_HIRESERVE 0xffff

typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} elf32_shdr;

enum {
    SHT_NULL,
    SHT_PROGBITS,
    SHT_SYMTAB,
    SHT_STRTAB,
    SHT_RELA,
    SHT_HASH,
    SHT_DYNAMIC,
    SHT_NOTE,
    SHT_NOBITS,
    SHT_REL,
    SHT_SHLIB,
    SHT_DYNSYM,
};

#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff

#define SHF_WRITE     0x1
#define SHF_ALLOC     0x2
#define SHF_EXECINSTR 0x4
#define SHF_MASKPROC 0xf0000000

#define STN_UNDEF 0

typedef struct {
    uint16_t st_name;
    uint16_t st_value;
    uint16_t st_size;
    uint8_t  st_info;
    uint8_t  st_other;
    uint16_t st_shndx;
} elf32_sym;

#define ELF32_ST_BIND(i) ((i)>>4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b, t) (((b)<<4)|((t)&0xf))

enum {
    STB_LOCAL,
    STB_GLOBAL,
    STB_WEAK, 
};

#define STB_LOPROC 13
#define STB_HIPROC 15

enum {
    STT_NOTYPE,
    STT_OBJECT,
    STT_FUNC,
    STT_SECTION,
    STT_FILE,
};

#define STT_LOPROC 13
#define STT_HIPROC 15

typedef struct {
    uint32_t r_offset;
    uint32_t r_info;
} elf32_rel;

typedef struct {
    uint32_t r_offset;
    uint32_t r_info;
    int16_t  r_addend;
} elf32_rela;

#define ELF32_R_SYM(i)  ((i)>>8)
#define ELF32_R_TYPE(i) ((uint8_t)(i))
#define ELF32_R_INFO(s, t) (((s)<<8)+(uint8_t)(t))

enum {
    R_386_NONE,
    R_386_32,
    R_386_PC32,
    R_386_GOT32,
    R_386_PLT32,
    R_386_COPY,
    R_386_GLOB_DAT,
    R_386_JMP_SLOT,
    R_386_RELATIVE,
    R_386_GOTOFF,
    R_386_GOTPC,
};

typedef struct {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} elf32_phdr;

enum {
    PT_NULL,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR,
};

#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7fffffff

