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

static size_t intervalCounter = GRANULARITY_MS;
/*
    Process identifier

    Note: 0 is always considered to be kernel
*/
static size_t currentProcId = 0;
static size_t nextProcId = 0;

static void k_proc_schedule_prepareRetiKernelStack() {
    // store currentProcId values
    // replace stack with procId values
    #warning TODO
    currentProcId = 0;
}

static void k_proc_schedule_switch(const size_t procId) {
    #warning TODO
    currentProcId = nextProcId;
    CRITICAL_SECTION_END;
}

void k_proc_schedule_intNeedsKernelHandling() {
    if (currentProcId) {
        k_proc_schedule_prepareRetiKernelStack();
    }
}

void k_proc_schedule_onKernelHandlingFinished() {
    if (currentProcId) {
        OOPS("Unexpected current process identifier");
    }
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
