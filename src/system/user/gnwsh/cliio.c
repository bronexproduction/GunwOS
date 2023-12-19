//
//  cliio.c
//  GunwOS
//
//  Created by Artur Danielewski on 07.03.2023.
//

#include "cliio.h"

#include <mem.h>
#include <string.h>
#include <gunwipc.h>
#include <gunwctrl.h>
#include <defs.h>

#define IO_GENERAL_FAILURE -1

static bool outputReady = false;

static int append(char c) {
    while (1) { 
        enum gnwIpcError e = ipcSend("t0",
                                     (data_t){ (ptr_t)&c, sizeof(char) },
                                     nullptr, 0,
                                     GIBF_NONE, 0);
        if (e == GIPCE_NONE) {
            outputReady = true;
            return 1;
        } else if (e == GIPCE_FULL) {
            yield();
            continue;
        } else if (e == GIPCE_NOT_FOUND && !outputReady) {
            yield();
            continue;
        } else {
            return IO_GENERAL_FAILURE;
        }
    }
}

int user_cli_putc(const char c) {
     return append(c);
}

int user_cli_puts(const char * const s) {
    int bWritten = 0;

    while (*(s + bWritten)) {
        if (user_cli_putc(*(s + bWritten)) > 0) {
            bWritten++;
        } else return IO_GENERAL_FAILURE;
    }

    return bWritten;
}

int user_cli_putsl(const char * const s, unsigned int l) {
    if (!l) {
        return 0;
    }

    int count = l;
    int bWritten = 0;

    while (*(s + bWritten) && l--) {
        if (user_cli_putc(*(s + bWritten)) > 0) {
            bWritten++;
        } else return IO_GENERAL_FAILURE;
    }

    if (bWritten != count) {
        return IO_GENERAL_FAILURE;
    }

    return bWritten;
}

// TODO: 64-bit version
int user_cli_putun_base(uint_32 i, uint_8 base) {
    char buffer[32];
    memzero(buffer, 32);
    size_t length = uint2str(i, buffer, base);

    if (!length) return IO_GENERAL_FAILURE;

    return user_cli_putsl(buffer, length);
}

// TODO: 64-bit version
int user_cli_putun(uint_32 i) {
    return user_cli_putun_base(i, 10);
}

// TODO: 64-bit version
int user_cli_putun_h(uint_32 i) {
    return user_cli_putun_base(i, 16);
}

// TODO: 64-bit version
int user_cli_putin(int_32 i) {
    char buffer[32];
    memzero(buffer, 32);
    size_t length = int2str(i, buffer);

    if (!length) return IO_GENERAL_FAILURE;

    return user_cli_putsl(buffer, length);
}
