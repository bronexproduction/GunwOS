//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <src/_gunwctrl.h>
#include <types.h>
#include <string.h>
#include <mem.h>
#include <gunwelf.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <log/log.h>
#include <storage/file.h>
#include "../func.h"

enum gnwCtrlError loadElf(const ptr_t filePtr, 
                          const size_t fileSizeBytes, 
                          ptr_t destPtr, 
                          size_t * const memBytes, 
                          addr_t * const entry) {
    if (!memBytes) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }
    if (!entry) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }
    addr_t vMemLow;
    *memBytes = elfAllocBytes(filePtr, fileSizeBytes, &vMemLow);
    if (!*memBytes) {
        return GCE_HEADER_INVALID;
    }
    addr_t vMemHigh = vMemLow + *memBytes - 1;
    if (vMemHigh <= vMemLow) {
        return GCE_HEADER_INVALID;
    }

    #warning allocate if possible
    #warning check memory limits
    #warning how to handle section flags?

    memzero(destPtr, *memBytes);
    for (size_t index = 0; index < elfGetSectionHeaderEntryCount(filePtr); ++index) {
        const struct elfSectionHeaderEntry32 * const entry = elfGetSectionHeaderEntry(filePtr, index, fileSizeBytes); 
        if (!entry) {
            OOPS("Unexpected nullptr", GCE_UNKNOWN);
        }
        if (!(entry->attributes & ESECATTR_ALLOC &&
              entry->type == ESECTYPE_PROGBITS)) {
            continue;
        }

        addr_t sectionVMemLow = (addr_t)entry->virtualAddr;
        addr_t sectionVMemHigh = sectionVMemLow + entry->fileSizeBytes - 1;
        /*
            Verify memory constraints
        */
        if (sectionVMemHigh < sectionVMemLow ||
            sectionVMemLow < vMemLow ||
            sectionVMemHigh > vMemHigh) {
            return GCE_HEADER_INVALID;
        }
        /*
            Verify file constraints
        */
        if (entry->offset >= fileSizeBytes ||
            entry->fileSizeBytes > (fileSizeBytes - entry->offset)) {
            return GCE_HEADER_INVALID;
        }

        addr_t relVMemAddr = sectionVMemLow - vMemLow;
        memcopy(filePtr + entry->offset, 
                destPtr + relVMemAddr, 
                entry->fileSizeBytes);
    }

    *entry = elfGetEntry(filePtr);
    if (*entry < vMemLow || vMemHigh < *entry) {
        return GCE_HEADER_INVALID;
    }
    *entry -= vMemLow;

    return GCE_NONE;
}

#define LOG_CODE(MSG, CODE) {                                           \
    LOG_START;                                                          \
    k_log_logd((data_t){ (ptr_t)abs_pathPtr, abs_descPtr->pathLen });   \
    LOG_NBR(" - ");                                                     \
    LOG_NBR(MSG);                                                       \
    if (CODE) {                                                         \
        char loc_code_str[10] = { 0 };                                  \
        int2str(CODE, loc_code_str);                                    \
        LOG_NBR(", code ");                                             \
        k_log_logd((data_t){ (ptr_t)loc_code_str, 10 });                \
    }                                                                   \
    LOG_END;                                                            \
}

void k_scr_usr_start(const procId_t procId, const struct gnwCtrlStartDescriptor * const descPtr) {
    SCLF_GET_VALID_ABS(const struct gnwCtrlStartDescriptor * const, descPtr, sizeof(struct gnwCtrlStartDescriptor), {},);
    SCLF_GET_VALID_ABS_NAMED(enum gnwCtrlError * const, errorPtr, abs_descPtr->errorPtr, sizeof(enum gnwCtrlError), {},);
    SCLF_GET_VALID_ABS_NAMED(const char * const, pathPtr, abs_descPtr->pathPtr, abs_descPtr->pathLen, {
        *abs_errorPtr = GCE_INVALID_ARGUMENT;
    },);

    size_t fileSizeBytes;
    
    /*
        Get file info
    */

    {
        LOG_START;
        LOG_NBR("Loading file ");
        k_log_logd((data_t){ (ptr_t)abs_pathPtr, abs_descPtr->pathLen });
        LOG_END;
    }
    
    struct gnwFileInfo fileInfo; {
        const enum gnwFileErrorCode err = k_stor_file_getInfo(abs_pathPtr, abs_descPtr->pathLen, &fileInfo);
        if (err != GFEC_NONE) {
            switch (err) {
            case GFEC_NOT_FOUND:
                *abs_errorPtr = GCE_NOT_FOUND;
                LOG_CODE("File not found", err);
                return;
            default:
                *abs_errorPtr = GCE_UNKNOWN;
                LOG_CODE("File info error", err);
                return;
            }
        }
        if (!fileInfo.sizeBytes) {
            *abs_errorPtr = GCE_OPERATION_FAILED;
            LOG_CODE("Unexpected empty file size", 0);
            return;
        }
    }

    /*
        Allocate memory 
    */

    fileSizeBytes = fileInfo.sizeBytes;
    ptr_t filePtr = (ptr_t)0x00300001; /* YOLO */ {

        /*
            Load file
        */

        const enum gnwFileErrorCode err = k_stor_file_load(abs_pathPtr, abs_descPtr->pathLen, filePtr);
        if (err != GFEC_NONE) {
            *abs_errorPtr = GCE_UNKNOWN;
            LOG_CODE("Storage error", err);
            return;
        }
    }

    /* 
        Load executable file header 
    */

    /* 
        Verify header 
    */

    struct elfExpectation exp;
    exp.class = ECLASS_32;
    exp.endianess = EENDIAN_LITTLE;
    exp.type = ETYPE_EXEC;
    exp.architecture = 3;

    if (!elfValidate(filePtr, fileSizeBytes, &exp)) {
        *abs_errorPtr = GCE_HEADER_INVALID;
        LOG_CODE("ELF header validation failure", 0);
        return;
    }
    
    /* 
        Allocate process memory 
    */

    static size_t index = 0;
    const size_t processBinBytes = KiB(512);
    const size_t processStackBytes = KiB(256);
    const size_t processExtraBytes = KiB(256);
    const size_t processMemTotalBytes = processBinBytes + processStackBytes + processExtraBytes;
    const ptr_t dstPtr = GDT_SEGMENT_START(r3code) + (++index * processMemTotalBytes);

    /* 
        Load executable 
    */
    
    enum gnwCtrlError err;
    size_t memBytes;
    addr_t entry;

    err = loadElf(filePtr, fileSizeBytes, dstPtr, &memBytes, &entry);
    if (err != GCE_NONE) {
        *abs_errorPtr = err;
        LOG_CODE("Failed to load ELF", err);
        return;
    }

    /* 
        Spawn process 
    */

    struct k_proc_descriptor desc;

    desc.img = dstPtr;
    desc.entry = entry;
    desc.imgBytes = memBytes;

    enum k_proc_error procErr = k_proc_spawn(&desc);
    if (procErr != PE_NONE) {
        *abs_errorPtr = GCE_OPERATION_FAILED;
        LOG_CODE("Failed to spawn process", procErr);
        return;
    }

    *abs_errorPtr = GCE_NONE;
    return;
}
