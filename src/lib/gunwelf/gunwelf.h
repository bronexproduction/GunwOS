//
//  gunwelf.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#ifndef GUNWELF_H
#define GUNWELF_H

#include <types.h>

#define ELF_MAGIC ("\x7F""ELF")
#define ELF_VERSION 1
#define ELF_HEADER_SIZE_32 (size_t)0x34
#define ELF_PROGRAM_ENTRY_SIZE_32 (size_t)0x20
#define ELF_SECTION_ENTRY_SIZE_32 (size_t)0x28

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
    ETYPE_LO_OS     = 0xFE00,   /* Reserved inclusive range bottom - OS specific */
    ETYPE_HI_OS     = 0xFEFF,   /* Reserved inclusive range top    - OS specific */
    ETYPE_LO_CPU    = 0xFF00,   /* Reserved inclusive range bottom - CPU specific */
    ETYPE_HI_CPU    = 0xFFFF,   /* Reserved inclusive range top    - CPU specific */
};

enum elfSegmentType {
    ESEGTYPE_NULL                   = 0x00,         /* Entry unused */
    ESEGTYPE_LOADABLE               = 0x01,         /* Loadable segment */
    ESEGTYPE_DYNAMIC                = 0x02,         /* Dynamic linking information */
    ESEGTYPE_INTERPRETER            = 0x03,         /* Interpreter information */
    ESEGTYPE_AUX                    = 0x04,         /* Auxiliary information */
    ESEGTYPE_RESERVED               = 0x05,         /* Reserved */
    ESEGTYPE_PROGRAM_HEADER_TABLE   = 0x06,         /* Segment containing program header table itself */
    ESEGTYPE_TLS_TEMPLATE           = 0x07,         /* Thread-Local Storage template */
    ESEGTYPE_LO_OS                  = 0x60000000,   /* Reserved inclusive range bottom - OS specific */
    ESEGTYPE_HI_OS                  = 0x6FFFFFFF,   /* Reserved inclusive range top    - OS specific */
    ESEGTYPE_LO_CPU                 = 0x70000000,   /* Reserved inclusive range bottom - CPU specific */
    ESEGTYPE_HI_CPU                 = 0x7FFFFFFF,   /* Reserved inclusive range top    - CPU specific */
};

enum elfSectionType {
    ESECTYPE_NULL           = 0x00, /* Entry unused */
    ESECTYPE_PROGBITS       = 0x01, /* Program data */
    ESECTYPE_SYMTAB         = 0x02, /* Symbol table */
    ESECTYPE_STRTAB         = 0x03, /* String table */
    ESECTYPE_RELA           = 0x04, /* Relocation entries with addends */
    ESECTYPE_HASH           = 0x05, /* Symbol hash table */
    ESECTYPE_DYNAMIC        = 0x06, /* Dynamic linking information */
    ESECTYPE_NOTE           = 0x07, /* Notes */
    ESECTYPE_NOBITS         = 0x08, /* Program space with no data (bss) */
    ESECTYPE_REL            = 0x09, /* Relocation entries, no addends */
    ESECTYPE_SHLIB          = 0x0A, /* Reserved */
    ESECTYPE_DYNSYM         = 0x0B, /* Dynamic linker symbol table */
    ESECTYPE_INIT_ARRAY     = 0x0E, /* Array of constructors */
    ESECTYPE_FINI_ARRAY     = 0x0F, /* Array of destructors */
    ESECTYPE_PREINIT_ARRAY  = 0x10, /* Array of pre-constructors */
    ESECTYPE_GROUP          = 0x11, /* Section group */
    ESECTYPE_SYMTAB_SHNDX   = 0x12, /* Extended section indices */
    ESECTYPE_NUM            = 0x13, /* Number of defined types. */
    ESECTYPE_LO_OS          = 0x60000000, /* OS-specific types start */
};

enum elfSectionAttr {
    ESECATTR_WRITE              = 0x01,     /* Writable */
    ESECATTR_ALLOC              = 0x02,     /* Occupies memory during execution */
    ESECATTR_EXECINSTR          = 0x04,     /* Executable */
    ESECATTR_MERGE              = 0x10,     /* Might be merged */
    ESECATTR_STRINGS            = 0x20,     /* Contains null-terminated strings */
    ESECATTR_INFO_LINK          = 0x40,     /* 'sh_info' contains SHT index */
    ESECATTR_LINK_ORDER         = 0x80,     /* Preserve order after combining */
    ESECATTR_OS_NONCONFORMING   = 0x100,    /* Non-standard OS specific handling required */
    ESECATTR_GROUP              = 0x200,    /* Section is member of a group */
    ESECATTR_TLS                = 0x400,    /* Section hold thread-local data */
    ESECATTR_ORDERED            = 0x04000000,   /* Special ordering requirement (Solaris) */
    ESECATTR_EXCLUDE            = 0x08000000,   /* Section is excluded unless referenced or allocated (Solaris) */
    ESECATTR_MASKOS             = 0x0FF00000,   /* OS-specific */
    ESECATTR_MASKPROC           = 0xF0000000,   /* Processor-specific */
};

struct __attribute__((packed)) elfIdentifier {
    const char magic[4];                        /* ELF magic number */
    const enum elfClass class:          8*1;    /* ELF class (32/64-bit) */
    const enum elfEndianess endianess:  8*1;    /* Byte endianess (little/big) - this affects interpretation of multi-byte fields starting with offset 0x10. */
    const uint_8 version;                       /* ELF version */
    const uint_8 osAbi;                         /* OS ABI */
    const uint_8 abiVersion;                    /* ABI version */
    const uint_8 _reserved[7];                  /* Reserved bytes */
};

struct __attribute__((packed)) elfHeader32 {
    const struct elfIdentifier identifier;
    const enum elfType type             :8*2;   /* Object file type */
    const uint_16 architecture;                 /* Target instruction set architecture */
    const uint_32 version;                      /* ELF version */
    const addr_t entry;                         /* Address the entry point (0 if no entry point) */
    const addr_t programHeaderTable;            /* Start of the program header table */
    const addr_t sectionHeaderTable;            /* Start of the section header table */
    const uint_32 archFlags; 	                /* Flags for target architecture */
    const uint_16 elfHeaderSize;                /* Size of this header */
    const uint_16 programHeaderEntrySize;       /* Size of a program header table entry */
    const uint_16 programHeaderEntries;         /* Number of entries in the program header table */
    const uint_16 sectionHeaderEntrySize;       /* Size of a section header table entry */
    const uint_16 sectionHeaderEntries;         /* Number of entries in the section header table */
    const uint_16 sectionNamesEntryIndex;       /* Index of the section header table entry that contains the section names */
};
_Static_assert(sizeof(struct elfHeader32) == ELF_HEADER_SIZE_32, "Unexpected struct elfHeader32 size");

struct __attribute__((packed)) elfProgramHeaderEntry32 {
    const enum elfSegmentType type      :8*4;   /* Type of the segment */
    const uint_32 offset;                       /* Offset of the segment in the file image */
    const addr_t virtualAddr;                   /* Virtual address of the segment in memory */
    const addr_t physicalAddr;                  /* Physical address of the segment in memory (where relevant) */
    const uint_32 fileSizeBytes;                /* Size in bytes of the segment in the file image (may be 0) */
    const uint_32 memorySizeBytes;              /* Size in bytes of the segment in memory (may be 0) */
    const uint_32 flags;                        /* Segment-dependent flags */
    const uint_32 alignment;                    /* 0 and 1 specify no alignment. 
                                                   Otherwise should be a positive, integral power of 2, 
                                                   with virtualAddr equating offset modulus alignment
                                                   (offset % alignment == virtualAddr % alignment) */
};
_Static_assert(sizeof(struct elfProgramHeaderEntry32) == ELF_PROGRAM_ENTRY_SIZE_32, "Unexpected struct elfProgramHeader32 size");

struct __attribute__((packed)) elfSectionHeaderEntry32 {
    const uint_32 nameOffset;                   /* An offset to a string in the .shstrtab section that represents the name of this section */
    const enum elfSectionType type      :8*4;   /* Type of the section */
    const enum elfSectionAttr attributes:8*4;   /* Attributes of the section */
    const addr_t virtualAddr;                   /* Virtual address of the section in memory, for sections that are loaded */
    const uint_32 offset;	                    /* Offset of the section in the file image */
    const uint_32 fileSizeBytes;                /* Size in bytes of the section in the file image (may be 0) */
    const uint_32 sectionIndex;                 /* Contains the section index of an associated section. 
                                                   This field is used for several purposes, depending on the type of section. */
    const uint_32 info;                         /* Contains extra information about the section. 
                                                   This field is used for several purposes, depending on the type of section. */
    const uint_32 addressAlignment;             /* Contains the required alignment of the section. 
                                                   0 and 1 specify no alignment
                                                   otherwise this field must be a power of two. */
    const uint_32 entrySizeBytes;               /* Contains the size, in bytes, of each entry, for sections that contain fixed-size entries. 
                                                   Otherwise, this field contains zero. */
};
_Static_assert(sizeof(struct elfSectionHeaderEntry32) == ELF_SECTION_ENTRY_SIZE_32, "Unexpected struct elfProgramHeader32 size");

struct elfExpectation {
    enum elfClass class;
    enum elfEndianess endianess;
    enum elfType type;
    uint_16 architecture;
};

bool elfValidate(const ptr_t filePtr, 
                 const size_t fileSizeBytes, 
                 const struct elfExpectation * const expectation);

/*
    Returns required memory size to allocate the binary
*/
size_t elfAllocBytes(const ptr_t filePtr, const size_t fileSizeBytes, addr_t * const virtualMemoryLowAddr);

/*
    Returns the number of section header entries
*/
size_t elfGetSectionHeaderEntryCount(const ptr_t filePtr);

/*
    Returns the section header entry at given index
*/
struct elfSectionHeaderEntry32 * elfGetSectionHeaderEntry(const ptr_t filePtr, const size_t index, const size_t fileSizeBytes);

/*
    Returns the entry point address
*/
addr_t elfGetEntry(const ptr_t filePtr);

#endif // GUNWELF_H
