//
//  proc.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"

#include <stdgunw/mem.h>

#include "../../schedule/schedule.h"
#include "../criticalsec/criticalsec.h"
#include "../../timer/timer.h"

struct k_proc_process pTab[MAX_PROC];

enum k_proc_error k_proc_spawn(const struct k_proc_descriptor * const descriptor) {
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

    memnull(&pTab[pIndex].cpuState, sizeof pTab[pIndex].cpuState);

    pTab[pIndex].cpuState.esp = (uint_32)descriptor->stack;
    pTab[pIndex].cpuState.eip = (uint_32)descriptor->img;

    k_proc_schedule_didSpawn(pIndex);

    return PE_NONE;
}

void k_proc_switch(const size_t currentProcId, const size_t nextProcId, const bool isr) {
    pTab[currentProcId].state = PS_READY;
    pTab[nextProcId].state = PS_RUNNING;

    // ISR puts 12 bytes on the stack?

    if (isr) {
        // inside an ISR
        // switching from R3 to kernel
    } else {
        // not inside an ISR
        // switching from kernel to R3
        CRITICAL_SECTION_END;
        __asm__ volatile ("iret");
    }
}

static void k_proc_prepareKernelProc() {
    pTab[0].state = PS_RUNNING;
}

void k_proc_init() {
    k_proc_prepareKernelProc();

    extern void k_proc_schedule_onTick();
    k_tmr_regMsHandler(k_proc_schedule_onTick);
}
