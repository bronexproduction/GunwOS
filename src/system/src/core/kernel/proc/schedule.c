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

#define GRANULARITY_MS  1000

static size_t intervalCounter = GRANULARITY_MS;
/*
    Process identifier

    Note: 0 is always considered to be kernel
*/
static size_t currentProcId = 0;
static size_t nextProcId = 0;

static void k_proc_schedule_switchStack(const size_t procId) {
    #warning TODO
    currentProcId = procId;
}

static void k_proc_schedule_switch(const size_t procId) {
    #warning TODO
    currentProcId = nextProcId;
    CRITICAL_SECTION_END;
}

void k_proc_schedule_intNeedsKernelHandling() {
    k_proc_schedule_switchStack(0);
}

void k_proc_schedule_onKernelHandlingFinished() {
    k_proc_schedule_switch(nextProcId);
}

static size_t k_proc_schedule_select() {
    #warning TODO calculation
    return 0;
}

void k_proc_schedule_evaluate() {
    nextProcId = k_proc_schedule_select();
}

/*
    Note: As launched intermediately via hardware interrupt
          its execution time should be as short as possible
*/
void k_proc_schedule_tick() {
    if (--intervalCounter) {
        return;
    }
     
    intervalCounter = GRANULARITY_MS;
    k_que_dispatch(k_proc_schedule_evaluate);
}
