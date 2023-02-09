//
//  proc.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"

#include "../common/criticalsec.h"
#include "../timer/timer.h"

struct k_proc_process pTab[MAX_PROC];

enum k_proc_error k_proc_spawn() {
    int pIndex;
    
    CRITICAL_SECTION_BEGIN {
        for (pIndex = 1; pIndex < MAX_PROC; ++pIndex) {
            if (!pTab[pIndex].identifier) {
                break;
            }
        }

        if (pIndex >= MAX_PROC) {
            CRITICAL_SECTION_END;
            return PE_LIMIT_REACHED;
        }

        pTab[pIndex].state = PS_NEW;
        pTab[pIndex].identifier = pIndex;
        
    } CRITICAL_SECTION_END;

    return PE_NONE;
}

void k_proc_init() {
    k_proc_prepareKernelProc();

    extern void k_proc_schedule_tick();
    k_tmr_regMsHandler(k_proc_schedule_tick);

    extern void TEST_PROC_SPAWN();
    TEST_PROC_SPAWN();
}

static void k_proc_prepareKernelProc() {
    pTab[0].state = PS_RUNNING;
}
