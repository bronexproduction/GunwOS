//
//  waitforevent.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.03.2023.
//

#include <hal/proc/proc.h>
#include <hal/criticalsec/criticalsec.h>
#include <runloop/runloop.h>

extern void k_scr_usr_waitForEvent(const procId_t procId) {
    CRITICAL_SECTION(
        if (k_runloop_isEmpty(procId)) {
            k_proc_lockIfNeeded(procId);
        }
    )
}
