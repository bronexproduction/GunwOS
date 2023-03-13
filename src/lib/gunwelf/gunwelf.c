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

    #warning how to validate file size
    #warning validate more fields
    #warning TO BE IMPLEMENTED

    if (expectation) {
        return validateExpectation(headerPtr, expectation);
    }

    return true;
}

size_t elfImageBytes(const ptr_t filePtr) {
    #warning TO BE IMPLEMENTED
    return 0;
}
