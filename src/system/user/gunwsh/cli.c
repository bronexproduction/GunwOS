//
//  cli.c
//  GunwOS
//
//  Created by Artur Danielewski on 14.03.2020.
//

#include "keybuf.h"
#include "keymap.h"
#include "charset.h"
#include <types.h>
#include <mem.h>
#include <gunwdev.h>
#include <gunwkeyboard.h>
#include <gunwfug.h>
#include <gunwrlp.h>

#include "cliio.h"
#include "cmdutil.h"

static char cmdBuf[CMD_LEN_MAX + 1];    // Ensure null-termination
static uint_8 cmdBufIndex;

static void prompt();

extern void (*s_cli_cmdSelector(const char * const cmd))(const char * const);

uint_8 s_cli_command(const char * const cmd) {
    uint_8 i = 0;

    while (cmd[i] && cmd[i] != ' ') ++i;

    const char * const params = cmd[i] ? cmd + i + 1 : 0;

    char cmdOnly[i + 1];
    memzero(cmdOnly, (i + 1) * sizeof(char));
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
            user_cli_puts("Command not recognized: \"");
            user_cli_puts(cmdBuf);
            user_cli_puts("\"\n");
        }
    }
    
    user_cli_putc('\n');
    prompt();
}

static void append(const char c) {
    if (!c) {
        return;
    }

    if (c == '\n') {        
        
        // Execute command
        
        user_cli_putc(c);
        exec();
    } else if (c == '\b') { 
        
        // Remove last character
        
        if (!cmdBufIndex) {
            return;
        }

        cmdBuf[--cmdBufIndex] = 0;
        user_cli_putc(c);
    } else {                    
        
        // Append new character
        
        if (cmdBufIndex >= CMD_LEN_MAX) {
            return;
        }

        cmdBuf[cmdBufIndex++] = c;
        user_cli_putc(c);
    }
}

static void onKeyDown(const uint_8 c) {
    if (!user_cli_kmp_defines(user_cli_kmp_default, c)) {
        user_cli_puts("[UNKNOWN SCANCODE: ");
        user_cli_putin(c);
        user_cli_putc(']');
        return;
    }

    uint_8 charCode = *(&(user_cli_kmp_default[c].code) + user_cli_kbf_currModMask());

    if (charCode >= CHARSET_CODES_MAX) {
        // OOPS("Character code over accepted limit");
        fug(FUG_UNDEFINED);
        return;
    }

    if (user_cli_chs_defines(user_cli_chs_default, charCode)) {
        append(user_cli_chs_default[charCode].character);
    }    
}

static GNW_KEYBOARD_EVENT_LISTENER(onKeyboardEvent) {
    if (event->code == GKEC_KEY_DOWN) {
        onKeyDown(*event->key);
    }
}

static void prompt() {
    memzero(cmdBuf, CMD_LEN_MAX * sizeof(char) + 1);
    cmdBufIndex = 0;

    user_cli_puts("[GunwSH]: ");
}

static void s_cli_init() {

    // /* 
    //     Attach to character output
    // */
    // struct gnwDeviceUHADesc charOutDesc;
    enum gnwDeviceError e = GDE_NONE;
    // e = devGetByType(DEV_TYPE_CHAR_OUT, &charOutDesc);

    // if (e) {
    //     // OOPS("Error retrieving available character output device");
    //     fug(FUG_UNDEFINED);
    // }

    // e = devAcquire(charOutDesc.identifier);
    // if (e) {
    //     // OOPS("Unable to attach to character output");
    //     fug(FUG_UNDEFINED);
    // }
    
    /* 
        Attach keyboard listener
    */
    struct gnwDeviceUHADesc keyboardDesc;
    e = devGetByType(DEV_TYPE_KEYBOARD, &keyboardDesc);

    if (e) {
        // devRelease(charOutDesc.identifier);
        // OOPS("Error retrieving available keyboard");
        fug(FUG_UNDEFINED);
    }

    e = devAcquire(keyboardDesc.identifier);
    if (e) {
        // devRelease(charOutDesc.identifier);
        // OOPS("Unable to attach to keyboard");
        fug(FUG_UNDEFINED);
    }

    e = devListen(keyboardDesc.identifier, (gnwDeviceEventListener)onKeyboardEvent);
    if (e) {
        // devRelease(charOutDesc.identifier);
        devRelease(keyboardDesc.identifier);
        // OOPS("Unable to attach keyboard listener");
        fug(FUG_UNDEFINED);
    }

    /*
        Configure variables
    */
    // user_cli_charOutAttached = true;
    // user_cli_charOutIdentifier = charOutDesc.identifier;

    user_cli_kbf_register((struct user_cli_kbf_listener){0, onKeyDown});
    
#warning IO_GENERAL_FAILURE not handled at all

    user_cli_puts("GunwOS 0.0.5_DEV started. (C) Bronex Production 2023\n\n");

    prompt();
}

void dupa() {
    s_cli_init();

    runLoopStart();
}
