//
//  waitforevent.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.03.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <src/_gunwrlp.h>
#include <hal/criticalsec/criticalsec.h>

extern void k_scr_usr_waitForEvent(const procId_t procId, const struct gnwRunLoop * const runLoop) {
    const ptr_t absRunLoopPtr = k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)runLoop, sizeof(struct gnwRunLoop));
    if (!absRunLoopPtr) {
        OOPS("Invalid parameter");
    }

    CRITICAL_SECTION(
        if (gnwRunLoopIsEmpty((struct gnwRunLoop *)absRunLoopPtr)) {
            k_proc_lockIfNeeded(procId);
        }
    )
}
