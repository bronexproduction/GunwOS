//
//  keybuf.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.03.2020.
//

#include "keybuf.h"

#include "keymap.h"
#include <log/log.h>

static char kbuf[SCANCODES_MAX];
static struct k_kbf_listener klsn;
static uint_8 kmod;

void k_kbf_register(const struct k_kbf_listener l) {
    klsn = l;
}

uint_8 k_kbf_currModMask() {
    return kmod;
}

void k_kbf_up(const uint_8 k) {
    if (k >= SCANCODES_MAX) {
        LOG_FATAL("Key code exceeds limit")
        return;
    }

    kbuf[k] = 0;
    kmod &= ~(k_kmp_default[k].modifier);

    if (klsn.up) {
        klsn.up(k);
    }
}

void k_kbf_down(const uint_8 k) {
    if (k >= SCANCODES_MAX) {
        LOG_FATAL("Key code exceeds limit")
        return;
    }

    kmod |= k_kmp_default[k].modifier;

    if (klsn.down) {
        klsn.down(k);
    }
}
