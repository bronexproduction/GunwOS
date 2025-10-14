//
//  keybuf.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.03.2020.
//

#include "keybuf.h"
#include <string.h>
#include <gunwfug.h>
#include <gunwoutput.h>
#include "keymap.h"

static char kbuf[SCANCODES_MAX];
static struct user_cli_kbf_listener klsn;
static uint_8 kmod;

void user_cli_kbf_register(const struct user_cli_kbf_listener l) {
    klsn = l;
}

uint_8 user_cli_kbf_currModMask() {
    return kmod;
}

void user_cli_kbf_up(const uint_8 k) {
    
    logfn("cli_kbf_up - {h}", k);
    
    if (k >= SCANCODES_MAX) {
        logn("cli_kbf_up - scancode limit exceeded");
        fug(FUG_UNDEFINED);
        return;
    }

    kbuf[k] = 0;
    kmod &= ~(user_cli_kmp_default[k].modifier);

    if (klsn.up) {
        klsn.up(k);
    }
}

void user_cli_kbf_down(const uint_8 k) {

    logfn("cli_kbf_down - {h}", k);

    if (k >= SCANCODES_MAX) {
        logn("cli_kbf_down - scancode limit exceeded");
        fug(FUG_UNDEFINED);
        return;
    }

    kmod |= user_cli_kmp_default[k].modifier;

    if (klsn.down) {
        klsn.down(k);
    }
}
