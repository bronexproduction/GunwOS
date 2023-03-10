//
//  rdmsr.c
//  GunwOS
//
//  Created by Artur Danielewski on 07.01.2021.
//

#include <cmdutil.h>
#include <cliio.h>

void cmd_rdmsr(const char * const params) {
    CMD_PARAM_INT(0, uint_32, msrAddr, "MSR address")

    user_cli_puts("Value of MSR at ");
    user_cli_putun(msrAddr);
    user_cli_puts(": ");

    uint_64 msrValue = 0; //k_reg_rdmsr(msrAddr);
#warning putun is 32-bit only - may affect precision!
    user_cli_putun(msrValue);
}
