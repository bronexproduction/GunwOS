//
//  uptime.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include "../cmdutil.h"
#include "../../terminal/terminal.h"
#include "../../../kernel/timer/timer.h"

void cmd_uptime(const char * const params) {
    CMD_NOPARAM

    uint_32 ticks = k_tmr_getMs();
    uint_32 milliseconds = ticks % 1000; ticks /= 1000;
    uint_32 seconds = ticks % 60; ticks /= 60;
    uint_32 minutes = ticks % 60; ticks /= 60;
    uint_32 hours = ticks % 60; ticks /= 60;
    uint_32 days = ticks;

    s_trm_puts("Uptime: ");
    s_trm_putin(days); s_trm_puts(" days, ");
    s_trm_putin(hours); s_trm_puts(" hours, ");
    s_trm_putin(minutes); s_trm_puts(" minutes, ");
    s_trm_putin(seconds); s_trm_puts(" seconds, ");
    s_trm_putin(milliseconds); s_trm_puts(" milliseconds");
}
