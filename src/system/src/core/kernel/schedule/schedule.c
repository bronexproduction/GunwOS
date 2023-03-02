//
//  schedule.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "../hal/proc/proc.h"

#include <stdgunw/defs.h>
#include "../queue/queue.h"
#include "../error/panic.h"
#include "../hal/criticalsec/criticalsec.h"

#define GRANULARITY_MS  30000

static size_t executionTimeCounter = GRANULARITY_MS;

/*
    Process identifiers (last, next)

    Note: 0 is always considered to be kernel
*/
static size_t lastProcId = 0;
static size_t currentProcId = 0;
static size_t nextProcId = 0;

static void procSwitch() {
    if (nextProcId) {
        CRITICAL_SECTION_BEGIN;
    }
    else {
        lastProcId = currentProcId;
    }

    if (currentProcId == nextProcId) {
        OOPS("Switching to already running process");
    }
    if (currentProcId & nextProcId) {
        OOPS("Process switch flow inconsistency");
    }
    
    const size_t prevProcId = currentProcId;
    currentProcId = nextProcId;
    if (currentProcId) {
        executionTimeCounter = 0;
    }

    k_proc_switch(prevProcId, currentProcId);
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
        nextProcId = 0;
        procSwitch();
    }
}

void k_proc_schedule_onKernelHandlingFinished() {
    if (currentProcId) {
        OOPS("Unexpected current process identifier");
    }
    if (!nextProcId) {
        return;
    }

    procSwitch();
}

void k_proc_schedule_didSpawn(size_t procId) {
    pTab[procId].state = PS_READY;

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
    if (executionTimeCounter >= GRANULARITY_MS) {
        return;
    }
    if (++executionTimeCounter < GRANULARITY_MS) {
        return;
    }

    k_que_dispatch(schedEvaluate);
}
