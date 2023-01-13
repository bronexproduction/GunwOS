//
//  cmdreg.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include <stddef.h>

#include <stdgunw/utils.h>
#include <stdgunw/string.h>
#include "../core/driver/terminal/terminal.h"
#include "cmdutil.h"

#define CMD_REG(NAME)                              \
    {                                              \
        extern void cmd_##NAME(const char *const); \
        if (isHalp)                                \
            c_trm_puts(" " STR(NAME));             \
        else if (!strcmp(cmd, STR(NAME) "\0"))     \
            return cmd_##NAME;                     \
    }

static void cmd_nop(__attribute__((unused)) const char *const params) {
}

void (*s_cli_cmdSelector(const char *const cmd))(const char *const) {
    char isHalp = !strcmp(cmd, "halp");
    if (isHalp) {
        c_trm_puts("Available commands:");
    }
    CMD_REG(help)
    CMD_REG(rdmsr)
    CMD_REG(uptime)
    CMD_REG(sysinfo)

    return isHalp ? cmd_nop : NULL;
}
