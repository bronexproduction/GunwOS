//
//  help.c
//  GunwOS
//
//  Created by Artur Danielewski on 07.10.2022.
//

#include "../../terminal/terminal.h"

void cmd_help(__attribute__((unused)) const char * const params) {
    s_trm_puts("Command not recognized: help - did you mean \"halp\"?");
}
