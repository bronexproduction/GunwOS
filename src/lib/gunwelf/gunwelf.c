//
//  gunwelf.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.03.2023.
//

#include "gunwelf.h"
#include <string.h>

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

    #warning TO BE IMPLEMENTED
    return true;
}

static bool validateSectionHeaderEntry(const struct elfSectionHeaderEntry32 * const entryPtr) {
    if (!entryPtr) {
        return false;
    }

    #warning TO BE IMPLEMENTED
    return true;
}

bool elfValidate(const ptr_t filePtr,
                 const size_t fileSizeBytes, 
                 const struct elfExpectation * const expectation) {
    if (!filePtr) {
        return false;
    }

    const struct elfHeader32 * const headerPtr = (struct elfHeader32 *)filePtr;

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

    size_t headerSizeBytes;
    size_t programHeaderEntrySize;
    size_t sectionHeaderEntrySize;
    switch (headerPtr->identifier.class) {
        case ECLASS_32:
            headerSizeBytes = ELF_HEADER_SIZE_32;
            programHeaderEntrySize = ELF_PROGRAM_ENTRY_SIZE_32;
            sectionHeaderEntrySize = ELF_SECTION_ENTRY_SIZE_32;
            break;
        default:
            return false;
    }
    if (headerPtr->elfHeaderSize != headerSizeBytes) {
        return false;
    }
    if (headerPtr->programHeaderEntrySize != programHeaderEntrySize) {
        return false;
    }
    if (headerPtr->sectionHeaderEntrySize != sectionHeaderEntrySize) {
        return false;
    }

    hugeSize_t totalFileAddressedBytes = headerSizeBytes;
    hugeSize_t totalMemoryAddressedBytes = 0;
    #warning TO BE CALCULATED
    for (size_t entry = 0; entry < headerPtr->programHeaderEntries; ++entry) {
        uint_64 offset = headerPtr->elfHeaderSize + (entry * programHeaderEntrySize);
        if ((offset + programHeaderEntrySize) > fileSizeBytes) {
            return false;
        }
        const struct elfProgramHeaderEntry32 * const entryPtr = (struct elfProgramHeaderEntry32 *)(filePtr + (uint_32)offset);
        if (!validateProgramHeaderEntry(entryPtr)) {
            return false;
        }
        totalFileAddressedBytes += programHeaderEntrySize + entryPtr->fileSizeBytes;
    }
    for (size_t entry = 0; entry < headerPtr->sectionHeaderEntries; ++entry) {
        if ((totalFileAddressedBytes + sectionHeaderEntrySize) > fileSizeBytes) {
            return false;
        }
        if (!validateSectionHeaderEntry((struct elfSectionHeaderEntry32 *)(uint_32)totalFileAddressedBytes)) {
            return false;
        }
        totalFileAddressedBytes += sectionHeaderEntrySize;
    }

    if (totalFileAddressedBytes != fileSizeBytes) {
        return false;
    }

    if (expectation) {
        return validateExpectation(headerPtr, expectation);
    }

    return true;
}

size_t elfImageBytes(const ptr_t filePtr) {
    #warning TO BE IMPLEMENTED
    return 0;
}
