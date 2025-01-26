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
    
    MEM_VALIDATE_VPTR(procId, vErrorPtr, enum gnwDeviceError,
        { OOPS("Unexpected null pointer",); },
        { OOPS("Reserved zone access violation",); }
    )
    MEM_VALIDATE_VPTR(procId, vQueryPtr, struct gnwDeviceMemWriteQuery, {
        OOPS("Unexpected null pointer",); 
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
    }, {
        OOPS("Reserved zone access violation",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
    })

    struct gnwDeviceMemWriteQuery query;
    MEM_ONTABLE(procId,
        query = *(vQueryPtr);
    )

    if (!query.inputBufferRange.sizeBytes) {
        OOPS("Unexpected buffer size",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    MEM_VALIDATE_VPTR_BUFFER(procId, query.buffer, query.inputBufferRange.sizeBytes, {
        OOPS("Unexpected null pointer",); 
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
    }, {
        OOPS("Reserved zone access violation",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
    })

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
