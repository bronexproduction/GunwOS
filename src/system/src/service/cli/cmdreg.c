//
//  cmdreg.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include "../../../../lib/stdgunw/utils.h"
#include "../../../../lib/stdgunw/string.h"

#define CMD_REG(NAME) { extern void cmd_ ## NAME (const char * const);  \
                        if (!strcmp(cmd, STR(NAME) "\0")) return cmd_ ## NAME ;}

void (*s_cli_cmdSelector(const char * const cmd))(const char * const) {
    CMD_REG(busr)
    CMD_REG(busw)
    CMD_REG(memr)
    CMD_REG(rdmsr)
    CMD_REG(uptime)
    
    return 0;
}
