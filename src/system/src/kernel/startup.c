//
//  startup.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include <types.h>

#define ELF_MAGIC 0x7F454c46

enum elfClass {
    ECLASS_32 = 1,
    ECLASS_64 = 2,
};

enum elfEndianess {
    EENDIAN_LITTLE  = 1,
    EENDIAN_BIG     = 2,
};

enum elfType {
    ETYPE_NONE      = 0x00,     /* Unknown */
    ETYPE_RELOC     = 0x01,     /* Relocatable file */
    ETYPE_EXEC      = 0x02,     /* Executable file */
    ETYPE_SHARED    = 0x03,     /* Shared object */
    ETYPE_CORE 	    = 0x04,     /* Core file */
    ETYPE_LOOS 	    = 0xFE00,   /* Reserved inclusive range bottom - OS specific */
    ETYPE_HIOS      = 0xFEFF,   /* Reserved inclusive range top    - OS specific */
    ETYPE_LOCPU     = 0xFF00,   /* Reserved inclusive range bottom - CPU specific */
    ETYPE_HICPU     = 0xFFFF,   /* Reserved inclusive range top    - CPU specific */
};

struct __attribute__((packed)) elfIdentifier {
    const uint_32 magic;                        /* ELF magic number */
    const enum elfClass class:          8*1;    /* ELF class (32/64-bit) */
    const enum elfEndianess endianess:  8*1;    /* Byte endianess (little/big) - this affects interpretation of multi-byte fields starting with offset 0x10. */
    const uint_8 version;                       /* ELF version */
    const uint_8 osAbi;                         /* OS ABI */
    const uint_8 abiVersion;                    /* ABI version */
    const uint_8 _reserved[7];                  /* Reserved bytes */
};

struct __attribute__((packed)) elfHeader32 {
    const struct elfIdentifier identifier;
    const uint_16 type;                     /* Object file type */
    const uint_16 architecture;             /* Target instruction set architecture */
    const uint_32 version;                  /* ELF version */
    const ptr_t entry; 	                    /* Address the entry point (0 if no entry point) */
    const ptr_t programHeaderTable;         /* Start of the program header table */
    const ptr_t sectionHeaderTable;         /* Start of the section header table */
    const uint_32 archFlags; 	            /* Flags for target architecture */
    const uint_16 elfHeaderSize;            /* Size of this header */
    const uint_16 programHeaderEntrySize;   /* Size of a program header table entry */
    const uint_16 programHeaderEntries;     /* Number of entries in the program header table */
    const uint_16 sectionHeaderEntrySize;   /* Size of a section header table entry */
    const uint_16 sectionHeaderEntries;     /* Number of entries in the section header table */
    const uint_16 sectionNamesEntryIndex; 	/* Index of the section header table entry that contains the section names */
};
_Static_assert(sizeof(struct elfHeader32) == 52, "Unexpected struct elf32 size");

void k_startup() {
    ptr_t cliElf = (ptr_t)0x50000;
    const struct elfHeader32 *headerPtr = (struct elfHeader32 *)cliElf;

    (void)headerPtr;

    int i = 0;
    i++;
    #warning TO BE IMPLEMENTED
    
}
