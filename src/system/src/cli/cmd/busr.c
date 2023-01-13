//
//  busr.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.03.2020.
//

#include <gunwio.h>
#include "../cmdutil.h"
#include "../../core/driver/terminal/terminal.h"

void cmd_busr(const char * const params) {
    CMD_PARAM_INT(0, uint_16, addr, "bus address")

    c_trm_puts("Byte from ");
    c_trm_putun(addr);
    c_trm_puts(": ");
    c_trm_putin(rdb(addr));
}
