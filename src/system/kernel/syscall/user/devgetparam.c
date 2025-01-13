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
                           const struct gnwDeviceParamDescriptor * const vParamDescriptorPtr,
                           size_t * const vResultPtr,
                           enum gnwDeviceError * const vErrorPtr) {
    
    if (!vErrorPtr) {
        OOPS("Unexpected null pointer",);
        return;
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vErrorPtr, sizeof(enum gnwDeviceError))) {
        OOPS("Reserved zone access violation",);
        return;
    }
    if (!vParamDescriptorPtr) {
        OOPS("Unexpected null pointer",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vParamDescriptorPtr, sizeof(struct gnwDeviceParamDescriptor))) {
        OOPS("Reserved zone access violation",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    if (!vResultPtr) {
        OOPS("Unexpected null pointer",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vResultPtr, sizeof(size_t))) {
        OOPS("Reserved zone access violation",);
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDE_UNKNOWN;
        )
        return;
    }
    
    struct gnwDeviceParamDescriptor paramDescriptor;
    MEM_ONTABLE(procId, 
        paramDescriptor = *(vParamDescriptorPtr);
    )

    k_dev_getParam(procId, devId, paramDescriptor, vResultPtr, vErrorPtr);
}