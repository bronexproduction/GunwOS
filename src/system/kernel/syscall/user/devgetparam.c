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

    MEM_VALIDATE_VPTR(procId, vErrorPtr, enum gnwDeviceError,
        { OOPS("Unexpected null pointer",); },
        { OOPS("Reserved zone access violation",); }
    )
    MEM_VALIDATE_VPTR(procId, vQueryPtr, struct gnwDeviceGetParamQuery, {
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
    MEM_VALIDATE_VPTR(procId, vReplyPtr, size_t, {
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
    
    struct gnwDeviceGetParamQuery query;
    MEM_ONTABLE(procId, 
        query = *(vQueryPtr);
    )

    k_dev_getParam(procId, devId, query, vReplyPtr, vErrorPtr);
}