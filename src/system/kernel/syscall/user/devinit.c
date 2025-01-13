//
//  devinit.c
//  GunwOS
//
//  Created by Artur Danielewski on 05.01.2024.
//

#include <hal/mem/mem.h>
#include <hal/paging/paging.h>
#include <hal/proc/proc.h>
#include <gunwdrvtypes.h>
#include <error/panic.h>
#include <dev/dev.h>

void k_scr_usr_devInit(const procId_t procId,
                       const procId_t operatorProcId,
                       enum gnwDriverError * vErrorPtr) {

    if (!vErrorPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vErrorPtr, sizeof(enum gnwDriverError))) {
        OOPS("Reserved zone access violation",);
    }

    enum gnwDriverError error = k_dev_init_async(operatorProcId, procId, vErrorPtr);

    if (error == GDRE_NOT_FOUND) {
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = GDRE_NONE;
        )
        return;
    } else if (error != GDRE_NONE) {
        MEM_ONTABLE(procId, 
            *(vErrorPtr) = error;
        )
        return;
    } else {
        k_proc_lock(procId, PLT_SYNC);
    }
}
