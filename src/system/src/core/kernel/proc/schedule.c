//
//  schedule.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"

#include <scl_def.h>

#define GRANULARITY_MS  30000

static size_t intervalCounter = GRANULARITY_MS;
/*
    Process identifier

    Note: 0 is always considered to be kernel
*/
// static size_t currentProcId = 0;

static void k_proc_schedule_switch(const size_t procId) {

}

static size_t k_proc_schedule_select() {
    return 0;
}

void k_proc_schedule_evaluate() {
    k_proc_schedule_switch(k_proc_schedule_select());
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
    
    SYSCALL_KERNEL_FUNC(PROC_SCHED_EVAL);
    SYSCALL_KERNEL_INT;
}
