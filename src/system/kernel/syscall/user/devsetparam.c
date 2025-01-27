//
//  devsetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#include <dev/dev.h>
#include <hal/mem/mem.h>
#include <hal/paging/paging.h>
#include <error/panic.h>

void k_scr_usr_devSetParam(const procId_t procId,
                           const size_t devId, 
                           const struct gnwDeviceSetParamQuery * const vQueryPtr,
                           enum gnwDeviceError * const vErrorPtr) {

    MEM_VALIDATE_VPTR(procId, vErrorPtr, enum gnwDeviceError);
    MEM_VALIDATE_VPTR_E(procId, vQueryPtr, struct gnwDeviceSetParamQuery, {
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
    })
    
    struct gnwDeviceSetParamQuery query;
    MEM_ONTABLE(procId, 
        query = *(vQueryPtr);
    )
    
    k_dev_setParam(procId, devId, query, vErrorPtr);
}
