#ifndef ELF_H
#define ELF_H
#include <stdint.h>

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef uint32_t Elf64_Sword;
typedef uint64_t Elf64_Xword;
typedef uint64_t Elf64_Sxword;

static const uint8_t EI_MAG0        =   0;
static const uint8_t EI_MAG1        =   1;
static const uint8_t EI_MAG2        =   2;
static const uint8_t EI_MAG3        =   3;
static const uint8_t EI_CLASS       =   4;
static const uint8_t EI_DATA        =   5;
static const uint8_t EI_VERSION     =   6;
static const uint8_t EI_OSABI       =   7;
static const uint8_t EI_ABIVERSION  =   8;
static const uint8_t EI_PAD         =   9;
static const uint8_t EI_NIDENT      =   16;
// so apparently C compilers treat const differently than c++
// meaning that for some stupid reason I *have* to use preprocessor
// constants to define array size. >:(
#define EI_NIDENT   16

// e_ident[EI_CLASS]
static const uint8_t ELF_CLASS32    =   1;
static const uint8_t ELF_CLASS64    =   2;

// e_ident[]
static const uint8_t ELF_DATA2LSB   =   1;
static const uint8_t ELF_DATA2MSB   =   2;

static const uint8_t ELF_OSABI_SYSV =   0;
static const uint8_t ELF_OSABI_HPUX =   1;
static const uint8_t ELF_OSABI_SA   =   255; // standalone/embedded exe

// obj-file type
static const uint8_t ET_NONE        =   0;
static const uint8_t ET_REL         =   1;
static const uint8_t ET_EXEC        =   2;
static const uint8_t ET_DYN         =   3;
static const uint8_t ET_CORE        =   4;
static const uint16_t ET_LOOS       =   0xFE00;
static const uint16_t ET_HIOS       =   0xFEFF;
static const uint16_t ET_LOPROC     =   0xFF00;
static const uint16_t ET_HIPROC     =   0xFFFF;

// symbol bindings (high 4 bits in struct --> Elf64_Sym.st_info)
static const uint8_t STB_LOCAL      =   0;
static const uint8_t STB_GLOBAL     =   1;
static const uint8_t STB_WEAK       =   2; // lower-precedence global
static const uint8_t STB_LOOS       =   10; // env-specific
static const uint8_t STB_HIOS       =   12;
static const uint8_t STB_LOPROC     =   13;
static const uint8_t STB_HIPROC     =   15;

// symbol types (low 4 bits in struct --> Elf64_Sym.st_info)
static const uint8_t STT_NOTYPE     =   0;
static const uint8_t STT_OBJECT     =   1;
static const uint8_t STT_FUNC       =   2;
static const uint8_t STT_SECTION    =   3;
static const uint8_t STT_FILE       =   4;
static const uint8_t STT_LOOS       =   10;
static const uint8_t STT_HIOS       =   12;
static const uint8_t STT_LOPROC     =   13;
static const uint8_t STT_HIPROC     =   15;

// Segment Types (Program Header)
static const uint8_t PT_NULL        =   0;  /* Unused entry */
static const uint8_t PT_LOAD        =   1;  /* Loadable segment */
static const uint8_t PT_DYNAMIC     =   2;  /* Dynamic linking tables */
static const uint8_t PT_INTERP      =   3;  /* Program interpreter path */
static const uint8_t PT_NOTE        =   4;  /* Note sections */
static const uint8_t PT_SHLIB       =   5;  /* Reserved */
static const uint8_t PT_PHDR        =   6;  /* Program header table */
static const uint32_t PT_LOOS       =   0x60000000;
static const uint32_t PT_HIOS       =   0x6FFFFFFF;
static const uint32_t PT_LOPROC     =   0x70000000;
static const uint32_t PT_HIPROC     =   0x7FFFFFFF;

// Segment attributes (permission bits)
static const uint8_t PF_X           =   0x1;
static const uint8_t PF_W           =   0x2;
static const uint8_t PF_R           =   0x4;
static const uint32_t PF_MASKOS     =   0x00FF0000;
static const uint32_t PF_MASKPROC   =   0xFF000000;
static const uint8_t TRIAL = 0;

typedef struct
{
    unsigned char   e_ident[EI_NIDENT];   /* ELF identification */
    Elf64_Half      e_type;             /* Object file type */
    Elf64_Half      e_machine;          /* Machine type */
    Elf64_Word      e_version;          /* Object file version */
    Elf64_Addr      e_entry;            /* Entry point address */
    Elf64_Off       e_phoff;            /* Program header offset */
    Elf64_Off       e_shoff;            /* Section header offset */
    Elf64_Word      e_flags;            /* Processor-specific flags */
    Elf64_Half      e_ehsize;           /* ELF header size */
    Elf64_Half      e_phentsize;        /* Size of program header entry */
    Elf64_Half      e_phnum;            /* Number of program header entries */
    Elf64_Half      e_shentsize;        /* Size of section header entry */
    Elf64_Half      e_shnum;            /* Number of section header entries */
    Elf64_Half      e_shstrndx;         /* Section name string table index */
} Elf64_Ehdr;

typedef struct
{
    Elf64_Word      p_type;     /* Type of segment */
    Elf64_Word      p_flags;    /* Segment attributes */
    Elf64_Off       p_offset;   /* Offset in file */
    Elf64_Addr      p_vaddr;    /* Virtual address in memory */
    Elf64_Addr      p_paddr;    /* Reserved */
    Elf64_Xword     p_filesz;   /* Size of segment in file */
    Elf64_Xword     p_memsz;    /* Size of segment in memory */
    Elf64_Xword     p_align;    /* Alignment of segment */
} Elf64_Phdr;

typedef struct
{
    Elf64_Word      st_name;    /* Symbol name */
    unsigned char   st_info;    /* Type and Binding attributes */
    unsigned char   st_other;   /* Reserved */
    Elf64_Half      st_shndx;   /* Section table index */
    Elf64_Addr      st_value;   /* Symbol value */
    Elf64_Xword     st_size;    /* Size of object */
} Elf64_Sym;

#endif