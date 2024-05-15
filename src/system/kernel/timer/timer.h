//
//  timer.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.03.2020.
//

#include <types.h>

void k_tmr_init();

// TODO: Implement 64-bit counters 

time_t k_tmr_getTicks();
time_t k_tmr_getSec();
time_t k_tmr_getMs();

/*
    Register auxiliary handler for millisecond event
*/
void k_tmr_regMsHandler(void (*handler)());
