//
//  gunwelf.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include "gunwelf.h"
#include <string.h>
#include <defs.h>

#define PROGRAM_HEADER_ENTRY_OFFSET_32(INDEX) (ELF_HEADER_SIZE_32 + INDEX * ELF_PROGRAM_ENTRY_SIZE_32)
#define SECTION_HEADER_ENTRY_OFFSET_32(INDEX, HEADER_PTR, FILE_SIZE_BYTES) (FILE_SIZE_BYTES - ((HEADER_PTR->sectionHeaderEntries - INDEX) * ELF_SECTION_ENTRY_SIZE_32))

static bool validateIdentifier(const struct elfIdentifier * const idPtr) {
    if (!idPtr) {
        return false;
    }

    if (strcmpl(idPtr->magic, ELF_MAGIC, 4)) {
        return false;
    }
    if (idPtr->version != ELF_VERSION) {
        return false;
    }

    return true;
}

static bool validateExpectation(const struct elfHeader32 * const headerPtr, 
                                const struct elfExpectation * const expPtr) {
    if (!headerPtr || !expPtr) {
        return false;
    }

    if (headerPtr->identifier.class != expPtr->class) {
        return false;
    }
    if (headerPtr->identifier.endianess != expPtr->endianess) {
        return false;
    }
    if (headerPtr->type != expPtr->type) {
        return false;
    }
    if (headerPtr->architecture != expPtr->architecture) {
        return false;
    }

    return true;
}

static bool validateProgramHeaderEntry(const struct elfProgramHeaderEntry32 * const entryPtr) {
    if (!entryPtr) {
        return false;
    }

    if (entryPtr->alignment > 1) {
        if (__builtin_popcount(entryPtr->alignment) != 1) {
            return false;
        }
        if ((addr_t)entryPtr->offset % entryPtr->alignment != (addr_t)entryPtr->virtualAddr % entryPtr->alignment) {
            return false;
        }

        #warning VALIDATE FILE OFFSET AND SIZE
    }

    return true;
}

static bool validateSectionHeaderEntry(const struct elfSectionHeaderEntry32 * const entryPtr) {
    if (!entryPtr) {
        return false;
    }
    
    if (entryPtr->addressAlignment > 1) {
        if (__builtin_popcount(entryPtr->addressAlignment) != 1) {
            return false;
        }
        if ((addr_t)entryPtr->virtualAddr % entryPtr->addressAlignment) {
            return false;
        }
        addr_t low = (addr_t)entryPtr->virtualAddr;
        addr_t high = entryPtr->fileSizeBytes;
        if (low && (low + high < low)) {
            return false;
        }

        #warning VALIDATE FILE OFFSET AND SIZE
    }

    return true;
}

static ptr_t section(const data_t fileData, const enum elfSectionType type, const struct elfSectionHeaderEntry32 * * resultSectionHeaderEntry) {
    if (!fileData.ptr) {
        return NULL;
    }
    if (!fileData.bytes) {
        return NULL;
    }

    const size_t sectionHeaderEntryCount = elfGetSectionHeaderEntryCount(fileData.ptr);
    const struct elfSectionHeaderEntry32 * sectionHeaderEntry = nullptr;
    for (size_t index = 0; index < sectionHeaderEntryCount; ++index) {
        const struct elfSectionHeaderEntry32 * const currentSectionHeaderEntry = elfGetSectionHeaderEntryAtIndex(fileData, index);
        if (!currentSectionHeaderEntry) {
            return NULL;
        } else if (currentSectionHeaderEntry->type == type) {
            sectionHeaderEntry = currentSectionHeaderEntry;
            break;
        }
    }

    if ((addr_t)fileData.ptr >= (addr_t)fileData.ptr + sectionHeaderEntry->offset + sectionHeaderEntry->fileSizeBytes) {
        /*
            Wraparound
        */
        return NULL;
    }
    if (sectionHeaderEntry->fileSizeBytes % ELF_SYMTAB_SECTION_ENTRY_SIZE_32) {
        /*
            Unexpected size
        */
        return NULL;
    }

    if (sectionHeaderEntry) {
        *resultSectionHeaderEntry = sectionHeaderEntry;
    }
    return fileData.ptr + sectionHeaderEntry->offset;
}

static struct elfSymtabSectionEntry32 * symtab(const data_t fileData, size_t * const entryCount) {
    if (!entryCount) {
        return NULL;
    }

    *entryCount = 0;

    const struct elfSectionHeaderEntry32 * symbolTableSectionHeaderEntry = nullptr;
    struct elfSymtabSectionEntry32 * const symbolTable = section(fileData, ESECTYPE_SYMTAB, &symbolTableSectionHeaderEntry);

    if (!symbolTableSectionHeaderEntry) {
        return nullptr;
    }
    if (!symbolTable) {
        return nullptr;
    }

    *entryCount = symbolTableSectionHeaderEntry->fileSizeBytes / ELF_SYMTAB_SECTION_ENTRY_SIZE_32;
    return symbolTable;
}

static ptr_t strtab(const data_t fileData) {
    return section(fileData, ESECTYPE_STRTAB, nullptr);
}

bool elfValidate(const data_t fileData, 
                 const struct elfExpectation * const expectation) {
    if (!fileData.ptr) {
        return false;
    }

    const struct elfHeader32 * const headerPtr = (struct elfHeader32 *)fileData.ptr;

    if (!validateIdentifier(&headerPtr->identifier)) {
        return false;
    }
    if (headerPtr->identifier.version != headerPtr->version) {
        return false;
    }
    if (!headerPtr->programHeaderTable) {
        return false;
    }
    if (!headerPtr->sectionHeaderTable) {
        return false;
    }

    switch (headerPtr->identifier.class) {
        case ECLASS_32:
            break;
        default:
            return false;
    }
    if (headerPtr->elfHeaderSize != ELF_HEADER_SIZE_32) {
        return false;
    }
    if (headerPtr->programHeaderEntrySize != ELF_PROGRAM_ENTRY_SIZE_32) {
        return false;
    }
    if (headerPtr->sectionHeaderEntrySize != ELF_SECTION_ENTRY_SIZE_32) {
        return false;
    }

    if (fileData.bytes < (ELF_HEADER_SIZE_32 + (headerPtr->programHeaderEntries * ELF_PROGRAM_ENTRY_SIZE_32) + (headerPtr->sectionHeaderEntries * ELF_SECTION_ENTRY_SIZE_32))) {
        return false;
    }
    for (size_t index = 0; index < headerPtr->programHeaderEntries; ++index) {
        const struct elfProgramHeaderEntry32 * const entry = (struct elfProgramHeaderEntry32 *)((ptr_t)headerPtr + PROGRAM_HEADER_ENTRY_OFFSET_32(index));
        if (!validateProgramHeaderEntry(entry)) {
            return false;
        }
    }
    for (size_t entry = 0; entry < headerPtr->sectionHeaderEntries; ++entry) {
        const struct elfSectionHeaderEntry32 * const entryPtr = (struct elfSectionHeaderEntry32 *)(fileData.ptr + SECTION_HEADER_ENTRY_OFFSET_32(entry, headerPtr, fileData.bytes));
        if (!validateSectionHeaderEntry(entryPtr)) {
            return false;
        }
    }

    if (expectation) {
        return validateExpectation(headerPtr, expectation);
    }

    return true;
}

size_t elfGetSectionHeaderEntryCount(const ptr_t filePtr) {
    const struct elfHeader32 * const headerPtr = (struct elfHeader32 *)filePtr;
    return headerPtr->sectionHeaderEntries;
}

struct elfSectionHeaderEntry32 * elfGetSectionHeaderEntryAtIndex(const data_t fileData,
                                                                 const size_t index) {
    const struct elfHeader32 * const headerPtr = (struct elfHeader32 *)fileData.ptr;
    if (index >= headerPtr->sectionHeaderEntries) {
        return nullptr;
    }
    return (struct elfSectionHeaderEntry32 *)(fileData.ptr + SECTION_HEADER_ENTRY_OFFSET_32(index, headerPtr, fileData.bytes));
}

addr_t elfGetEntry(const data_t fileData) {
    if (!fileData.ptr) {
        return NULL;
    }
    if (fileData.bytes < sizeof(struct elfHeader32)) {
        return NULL;
    }
    
    const struct elfHeader32 * const headerPtr = (struct elfHeader32 *)fileData.ptr;
    return headerPtr->entry;
}

addr_t elfGetSymbol(const data_t fileData,
                    const char * const symbolName) {
    if (!fileData.ptr) {
        return NULL;
    }
    if (fileData.bytes < sizeof(struct elfHeader32)) {
        return NULL;
    }
    if (!symbolName) {
        return NULL;
    }

    size_t symbolTableEntryCount;
    const struct elfSymtabSectionEntry32 * symbolTable = symtab(fileData, &symbolTableEntryCount);
    const ptr_t strTable = strtab(fileData);
    if (!symbolTable || !symbolTableEntryCount || !strTable) {
        return NULL;
    }

    for (size_t index = 0; index < symbolTableEntryCount; ++index) {
        const struct elfSymtabSectionEntry32 * const symbolTableEntry = &symbolTable[index];
        (void)symbolTableEntry;

        #warning TODO
    }

    #warning TODO

    const struct elfHeader32 * const headerPtr = (struct elfHeader32 *)fileData.ptr;
    return headerPtr->entry;
}
