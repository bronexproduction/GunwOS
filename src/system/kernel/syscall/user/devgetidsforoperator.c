//
//  devgetidsforoperator.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.02.2025.
//

#include <mem.h>
#include <dev/dev.h>
#include <hal/proc/proc.h>
#include <hal/mem/mem.h>
#include <hal/paging/paging.h>
#include <error/panic.h>

void k_scr_usr_devGetIdsForOperator(const procId_t procId,
                                    const procId_t operatorProcId,
                                    size_t * const vDeviceIdListStartPtr,
                                    enum gnwDeviceError * const vErrorPtr) {
    
    MEM_VALIDATE_VPTR(procId, vErrorPtr, enum gnwDeviceError);

    const size_t deviceCount = k_dev_countForOperator(operatorProcId);
    if (!deviceCount) {
        MEM_ONTABLE(procId,
            *vErrorPtr = GDE_NOT_FOUND;
        )
    }

    MEM_VALIDATE_VPTR_BUFFER(procId, vDeviceIdListStartPtr, sizeof(size_t) * deviceCount, , {
        MEM_ONTABLE(procId,
            *vErrorPtr = GDE_INVALID_PARAMETER;
        )
    })

    size_t deviceIdListBuffer[deviceCount]; {
        k_dev_identifiersForOperator(operatorProcId, deviceIdListBuffer, deviceCount);
    }

    MEM_ONTABLE(procId,
        memcopy(deviceIdListBuffer, vDeviceIdListStartPtr, sizeof(size_t) * deviceCount);
        *vErrorPtr = GDE_NONE;
    )
}
