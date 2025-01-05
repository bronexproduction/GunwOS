//
//  gunwctrltypes.h
//  GunwOS
//
//  Created by Artur Danielewski on 24.11.2024.
//

#ifndef GUNWOS_GUNWCTRLTYPES_H
#define GUNWOS_GUNWCTRLTYPES_H

#include <types.h>

enum gnwCtrlError {
    GCE_NONE                =  0,
    GCE_INVALID_ARGUMENT    = -1,
    GCE_HEADER_INVALID      = -2,
    GCE_NOT_FOUND           = -3,
    GCE_OPERATION_FAILED    = -4,
    GCE_UNKNOWN             = -5
};
_Static_assert(sizeof(enum gnwCtrlError) == sizeof(procId_t), "enum gnwCtrlError size must match procId_t size");

#endif // GUNWOS_GUNWCTRLTYPES_H
