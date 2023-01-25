//
//  schedule.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"

#define GRANULARITY_MS  100

static size_t intervalCounter = GRANULARITY_MS;

/*
    Note: As launched intermediately via hardware interrupt
          its execution time should be as short as possible
*/
void k_proc_schedule_tick() {
    if (--intervalCounter) {
        return;
    }
     
    intervalCounter = GRANULARITY_MS;
        // CHECK SCHEDULE
}
