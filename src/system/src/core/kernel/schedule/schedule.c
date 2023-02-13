//
//  schedule.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "../hal/proc/proc.h"

#include <stdgunw/defs.h>
#include "../queue/queue.h"
#include "../common/criticalsec.h"
#include "../error/panic.h"

#define GRANULARITY_MS  1000

static bool countExecutionTime = false;
static size_t executionTimeCounter = 0;

/*
    Process identifiers (last, current, next)

    Note: 0 is always considered to be kernel
*/
static size_t lastProcId = 0;
static size_t currentProcId = 0;
static size_t nextProcId = 0;

static void procSwitch(const size_t procId) {
    #warning analyse the need for critical section (may be useful BUT can't be used if we're called from interrupt)
    if (!procId) {
        lastProcId = currentProcId;
    }

    enum k_proc_error err = k_proc_switch(currentProcId, procId);
    if (err) {
        OOPS("Error switching process");
    }

    executionTimeCounter = 0;
    currentProcId = procId;

    if (currentProcId) {
        countExecutionTime = true;
    }
}

static size_t procSelect() {
    /*
        Simple round robin algorithm
    */
    for (size_t i = 0; i < MAX_PROC - 1; ++i) {
        size_t procId = (lastProcId + i) % (MAX_PROC - 1) + 1;
        if (pTab[procId].state == PS_READY) {
            return procId;
        }
    }
    
    return 0;
}

static void schedEvaluate() {
    nextProcId = procSelect();
}

void k_proc_schedule_intNeedsKernelHandling() {
    if (currentProcId) {
        procSwitch(0);
    }
}

void k_proc_schedule_onKernelHandlingFinished() {
    if (currentProcId) {
        OOPS("Unexpected current process identifier");
    }
    if (!nextProcId) {
        return;
    }

    procSwitch(nextProcId);
}

void k_proc_schedule_didSpawn() {
    if (!nextProcId) {
        schedEvaluate();
    }
}

/*
    Note: As launched intermediately via hardware interrupt
          its execution time should be as short as possible
*/
void k_proc_schedule_onTick() {
    #warning hardware interrupts may affect the calculations slightly
    if (!countExecutionTime) {
        return;
    }
    if (++executionTimeCounter < GRANULARITY_MS) {
        return;
    }

    countExecutionTime = false;
    k_que_dispatch(schedEvaluate);
}
