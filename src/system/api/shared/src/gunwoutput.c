//
//  gunwoutput.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwoutput.h"
#include "../_include/scl_user.h"
#include <string.h>

enum outputTarget {
    OT_LOG,
    OT_TERMINAL
};

static void output_print(enum outputTarget target, const char * const msg, __builtin_va_list args) {
    switch (target) {
        case OT_LOG:
            // TBD
            SYSCALL_USER_CALL(LOG, msg, strlen(msg), 0, 0);
            break;
        case OT_TERMINAL:
            // TBD
            break;
    }
    
}

void log(const char * const msg, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, msg);
    output_print(OT_LOG, msg, args);
    __builtin_va_end(args);

}

void print(const char * const msg, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, msg);
    output_print(OT_TERMINAL, msg, args);
    __builtin_va_end(args);
}

#endif // _GUNWAPI_KERNEL
