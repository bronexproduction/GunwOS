//
//  rdmsr.c
//  GunwOS
//
//  Created by Artur Danielewski on 07.01.2021.
//

#include "../cmdutil.h"
#include "../../terminal/terminal.h"
#include "../../../kernel/hal/io/reg.h"

void cmd_rdmsr(const char * const params) {
    CMD_PARAM_INT(0, uint_32, msrAddr, "MSR address")

    s_trm_puts("Value of MSR at ");
    s_trm_putun(msrAddr);
    s_trm_puts(": ");

    uint_64 msrValue = k_reg_rdmsr(msrAddr);
#warning putun is 32-bit only - may affect precision!
    s_trm_putun(msrValue);
}
