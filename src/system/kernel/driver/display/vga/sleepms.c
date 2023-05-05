//
//  sleepms.c
//  GunwOS
//
//  Created by Artur Danielewski on 05.05.2023.
//  
//  Temporary solution
//

#include <gunwtime.h>

#warning to be removed if moved to process

void vga_sleepms(const size_t ms) {
    const time_t startTime = timeMs();
    while(timeMs() < startTime + ms);
}