//
//  bye.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.12.2023.
//

#include <queue/queue.h>
#include <dev/dev.h>
#include <ipc/ipc.h>
#include <runloop/runloop.h>
#include <hal/proc/proc.h>

void k_scr_usr_bye(const procId_t procId, const int_32 status) {
    k_que_dispatch_arch((fPtr_arch)k_dev_procCleanup, procId);
    k_que_dispatch_arch((fPtr_arch)k_ipc_procCleanup, procId);
    k_que_dispatch_arch((fPtr_arch)k_runloop_procCleanup, procId);
    k_proc_stop(procId);
}
