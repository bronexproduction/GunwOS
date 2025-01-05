//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <_gunwctrl.h>
#include <hal/mem/mem.h>
#include <error/panic.h>
#include <prog/prog.h>
#include <log/log.h>
#include <hal/paging/paging.h>
#include <mem.h>

void k_scr_usr_start(const procId_t procId,
                     struct gnwCtrlStartDescriptor * const descPtr) {
    if (!descPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr, sizeof(struct gnwCtrlStartDescriptor))) {
        OOPS("Reserved zone access violation",);
    }

    byte_t pathBuffer[PATH_LENGTH_LIMIT] = { 0 };
    data_t pathData;
    enum gnwCtrlExecType type;
    MEM_ONTABLE(procId,
        if (!descPtr->procIdOrErrorPtr) {
            OOPS("Unexpected nullptr",);
        }
        if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->procIdOrErrorPtr, sizeof(procId_t))) {
            OOPS("Reserved zone access violation",);
        }
        if (!descPtr->pathData.ptr) {
            *(descPtr->procIdOrErrorPtr) = GCE_INVALID_ARGUMENT;
            OOPS("Unexpected nullptr",);
        }
        if (!descPtr->pathData.bytes || descPtr->pathData.bytes > PATH_LENGTH_LIMIT) {
            *(descPtr->procIdOrErrorPtr) = GCE_INVALID_ARGUMENT;
            OOPS("Unexpected length",);
        }
        if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->pathData.ptr, descPtr->pathData.bytes)) {
            OOPS("Reserved zone access violation",);
        }

        memcopy(descPtr->pathData.ptr, pathBuffer, descPtr->pathData.bytes);
        pathData.ptr = pathBuffer;
        pathData.bytes = descPtr->pathData.bytes;
        type = descPtr->type;
    )

    procId_t spawnedProcId;
    switch (type) {
        case GET_PROGRAM:
            spawnedProcId = k_prog_spawnProgram(procId, pathData);   
            break;
        case GET_DRIVER:
            spawnedProcId = k_prog_spawnDriver(procId, pathData);   
            break;
        default:
            LOG("Invalid executable type");
            spawnedProcId = GCE_INVALID_ARGUMENT;
            break;
    }

    MEM_ONTABLE(procId,
        *(descPtr->procIdOrErrorPtr) = spawnedProcId;
    )
}
