//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <gunwctrl.h>
#include <types.h>
#include <string.h>
#include <mem.h>
#include <gunwelf.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <storage/file.h>

enum gnwCtrlError loadElf(const ptr_t filePtr, 
                          const size_t fileSizeBytes, 
                          ptr_t destPtr, 
                          size_t * const memBytes, 
                          addr_t * const entry) {
    if (!memBytes) {
        OOPS("Unexpected nullptr");
    }
    if (!entry) {
        OOPS("Unexpected nullptr");
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

    memnull(destPtr, *memBytes);
    for (size_t index = 0; index < elfGetSectionHeaderEntryCount(filePtr); ++index) {
        const struct elfSectionHeaderEntry32 * const entry = elfGetSectionHeaderEntry(filePtr, index, fileSizeBytes); 
        if (!entry) {
            OOPS("Unexpected nullptr");
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
        if (sectionVMemHigh <= sectionVMemLow ||
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

enum gnwCtrlError k_scr_usr_start(const char * const path, const size_t pathLen) {
    if (!path) {
        return GCE_INVALID_ARGUMENT;
    }

    procId_t procId = k_proc_getCurrentId();
    ptr_t absPathPtr = k_mem_absForProc(procId, (const ptr_t)path);

    if (!k_mem_bufInZoneForProc(procId, absPathPtr, pathLen)) {
        return GCE_INVALID_ARGUMENT;
    }

#warning what stage should be queued?

    size_t fileSizeBytes;
    
    /*
        Get file info
    */
        
    struct gnwFileInfo fileInfo; {
        const enum gnwFileErrorCode err = k_stor_file_getInfo(path, pathLen, &fileInfo);
        if (err != GFEC_NONE) {
            switch (err) {
            case GFEC_NOT_FOUND:
                return GCE_NOT_FOUND;
            default:
                return GCE_UNKNOWN;
            }
        }
        if (!fileInfo.sizeBytes) {
            return GCE_OPERATION_FAILED;
        }
    }
         
    /*
        Allocate memory 
    */

    fileSizeBytes = fileInfo.sizeBytes;
    static addr_t fileAddr = 0;
    if (fileAddr) {
        fileAddr += 0x10000; /* YOLO */
    } else {
        fileAddr = (addr_t)0x50000; /* YOLO - assume 64K per binary and ignore wrapping */
    }
    ptr_t filePtr = (ptr_t)fileAddr; {

        /*
            Load file
        */

        const enum gnwFileErrorCode err = k_stor_file_load(path, pathLen, filePtr);
        if (err != GFEC_NONE) {
            return GCE_UNKNOWN;
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
        return GCE_HEADER_INVALID;
    }
    
    /* 
        Allocate process memory 
    */

    const size_t index = 0;
    const size_t processBinBytes = MiB(1);
    const size_t processStackBytes = KiB(512);
    const size_t processExtraBytes = KiB(512);
    const size_t processMemTotalBytes = processBinBytes + processStackBytes + processExtraBytes;
    const ptr_t dstPtr = GDT_SEGMENT_START(r3code) + (index * processMemTotalBytes) + MiB(1);

    /* 
        Load executable 
    */
    
    enum gnwCtrlError err;
    size_t memBytes;
    addr_t entry; 
    err = loadElf(filePtr, fileSizeBytes, dstPtr, &memBytes, &entry);
    if (err != GCE_NONE) {
        return err;
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
        return GCE_OPERATION_FAILED;
    }

    return GCE_NONE;
}
