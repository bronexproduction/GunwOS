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

enum gnwCtrlError loadFile(const char * const pathPtr,
                           const size_t pathLen,
                           ptr_t filePtr,
                           size_t * fileSizeBytesPtr) {
    if (!pathPtr) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }
    if (!pathLen) {
        OOPS("Unexpected length", GCE_UNKNOWN);
    }
    if (!filePtr) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }
    if (fileSizeBytesPtr) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }

    {
        LOG_START;
        LOG_NBR("Loading file ");
        k_log_logd((data_t){ (ptr_t)pathPtr, pathLen });
        LOG_END;
    }

    /*
        Get file info
    */
    
    struct gnwFileInfo fileInfo; {
        const enum gnwFileErrorCode err = k_stor_file_getInfo(pathPtr, pathLen, &fileInfo);
        if (err != GFEC_NONE) {
            switch (err) {
            case GFEC_NOT_FOUND:
                LOG_CODE("File not found", err);
                return GCE_NOT_FOUND;
            default:
                LOG_CODE("File info error", err);
                return GCE_UNKNOWN;
            }
        }
        if (!fileInfo.sizeBytes) {
            LOG_CODE("Unexpected empty file size", 0);
            return GCE_OPERATION_FAILED;
        }
    }

    /*
        Load file from storage
    */

    const enum gnwFileErrorCode err = k_stor_file_load(pathPtr, pathLen, filePtr);
    if (err != GFEC_NONE) {
        LOG_CODE("Storage error", err);
        return GCE_UNKNOWN;
    }

    *fileSizeBytesPtr = fileInfo.sizeBytes;
    return GCE_NONE;
}

enum gnwCtrlError loadElf(const ptr_t filePtr,
                          const size_t fileSizeBytes,
                          addr_t * const entry) {
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

#define LOG_CODE(MSG, CODE) {                                               \
    LOG_START;                                                              \
    k_log_logd((data_t){ (ptr_t)(descPtr->pathPtr), descPtr->pathLen });    \
    LOG_NBR(" - ");                                                         \
    LOG_NBR(MSG);                                                           \
    if (CODE) {                                                             \
        char loc_code_str[10] = { 0 };                                      \
        int2str(CODE, loc_code_str);                                        \
        LOG_NBR(", code ");                                                 \
        k_log_logd((data_t){ (ptr_t)loc_code_str, 10 });                    \
    }                                                                       \
    LOG_END;                                                                \
}

void k_scr_usr_start(const struct gnwCtrlStartDescriptor * const descPtr) {
    if (!descPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!descPtr->errorPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!descPtr->pathPtr) {
        *(descPtr->errorPtr) = GCE_INVALID_ARGUMENT;
        OOPS("Unexpected nullptr",);
    }
    if (!descPtr->pathLen) {
        *(descPtr->errorPtr) = GCE_INVALID_ARGUMENT;
        OOPS("Unexpected length",);
    }

    /*
        Load file from storage
    */
    
    ptr_t filePtr = (ptr_t)0xfff00001; /* YOLO */ 
    size_t fileSizeBytes; {
        const enum gnwCtrlError err = loadFile(descPtr->pathPtr, descPtr->pathLen, filePtr, &fileSizeBytes);
        if (err != GCE_NONE) {
            *(descPtr->errorPtr) = err;
            return;
        }
    }

    /* 
        Verify header 
    */

    struct elfExpectation exp;
    exp.class = ECLASS_32;
    exp.endianess = EENDIAN_LITTLE;
    exp.type = ETYPE_EXEC;
    exp.architecture = 3;

    if (!elfValidate(filePtr, fileSizeBytes, &exp)) {
        *(descPtr->errorPtr) = GCE_HEADER_INVALID;
        LOG_CODE("ELF header validation failure", 0);
        return;
    }

    /*
        Spawn process
    */

    procId_t procId;
    enum k_proc_error procErr = k_proc_spawn(&procId);
    if (procErr != PE_NONE) {
        *(descPtr->errorPtr) = GCE_OPERATION_FAILED;
        LOG_CODE("Failed to spawn process", procErr);
        return;
    }

#warning TODO START

    /*
        Allocate memory 
    */

    /*
        Prepare process descriptor
    */

    struct k_proc_descriptor desc; {
        /*
            Switch to the new process page table (?)
        */
        
        /*
            Load executable
        */
    
        enum gnwCtrlError err;
    
        err = loadElf(filePtr, fileSizeBytes, &desc.entryLinearAddr);

        /*
            Revert to old page table (?)
        */

        if (err != GCE_NONE) {
            *(descPtr->errorPtr) = err;
            LOG_CODE("Failed to load ELF", err);

            k_proc_cleanup(procId);
            return;
        }
    }

#warning TODO END

    /*
        Hatch process
    */

    procErr = k_proc_hatch(desc, procId);
    if (procErr != PE_NONE) {
        *(descPtr->errorPtr) = GCE_OPERATION_FAILED;
        LOG_CODE("Failed to hatch process", procErr);

        k_proc_cleanup(procId);
        return;
    }

    *(descPtr->errorPtr) = GCE_NONE;
    return;
}
