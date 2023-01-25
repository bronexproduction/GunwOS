//
//  proc.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"

#include "../timer/timer.h"

struct k_proc_process pTab[MAX_PROC];

void k_proc_init() {
    extern void k_proc_schedule_tick();
    k_tmr_regMsHandler(k_proc_schedule_tick);
}
