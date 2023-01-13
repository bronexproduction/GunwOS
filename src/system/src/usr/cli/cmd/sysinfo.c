//
//  sysinfo.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.12.2022.
//

#include <gunwio.h>
#include "../cmdutil.h"
#include "../../../core/driver/terminal/terminal.h"
#include "../../../core/kernel/mem/mem.h"

void cmd_sysinfo(const char * const params) {
    k_trm_puts("Total system memory: ");
    k_trm_putun(s_mem_totalMemBytes());
    k_trm_puts(" bytes");
    k_trm_putc('\n');
    k_trm_puts("Free system memory: ");
    k_trm_putun(s_mem_freeMemBytes());
    k_trm_puts(" bytes");
}
