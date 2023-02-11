//
//  proc.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"
#include "schedule.h"

#include "../common/criticalsec.h"
#include "../timer/timer.h"

struct k_proc_process pTab[MAX_PROC];

enum k_proc_error k_proc_spawn(ptr_t imageStart) {
    int pIndex;
    
    CRITICAL_SECTION_BEGIN {
        for (pIndex = 1; pIndex < MAX_PROC; ++pIndex) {
            if (pTab[pIndex].state == PS_NONE) {
                break;
            }
        }

        if (pIndex >= MAX_PROC) {
            CRITICAL_SECTION_END;
            return PE_LIMIT_REACHED;
        }

        pTab[pIndex].state = PS_NEW;
        
    } CRITICAL_SECTION_END;

    pTab[pIndex].state = PS_READY;
    k_proc_schedule_didSpawn();

    return PE_NONE;
}

static void k_proc_prepareKernelProc() {
    pTab[0].state = PS_RUNNING;
}

void k_proc_init() {
    k_proc_prepareKernelProc();

    extern void k_proc_schedule_onTick();
    k_tmr_regMsHandler(k_proc_schedule_onTick);
}
