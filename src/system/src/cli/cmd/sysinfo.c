//
//  sysinfo.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.12.2022.
//

#include "../cmdutil.h"
#include "../../core/driver/terminal/terminal.h"
#include "../../core/kernel/mem/mem.h"

void cmd_sysinfo(const char * const params) {
    CMD_NOPARAM

    c_trm_puts("Total system memory: ");
    c_trm_putun(s_mem_totalMemBytes());
    c_trm_puts(" bytes");
    c_trm_putc('\n');
    c_trm_puts("Free system memory: ");
    c_trm_putun(s_mem_freeMemBytes());
    c_trm_puts(" bytes");
}
