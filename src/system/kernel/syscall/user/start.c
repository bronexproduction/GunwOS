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
    
    MEM_VALIDATE_VPTR(procId, descPtr, struct gnwCtrlStartDescriptor);

    byte_t pathBuffer[PATH_LENGTH_LIMIT] = { 0 };
    data_t pathData;
    enum gnwCtrlExecType type;
    MEM_ONTABLE(procId,
        MEM_VALIDATE_VPTR(procId, descPtr->procIdOrErrorPtr, procId_t);
        if (descPtr->pathData.bytes > PATH_LENGTH_LIMIT) {
            *(descPtr->procIdOrErrorPtr) = GCE_INVALID_ARGUMENT;
            OOPS("Unexpected length",);
        }
        MEM_VALIDATE_VPTR_BUFFER(procId, descPtr->pathData.ptr, descPtr->pathData.bytes,, {
            *(descPtr->procIdOrErrorPtr) = GCE_INVALID_ARGUMENT;
        });

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
