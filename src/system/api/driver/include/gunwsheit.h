//  gunwsheit.h
//  GunwOS
//
//  Created by Artur Danielewski on 16.11.2024.
//

#ifndef GUNWOS_GUNWSHEIT_H
#define GUNWOS_GUNWSHEIT_H

enum gnwSheitCode {
    SHEIT_NULLPTR           = 1,    /* Requested memory access at invalid address - 0 */
    SHEIT_INVALID_OPERATION = 2,    /* Invalid operation error */
    SHEIT_NOT_SUPPORTED     = 3,    /* Operation not supported error */
    SHEIT_UNDEFINED         = -1,   /* Undefined error */
};

#ifndef _GUNWAPI_KERNEL

#define CHECKPTR(PTR) { if (!PTR) { sheit(SHEIT_NULLPTR); __builtin_unreachable(); } }

/*
    Sends SHEIT command to the kernel

    SHEIT command informs about some kind of terrible failure
    happened inside the driver
    and it should be terminated
*/
extern void sheit(enum gnwSheitCode code);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWSHEIT_H
