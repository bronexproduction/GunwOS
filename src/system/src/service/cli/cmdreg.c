//
//  cmdreg.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.03.2020.
//

#include "../../../../lib/stdgunw/utils.h"
#include "../../../../lib/stdgunw/string.h"
#include "cmdutil.h"

static char commandList[CMD_NUM_MAX][CMD_LEN_MAX];
typedef uint_8 commandCount_t;
static commandCount_t commandCount;
_Static_assert(CMD_NUM_MAX == (commandCount_t)CMD_NUM_MAX, "commandLastIndex value range insufficient - adjust for CMD_NUM_MAX");

#define CMD_REG(NAME) { extern void cmd_ ## NAME (const char * const);  \
                        if (!strcmp(cmd, STR(NAME) "\0")) return cmd_ ## NAME ;}
#define CMD_HELP

void (*s_cli_cmdSelector(const char * const cmd))(const char * const) {
    CMD_REG(busr)
    CMD_REG(busw)
    CMD_REG(memr)
    CMD_REG(rdmsr)
    CMD_REG(uptime)
    
    return 0;
}
