//
//  memr.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include "../cmdutil.h"
#include "../../terminal/terminal.h"

void cmd_memr(const char * const params) {
    CMD_PARAM_INT(0, ptr_t, addr, "memory address")

    s_trm_puts("Byte at ");
    s_trm_putun((uint_32)addr);
    s_trm_puts(": ");
    s_trm_putin(*((uint_8 *)addr));
}
