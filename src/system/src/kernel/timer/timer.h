//
//  timer.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.03.2020.
//

#include <types.h>

// TODO: Implement 64-bit counters 

time_t k_tmr_getTicks();
time_t k_tmr_getSec();
time_t k_tmr_getMs();

/*
    Sleep function (in seconds)
*/
void k_tmr_sleeps(const unsigned int seconds);

/*
    Sleep function (in milliseconds)
*/
void k_tmr_sleepms(const unsigned int milliseconds);

/*
    Register auxiliary handler for millisecond event
*/
void k_tmr_regMsHandler(void (*handler)());
