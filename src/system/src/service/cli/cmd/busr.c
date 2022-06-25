//
//  busr.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.03.2020.
//

#include "../../../../include/gunwio.h"
#include "../cmdutil.h"
#include "../../terminal/terminal.h"

void cmd_busr(const char * const params) {
    CMD_PARAM_INT(0, uint_16, addr, "bus address")

    s_trm_puts("Byte from ");
    s_trm_putun(addr);
    s_trm_puts(": ");
    s_trm_putin(rdb(addr));
}
