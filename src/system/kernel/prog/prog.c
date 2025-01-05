//
//  prog.c
//  GunwOS
//
//  Created by Artur Danielewski on 24.11.2024.
//

#include <gunwctrltypes.h>
#include <log/log.h>
#include <storage/file.h>
#include <string.h>
#include <gunwelf.h>
#include <gunwdrv.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <dev/dev.h>
#include <error/panic.h>
#include <gunwdevtypes.h>
#include <hal/paging/paging.h>

#define LOG_CODE(MSG, CODE) {                                       \
    LOG_START;                                                      \
    k_log_logd((data_t){ (ptr_t)pathData.ptr, pathData.bytes });    \
    LOG_NBR(" - ");                                                 \
    LOG_NBR(MSG);                                                   \
    if (CODE) {                                                     \
        char loc_code_str[10] = { 0 };                              \
        int2str(CODE, loc_code_str);                                \
        LOG_NBR(", code ");                                         \
        k_log_logd((data_t){ (ptr_t)loc_code_str, 10 });            \
    }                                                               \
    LOG_END;                                                        \
}

static enum gnwCtrlError loadFile(const data_t pathData,
                                  data_t * const fileData) {
    if (!pathData.ptr) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }
    if (!pathData.bytes) {
        OOPS("Unexpected length", GCE_UNKNOWN);
    }
    if (!fileData) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }
    if (!fileData->ptr) {
        OOPS("Unexpected nullptr", GCE_UNKNOWN);
    }

    {
        LOG_START;
        LOG_NBR("Loading file ");
        k_log_logd((data_t){ (ptr_t)pathData.ptr, pathData.bytes });
        LOG_END;
    }

    /*
        Get file info
    */
    
    struct gnwFileInfo fileInfo; {
        const enum gnwFileErrorCode err = k_stor_file_getInfo(pathData, &fileInfo);
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

    const enum gnwFileErrorCode err = k_stor_file_load(pathData, fileData->ptr);
    if (err != GFEC_NONE) {
        LOG_CODE("Storage error", err);
        return GCE_UNKNOWN;
    }

    fileData->bytes = fileInfo.sizeBytes;
    return GCE_NONE;
}

static enum gnwCtrlError loadElfFile(const data_t pathData,
                                     data_t * const fileData) {
    
    if (!pathData.ptr || !pathData.bytes) {
        return GDE_INVALID_PARAMETER;
    }
    if (!fileData) {
        return GDE_INVALID_PARAMETER;
    }

    /*
        Load file from storage
    */

    *fileData = (data_t){ (ptr_t)0xfff00001 /* YOLO */, 0 };
    enum gnwCtrlError err = loadFile(pathData, fileData);
    if (err != GCE_NONE || !fileData->bytes) {
        return err;
    }

    /* 
        Verify header 
    */

    struct elfExpectation exp;
    exp.class = ECLASS_32;
    exp.endianess = EENDIAN_LITTLE;
    exp.type = ETYPE_EXEC;
    exp.architecture = 3;

    if (!elfValidate(*fileData, &exp)) {
        LOG_CODE("ELF header validation failure", 0);
        return GCE_HEADER_INVALID;
    }

    return GCE_NONE;
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

static enum gnwCtrlError spawn(const data_t fileData,
                               procId_t * spawnedProcId,
                               const enum k_proc_procType procType) {
    
    if (!spawnedProcId) {
        return GCE_INVALID_ARGUMENT;
    }
    if (procType == PT_NONE) {
        return GCE_INVALID_ARGUMENT;
    }

    /*
        Spawn process
    */

    enum k_proc_error procErr = k_proc_spawn(spawnedProcId, procType);
    if (procErr != PE_NONE) {
        return GCE_OPERATION_FAILED;
    }

    /*
        Load executable
    */
    
    struct k_proc_descriptor desc;
    const enum gnwCtrlError err = loadElf(fileData,
                                          *spawnedProcId,
                                          &desc.entryLinearAddr,
                                          &desc.heapLinearAddr);
    if (err != GCE_NONE) {
        k_proc_cleanup(*spawnedProcId);
        return err;
    }

    /*
        Allocate memory for process stack

        Note: YOLO
    */

    const size_t stackSize = KiB(256);
    enum k_mem_error error = k_mem_gimme(*spawnedProcId,
                                         (ptr_t)(0 - MEM_VIRTUAL_RESERVED_KERNEL_MEM - stackSize),
                                         stackSize);
    if (error != ME_NONE) {
        k_proc_cleanup(*spawnedProcId);
        return GCE_OPERATION_FAILED;
    }

    /*
        Hatch process
    */

    procErr = k_proc_hatch(desc, *spawnedProcId);
    if (procErr != PE_NONE) {
        k_proc_cleanup(*spawnedProcId);
        return GCE_OPERATION_FAILED;
    }

    return GCE_NONE;
}

procId_t k_prog_spawnProgram(const procId_t procId,
                             const data_t pathData) {
    
    if (!pathData.ptr) {
        OOPS("Path nullptr", GCE_INVALID_ARGUMENT);
    }
    if (!pathData.bytes) {
        OOPS("Zero-length path", GCE_INVALID_ARGUMENT);
    }

    /*
        Load file from storage
    */

    data_t fileData; {
        const enum gnwCtrlError err = loadElfFile(pathData, &fileData);
        if (err != GCE_NONE || !fileData.ptr || !fileData.bytes) {
            return err;
        }
    }

    /*
        Spawn process
    */

    procId_t spawnedProcId; {
        const enum gnwCtrlError err = spawn(fileData, &spawnedProcId, PT_PROG);
        if (err != GCE_NONE) {
            LOG_CODE("Failed to spawn process", err);
            return err;
        }
    }

    return spawnedProcId;
}

procId_t k_prog_spawnDriver(const procId_t procId,
                            const data_t pathData) {

    if (!pathData.ptr) {
        OOPS("Path nullptr", GCE_INVALID_ARGUMENT);
    }
    if (!pathData.bytes) {
        OOPS("Zero-length path", GCE_INVALID_ARGUMENT);
    }

    /*
        Load file from storage
    */

    data_t fileData; {
        const enum gnwCtrlError err = loadElfFile(pathData, &fileData);
        if (err != GCE_NONE || !fileData.ptr || !fileData.bytes) {
            return err;
        }
    }

    /*
        Get driver descriptor
    */

    size_t deviceDescriptorSizeBytes;
    const struct gnwDeviceDescriptor * const deviceDescriptorPtr = (struct gnwDeviceDescriptor *)elfGetSymbolFileAddr(fileData,
                                                                                                                      "_gnw_device_descriptor",
                                                                                                                      &deviceDescriptorSizeBytes);
    if (!deviceDescriptorPtr || !deviceDescriptorSizeBytes) {
        LOG_CODE("Device descriptor not found in driver file", 0);
        return GCE_HEADER_INVALID;
    }
    if (deviceDescriptorSizeBytes != sizeof(struct gnwDeviceDescriptor)) {
        LOG_CODE("Device descriptor size invalid", 0);
        return GCE_HEADER_INVALID;
    }

    /*
        Spawn operator process
    */

    procId_t spawnedProcId; {
        const enum gnwCtrlError err = spawn(fileData, &spawnedProcId, PT_DRIVER);
        if (err != GCE_NONE) {
            LOG_CODE("Failed to spawn process", err);
            return err;
        }
    }

    /*
        Create device stub
    */

    const enum gnwDriverError installError = k_dev_install_async(deviceDescriptorPtr, spawnedProcId);
    if (installError != GDRE_NONE) {
        k_proc_stop(spawnedProcId);
        
        #warning TODO find a way to return install error to the caller process

        return GCE_UNKNOWN;
    }

    return spawnedProcId;
}
