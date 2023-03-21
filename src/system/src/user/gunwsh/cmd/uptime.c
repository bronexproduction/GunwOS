//
//  uptime.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include <cmdutil.h>
#include <cliio.h>

void cmd_uptime(const char * const params) {
    CMD_NOPARAM

    uint_32 ticks = 0; //k_tmr_getMs();
    uint_32 milliseconds = ticks % 1000; ticks /= 1000;
    uint_32 seconds = ticks % 60; ticks /= 60;
    uint_32 minutes = ticks % 60; ticks /= 60;
    uint_32 hours = ticks % 60; ticks /= 60;
    uint_32 days = ticks;

    user_cli_puts("Uptime: ");
    user_cli_putin(days); user_cli_puts(" days, ");
    user_cli_putin(hours); user_cli_puts(" hours, ");
    user_cli_putin(minutes); user_cli_puts(" minutes, ");
    user_cli_putin(seconds); user_cli_puts(" seconds, ");
    user_cli_putin(milliseconds); user_cli_puts(" milliseconds");
}
