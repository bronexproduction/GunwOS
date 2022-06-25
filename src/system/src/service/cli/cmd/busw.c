//
//  busw.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.03.2020.
//

#include "../../../../include/gunwio.h"
#include "../cmdutil.h"
#include "../../terminal/terminal.h"

void cmd_busw(const char * const params) {
    CMD_PARAM_INT(0, uint_16, addr, "bus address")
    CMD_PARAM_INT(1, uint_8, byte, "byte")

    s_trm_puts("Writing byte: ");
    s_trm_putun(byte);
    s_trm_puts(" to: ");
    s_trm_putun(addr);

    wrb(addr, byte);
}
