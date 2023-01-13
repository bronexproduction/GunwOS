//
//  uptime.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include "../cmdutil.h"
#include "../../core/driver/terminal/terminal.h"
#include "../../core/kernel/timer/timer.h"

void cmd_uptime(const char * const params) {
    CMD_NOPARAM

    uint_32 ticks = k_tmr_getMs();
    uint_32 milliseconds = ticks % 1000; ticks /= 1000;
    uint_32 seconds = ticks % 60; ticks /= 60;
    uint_32 minutes = ticks % 60; ticks /= 60;
    uint_32 hours = ticks % 60; ticks /= 60;
    uint_32 days = ticks;

    c_trm_puts("Uptime: ");
    c_trm_putin(days); c_trm_puts(" days, ");
    c_trm_putin(hours); c_trm_puts(" hours, ");
    c_trm_putin(minutes); c_trm_puts(" minutes, ");
    c_trm_putin(seconds); c_trm_puts(" seconds, ");
    c_trm_putin(milliseconds); c_trm_puts(" milliseconds");
}
