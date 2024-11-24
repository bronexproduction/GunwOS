//
//  devinstall.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.11.2024.
//

#include <_gunwdev.h>
#include <error/panic.h>
#include <hal/mem/mem.h>
#include <log/log.h>
#include <gunwctrltypes.h>
#include <storage/file.h>
#include <string.h>
#include <gunwelf.h>
#include <hal/proc/proc.h>

#define LOG_CODE(MSG, CODE) {                               \
    LOG_START;                                              \
    k_log_logd((data_t){ (ptr_t)pathPtr, pathLen });        \
    LOG_NBR(" - ");                                         \
    LOG_NBR(MSG);                                           \
    if (CODE) {                                             \
        char loc_code_str[10] = { 0 };                      \
        int2str(CODE, loc_code_str);                        \
        LOG_NBR(", code ");                                 \
        k_log_logd((data_t){ (ptr_t)loc_code_str, 10 });    \
    }                                                       \
    LOG_END;                                                \
}

static enum gnwCtrlError loadElf(const data_t fileData,
                                 const procId_t procId,
                                 addr_t * const entry,
                                 addr_t * const heap) {
    if (!fileData.ptr) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }
    if (!fileData.bytes) {
        OOPS("Unexpected zero size", GCE_UNKNOWN);
    }
    if (!k_proc_idIsUser(procId)) {
        OOPS("Invalid process ID", GCE_INVALID_ARGUMENT);
    }
    if (!entry) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }
    if (!heap) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }

    *heap = 0;

    const size_t sectionHeaderEntryCount = elfGetSectionHeaderEntryCount(fileData.ptr);
    for (size_t index = 0; index < sectionHeaderEntryCount; ++index) {
        const struct elfSectionHeaderEntry32 * const sectionHeaderEntry = elfGetSectionHeaderEntryAtIndex(fileData, index); 
        if (!sectionHeaderEntry) {
            OOPS("Unexpected nullptr", GCE_UNKNOWN);
        }
        if (!(sectionHeaderEntry->attributes & ESECATTR_ALLOC)) {
            continue;
        }

        /*
            Assign memory for ALLOC section
        */

        enum k_mem_error err = k_mem_gimme(procId,
                                           (ptr_t)sectionHeaderEntry->virtualAddr,
                                           sectionHeaderEntry->fileSizeBytes);
        if (err != ME_NONE &&
            err != ME_ALREADY_ASSIGNED &&
            err != ME_PART_ALREADY_ASSIGNED) {
            OOPS("Memory assignment error", GCE_UNKNOWN);
        }
        
        *heap = MAX(*heap, sectionHeaderEntry->virtualAddr + sectionHeaderEntry->fileSizeBytes);

        err = k_mem_zero(procId,
                         (ptr_t)sectionHeaderEntry->virtualAddr,
                         sectionHeaderEntry->fileSizeBytes);
        if (err != ME_NONE) {
            OOPS("Memory purification error", GCE_UNKNOWN);
        }

        if (sectionHeaderEntry->type != ESECTYPE_PROGBITS) {
            continue;
        }

        /*
            Copy data for PROGBITS section
        */

        err = k_mem_copy(KERNEL_PROC_ID,
                         fileData.ptr + sectionHeaderEntry->offset,
                         procId,
                         (ptr_t)sectionHeaderEntry->virtualAddr,
                         sectionHeaderEntry->fileSizeBytes);
        if (err != ME_NONE) {
            OOPS("Memory copying error", GCE_UNKNOWN);
        }
    }

    *entry = elfGetEntry(fileData);
    return GCE_NONE;
}

static enum gnwCtrlError loadFile(const char * const pathPtr,
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
    if (!fileSizeBytesPtr) {
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

void k_scr_usr_devInstall(const procId_t procId,
                          const struct gnwDeviceInstallDescriptor * const descPtr) {

#define _FAIL(CTRL_ERROR, DRIVER_ERROR) { *(descPtr->ctrlDesc.errorPtr) = GCE_ ## CTRL_ERROR; *(descPtr->errorPtr) = GDIE_ ## DRIVER_ERROR; }

    if (!descPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr, sizeof(struct gnwDeviceInstallDescriptor))) {
        OOPS("Reserved zone access violation",);
    }
    if (!descPtr->errorPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->errorPtr, sizeof(enum gnwDeviceInstallError))) {
        OOPS("Reserved zone access violation",);
    }
    if (!descPtr->ctrlDesc.errorPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->ctrlDesc.errorPtr, sizeof(enum gnwCtrlError))) {
        OOPS("Reserved zone access violation",);
    }
    if (!descPtr->ctrlDesc.pathPtr) {
        _FAIL(INVALID_ARGUMENT, NONE);
        OOPS("Unexpected nullptr",);
    }
    if (!descPtr->ctrlDesc.pathLen) {
        _FAIL(INVALID_ARGUMENT, NONE);
        OOPS("Unexpected length",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->ctrlDesc.pathPtr, descPtr->ctrlDesc.pathLen)) {
        OOPS("Reserved zone access violation",);
    }

    const char * const pathPtr = descPtr->ctrlDesc.pathPtr;
    const size_t pathLen = descPtr->ctrlDesc.pathLen;

    #warning TODO - pointers accesses as if we're at the process' page (and we might not be)

    /*
        Load file from storage
    */
    
    ptr_t filePtr = (ptr_t)0xfff00001; /* YOLO */ 
    size_t fileSizeBytes; {
        const enum gnwCtrlError err = loadFile(pathPtr, pathLen, filePtr, &fileSizeBytes);
        if (err != GCE_NONE) {
            *(descPtr->ctrlDesc.errorPtr) = err;
            *(descPtr->errorPtr) = GDIE_NONE;
            return;
        }
    }

    /* 
        Verify header 
    */

    const data_t fileData = { filePtr, fileSizeBytes };

    struct elfExpectation exp;
    exp.class = ECLASS_32;
    exp.endianess = EENDIAN_LITTLE;
    exp.type = ETYPE_EXEC;
    exp.architecture = 3;

    if (!elfValidate(fileData, &exp)) {
        _FAIL(HEADER_INVALID, NONE);
        LOG_CODE("ELF header validation failure", 0);
        return;
    }

    /*
        Get and validate driver descriptor
    */

    const addr_t virtDeviceDescriptorAddr = elfGetSymbol(fileData, ".rodata", "_gnw_device_descriptor");
    if (!virtDeviceDescriptorAddr) {
        _FAIL(NOT_FOUND, NONE);
        LOG_CODE("Device descriptor not found in driver file", 0);
        return;
    }

    const struct gnwDeviceDescriptor * const deviceDescriptorPtr = nullptr;
    (void)deviceDescriptorPtr;
    #warning TODO

    /*
        Spawn process
    */

    procId_t spawnedProcId;
    enum k_proc_error procErr = k_proc_spawn(&spawnedProcId);
    if (procErr != PE_NONE) {
        _FAIL(OPERATION_FAILED, NONE);
        LOG_CODE("Failed to spawn process", procErr);
        return;
    }

    /*
        Load executable
    */
    
    struct k_proc_descriptor desc;
    enum gnwCtrlError err = loadElf(fileData,
                                    spawnedProcId,
                                    &desc.entryLinearAddr,
                                    &desc.heapLinearAddr);
    if (err != GCE_NONE) {
        _FAIL(UNKNOWN, NONE);
        LOG_CODE("Failed to load ELF", err);

        k_proc_cleanup(spawnedProcId);
        return;
    }

    /*
        Allocate memory for process stack

        Note: YOLO
    */

    const size_t stackSize = KiB(256);
    enum k_mem_error error = k_mem_gimme(spawnedProcId,
                                         (ptr_t)(0 - MEM_VIRTUAL_RESERVED_KERNEL_MEM - stackSize),
                                         stackSize);
    if (error != ME_NONE) {
        _FAIL(OPERATION_FAILED, NONE);
        LOG_CODE("Failed to allocate stack memory", error);

        k_proc_cleanup(spawnedProcId);
        return;
    }

    /*
        Hatch process
    */

    procErr = k_proc_hatch(desc, spawnedProcId);
    if (procErr != PE_NONE) {
        _FAIL(OPERATION_FAILED, NONE);
        LOG_CODE("Failed to hatch process", procErr);

        k_proc_cleanup(spawnedProcId);
        return;
    }

    /* 
        Perform driver installation and startup
    */

    #warning TODO

    *(descPtr->ctrlDesc.errorPtr) = GCE_NONE;
    *(descPtr->errorPtr) = GDIE_NONE;

#undef _FAIL
}
