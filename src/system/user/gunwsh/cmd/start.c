//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.03.2023.
//

#include <cmdutil.h>
#include <cliio.h>
#include <gunwctrl.h>

void cmd_start(const char * const params) {
    CMD_PARAM_STRING(0, name, "Program name")

    const enum gnwCtrlError err = start(name);
    switch (err) {
    case GCE_NONE:
        break;
    case GCE_NOT_FOUND:
        user_cli_puts("Program ");
        user_cli_puts(name);
        user_cli_puts(" not found");
        break;
    default:
        user_cli_puts("Error starting ");
        user_cli_puts("name");
        break;
    }
}
