//
//  uptime.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include "../cmdutil.h"
#include "../../../terminal/terminal.h"
#include "../../timer/timer.h"

void cmd_uptime(const char * const params) {
    CMD_NOPARAM

    uint_32 ticks = k_tmr_getMs();
    uint_32 milliseconds = ticks % 1000; ticks /= 1000;
    uint_32 seconds = ticks % 60; ticks /= 60;
    uint_32 minutes = ticks % 60; ticks /= 60;
    uint_32 hours = ticks % 60; ticks /= 60;
    uint_32 days = ticks;

    k_trm_puts("Uptime: ");
    k_trm_putin(days); k_trm_puts(" days, ");
    k_trm_putin(hours); k_trm_puts(" hours, ");
    k_trm_putin(minutes); k_trm_puts(" minutes, ");
    k_trm_putin(seconds); k_trm_puts(" seconds, ");
    k_trm_putin(milliseconds); k_trm_puts(" milliseconds");
}
