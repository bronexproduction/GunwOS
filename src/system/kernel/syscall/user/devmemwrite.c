//
//  devmemwrite.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <hal/mem/mem.h>
#include <hal/paging/paging.h>
#include <dev/dev.h>
#include <error/panic.h>

void k_scr_usr_devMemWrite(const procId_t procId,
                           const size_t devId,
                           const struct gnwDeviceMemWriteQuery * const vQueryPtr,
                           enum gnwDeviceError * const vErrorPtr) {

    if (!vErrorPtr) {
        OOPS("Unexpected null pointer",);
        return;
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vErrorPtr, sizeof(enum gnwDeviceError))) {
        OOPS("Reserved zone access violation",);
        return;
    }
    if (!vQueryPtr) {
        OOPS("Unexpected null pointer",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vQueryPtr, sizeof(struct gnwDeviceMemWriteQuery))) {
        OOPS("Reserved zone access violation",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }

    struct gnwDeviceMemWriteQuery query;
    MEM_ONTABLE(procId,
        query = *(vQueryPtr);
    )

    if (!query.buffer) {
        OOPS("Unexpected null pointer",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    if (!query.inputBufferRange.sizeBytes) {
        OOPS("Unexpected buffer size",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)query.buffer, query.inputBufferRange.sizeBytes)) {
        OOPS("Reserved zone access violation",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }

    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError err = k_dev_getById(devId, &desc);
    if (err != GDE_NONE) {
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = err;
        )
        return;
    }
    
    k_dev_writeMem(procId, devId, query, vErrorPtr);
}
