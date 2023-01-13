//
//  busw.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.03.2020.
//

#include <gunwio.h>
#include "../cmdutil.h"
#include "../../core/driver/terminal/terminal.h"

void cmd_busw(const char * const params) {
    CMD_PARAM_INT(0, uint_16, addr, "bus address")
    CMD_PARAM_INT(1, uint_8, byte, "byte")

    c_trm_puts("Writing byte: ");
    c_trm_putun(byte);
    c_trm_puts(" to: ");
    c_trm_putun(addr);

    wrb(addr, byte);
}
