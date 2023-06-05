//
//  cmdreg.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include <stddef.h>

#include <utils.h>
#include <string.h>
#include "cmdutil.h"
#include "cliio.h"

#define CMD_REG(NAME)                              \
    {                                              \
        extern void cmd_##NAME(const char *const); \
        if (isHalp)                                \
            user_cli_puts(" " STR(NAME));             \
        else if (!strcmp(cmd, STR(NAME) "\0"))     \
            return cmd_##NAME;                     \
    }

static void cmd_nop(__attribute__((unused)) const char *const params) {
}

void (*s_cli_cmdSelector(const char *const cmd))(const char *const) {
    char isHalp = !strcmp(cmd, "halp");
    if (isHalp) {
        user_cli_puts("Available commands:");
    }
    CMD_REG(help)
    CMD_REG(start)
    CMD_REG(exit)

    return isHalp ? cmd_nop : NULL;
}
