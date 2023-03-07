//
//  schedule.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include <stdgunw/defs.h>
#include <hal/proc/proc.h>
#include <queue/queue.h>
#include <error/panic.h>
#include <hal/criticalsec/criticalsec.h>

#define GRANULARITY_MS  30000

static size_t executionTimeCounter = GRANULARITY_MS;

/*
    Process identifiers (last, next)

    Note: KERNEL_PROC_ID for none
*/
static procId_t lastProcId = KERNEL_PROC_ID;
static procId_t nextProcId = KERNEL_PROC_ID;

static procId_t procSelect() {
    /*
        Simple round robin algorithm
    */
    for (size_t i = 0; i < MAX_PROC; ++i) {
        size_t procId = (lastProcId + i + 1) % MAX_PROC;
        if (k_proc_getInfo(procId).state == PS_READY) {
            return procId;
        }
    }
    
    return KERNEL_PROC_ID;
}

static void schedEvaluate() {
    nextProcId = procSelect();
}

void __attribute__((cdecl)) k_proc_schedule_intNeedsKernelHandling(const uint_32 refEsp) {
    k_proc_switchToKernelIfNeeded(refEsp, nextProcId);
}

void k_proc_schedule_onKernelHandlingFinished() {
    if (nextProcId < 0) {
        return;
    }

    CRITICAL_SECTION_BEGIN;
    
    executionTimeCounter = 0;
    k_proc_switch(nextProcId);
    lastProcId = nextProcId;
}

void k_proc_schedule_didSpawn(procId_t procId) {
    if (procId < 0 || procId >= MAX_PROC) {
        OOPS("Invalid spawned process id");
    }
    if (k_proc_getInfo(procId).state != PS_READY) {
        OOPS("Unexpected process state");
    }
    
    if (nextProcId < 0) {
        schedEvaluate();
    }
}

/*
    Note: As launched intermediately via hardware interrupt
          its execution time should be as short as possible
*/
void k_proc_schedule_onTick() {
    #warning hardware interrupts may affect the calculations slightly
    if (executionTimeCounter >= GRANULARITY_MS) {
        return;
    }
    if (++executionTimeCounter < GRANULARITY_MS) {
        return;
    }

    k_que_dispatch(schedEvaluate);
}
