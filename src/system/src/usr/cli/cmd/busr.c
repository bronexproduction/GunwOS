//
//  busr.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.03.2020.
//

#include <gunwio.h>
#include "../cmdutil.h"
#include "../../../core/driver/terminal/terminal.h"

void cmd_busr(const char * const params) {
    CMD_PARAM_INT(0, uint_16, addr, "bus address")

    k_trm_puts("Byte from ");
    k_trm_putun(addr);
    k_trm_puts(": ");
    k_trm_putin(rdb(addr));
}
