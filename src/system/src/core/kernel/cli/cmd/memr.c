//
//  memr.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include "../cmdutil.h"
#include "../../../terminal/terminal.h"

void cmd_memr(const char * const params) {
    CMD_PARAM_INT(0, ptr_t, addr, "memory address")

    k_trm_puts("Byte at ");
    k_trm_putun((uint_32)addr);
    k_trm_puts(": ");
    k_trm_putin(*((uint_8 *)addr));
}
