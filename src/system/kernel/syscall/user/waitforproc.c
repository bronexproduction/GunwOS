//
//  waitforproc.c
//  GunwOS
//
//  Created by Artur Danielewski on 02.03.2026.
//

#include <_gunwctrl.h>
#include <hal/proc/proc.h>

enum gnwCtrlError k_scr_usr_waitForProc(const procId_t procId) {
    
    struct k_proc_lockCondition condition = {
        .reason = PLR_PROC_WAIT
    };

    k_proc_lock(procId, condition);

    return GCE_NONE;
}
