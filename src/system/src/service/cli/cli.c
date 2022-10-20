//
//  cli.c
//  GunwOS
//
//  Created by Artur Danielewski on 14.03.2020.
//

#include "../terminal/terminal.h"
#include "../terminal/keybuf.h"
#include "../terminal/keymap.h"
#include "../terminal/charset.h"
#include "../../shared/log/log.h"
#include "../../../../lib/stdgunw/types.h"
#include "../../../../lib/stdgunw/mem.h"
#include "cmdutil.h"

static void onKeyDown(const uint_8 c);

static char cmdBuf[CMD_LEN_MAX + 1];    // Ensure null-termination
static uint_8 cmdBufIndex;

static void prompt();

void s_cli_init() {
    s_kbf_register((struct s_kbf_listener){0, onKeyDown});
    
    s_trm_puts("GunwOS 0.0.1_DEV started. (C) Bronex Production 2022\n\n");

    prompt();
}

static void prompt() {
    memnull(cmdBuf, CMD_LEN_MAX * sizeof(char) + 1);
    cmdBufIndex = 0;

    s_trm_puts("[GunwCLI]: ");
}

extern void (*s_cli_cmdSelector(const char * const cmd))(const char * const);

uint_8 s_cli_command(const char * const cmd) {
    uint_8 i = 0;

    while (cmd[i] && cmd[i] != ' ') ++i;

    const char * const params = cmd[i] ? cmd + i + 1 : 0;

    char cmdOnly[i + 1];
    memnull(cmdOnly, (i + 1) * sizeof(char));
    memcopy(cmd, cmdOnly, i * sizeof(char));

    void (*sel)(const char * const) = s_cli_cmdSelector(cmdOnly);

    if (sel) {
        sel(params);
        return 1;
    }

    return 0;
}

static void exec() {
    if (cmdBufIndex) {
        cmdBuf[CMD_LEN_MAX] = 0;    // Just in case

        if (!s_cli_command(cmdBuf)) {
            s_trm_puts("Command not recognized: \"");
            s_trm_puts(cmdBuf);
            s_trm_puts("\"\n");
        }
    }
    
    s_trm_putc('\n');
    prompt();
}

static void append(const char c) {
    if (!c) {
        return;
    }

    if (c == '\n') {        
        
        // Execute command
        
        s_trm_putc(c);
        exec();
    } else if (c == '\b') { 
        
        // Remove last character
        
        if (!cmdBufIndex) {
            return;
        }

        cmdBuf[--cmdBufIndex] = 0;
        s_trm_putc(c);
    } else {                    
        
        // Append new character
        
        if (cmdBufIndex >= CMD_LEN_MAX) {
            return;
        }

        cmdBuf[cmdBufIndex++] = c;
        s_trm_putc(c);
    }
}

static void onKeyDown(const uint_8 c) {
    if (!k_kmp_defines(k_kmp_default, c)) {
        s_trm_puts("[UNKNOWN SCANCODE: ");
        s_trm_putin(c);
        s_trm_putc(']');
        return;
    }

    uint_8 charCode = *(&(k_kmp_default[c].code) + s_kbf_currModMask());

    if (charCode >= CHARSET_CODES_MAX) {
        LOG_FATAL("Character code over accepted limit")
        return;
    }

    if (k_chs_defines(k_chs_default, charCode)) {
        append(k_chs_default[charCode].character);
    }    
}
