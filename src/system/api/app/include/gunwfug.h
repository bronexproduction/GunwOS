//  gunwfug.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef GUNWOS_GUNWFUG_H
#define GUNWOS_GUNWFUG_H

#ifndef _GUNWAPI_KERNEL

#define CHECKPTR(PTR) { if (!PTR) { fug(FUG_NULLPTR); __builtin_unreachable(); } }

enum gnwFugCode {
    FUG_NULLPTR         = 1,    /* Requested memory access at invalid address - 0 */
    FUG_INCONSISTENT    = 2,    /* Data consistency error */
    FUG_UNDEFINED       = 3,    /* Undefined error */
};

/*
    Sends FUG command to the kernel

    FUG command informs about some kind of terrible failure
    happened inside the caller process
    and it should be terminated
*/
extern void fug(enum gnwFugCode code);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWFUG_H
