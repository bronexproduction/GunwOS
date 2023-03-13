//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <gunwctrl.h>
#include <types.h>
#include <string.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <gunwelf.h>

enum gnwCtrlError loadElf(const ptr_t filePtr, const size_t fileSizeBytes, ptr_t destPtr, size_t * const imageBytes) {
    
    // memcopy(imageStart, img, imageSize);

    *imageBytes = elfImageBytes(filePtr);
    if (!*imageBytes || *imageBytes >= fileSizeBytes) {
        return GCE_HEADER_INVALID;
    }

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

    ptr_t filePtr;
    size_t fileSizeBytes;
    if (pathLen == 3 && !strcmpl("cli", (const char *)absPathPtr, pathLen)) {
        filePtr = (ptr_t)0x50000;
        fileSizeBytes = 0xBEB0;
    } else {
        return GCE_NOT_FOUND;
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
    exp.architecture = 0;


    if (!elfValidate(filePtr, fileSizeBytes, &exp)) {
        return GCE_HEADER_INVALID;
    }
    
    /* 
        Allocate process memory 
    */

    const size_t index = 0;
    const ptr_t dstPtr = (ptr_t)(MEM_KERNEL_START + MEM_KERNEL_RESERVED_BYTES + MiB(index * 2 + 1));

    /* 
        Load executable 
    */
    
    enum gnwCtrlError err;
    size_t imgBytes;
    err = loadElf(filePtr, fileSizeBytes, dstPtr, &imgBytes);
    if (err != GCE_NONE) {
        return err;
    }

    /* 
        Spawn process 
    */

    struct k_proc_descriptor desc;

    desc.img = dstPtr;
    desc.imgBytes = imgBytes;

    k_proc_spawn(&desc);

    return GCE_NONE;
}
