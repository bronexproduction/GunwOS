//
//  devgetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.04.2023.
//

#include <dev/dev.h>
#include <hal/mem/mem.h>
#include <hal/paging/paging.h>
#include <error/panic.h>

void k_scr_usr_devGetParam(const procId_t procId,
                           const size_t devId, 
                           const struct gnwDeviceGetParamQuery * const vQueryPtr,
                           size_t * const vReplyPtr,
                           enum gnwDeviceError * const vErrorPtr) {
    
    if (!vErrorPtr) {
        OOPS("Unexpected null pointer",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vErrorPtr, sizeof(enum gnwDeviceError))) {
        OOPS("Reserved zone access violation",);
    }
    if (!vQueryPtr) {
        OOPS("Unexpected null pointer",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vQueryPtr, sizeof(struct gnwDeviceGetParamQuery))) {
        OOPS("Reserved zone access violation",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    if (!vReplyPtr) {
        OOPS("Unexpected null pointer",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vReplyPtr, sizeof(size_t))) {
        OOPS("Reserved zone access violation",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    
    struct gnwDeviceGetParamQuery query;
    MEM_ONTABLE(procId, 
        query = *(vQueryPtr);
    )

    k_dev_getParam(procId, devId, query, vReplyPtr, vErrorPtr);
}