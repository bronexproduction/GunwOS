//
//  schedule.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"

#include <stdgunw/defs.h>
#include "../queue/queue.h"
#include "../common/criticalsec.h"
#include "../error/panic.h"

#define GRANULARITY_MS  1000

static size_t intervalCounter = 0;
/*
    Process identifier

    Note: 0 is always considered to be kernel
*/
static size_t lastProcId = 0;
static size_t currentProcId = 0;
static size_t countedProcId = 0;
static size_t nextProcId = 0;

static void procSwitch(const size_t procId) {
    #warning TODO
    if (currentProcId) {
        lastProcId = currentProcId;
    }
    pTab[currentProcId].state = PS_READY;
    intervalCounter = 0;
    countedProcId = currentProcId = nextProcId;
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
    procSwitch(nextProcId);
}

/*
    Note: As launched intermediately via hardware interrupt
          its execution time should be as short as possible
*/
void k_proc_schedule_tick() {
    if (!countedProcId) {
        return;
    }
    if (++intervalCounter < GRANULARITY_MS) {
        return;
    }
     
    countedProcId = 0;
    k_que_dispatch(schedEvaluate);
}
