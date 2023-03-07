//
//  cliio.c
//  GunwOS
//
//  Created by Artur Danielewski on 07.03.2023.
//

#include "cliio.h"

#include <stdgunw/mem.h>
#include <stdgunw/string.h>
#include <gunwio.h>

#define IO_GENERAL_FAILURE -1

bool user_cli_charOutAttached = false;
size_t user_cli_charOutIdentifier = 0;

static int append(const char c) {
    if (!user_cli_charOutAttached) {
        return IO_GENERAL_FAILURE;
    }
    enum gnwDeviceError e = charOutWrite(user_cli_charOutIdentifier, c);
    if (e != GDE_NONE) {
        return IO_GENERAL_FAILURE;
    }

    return 1;
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
    memnull(buffer, 32);
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
    memnull(buffer, 32);
    size_t length = int2str(i, buffer);

    if (!length) return IO_GENERAL_FAILURE;

    return user_cli_putsl(buffer, length);
}
