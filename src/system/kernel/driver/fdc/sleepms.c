//
//  sleepms.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.03.2023.
//  
//  Temporary solution
//

#include <timer/timer.h>

#warning to be removed if moved to process

void fdc_sleepms(const size_t ms) {
    const time_t startTime = k_tmr_getMs();
    while(k_tmr_getMs() < startTime + ms);
}