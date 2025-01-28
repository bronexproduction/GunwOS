//
//  gunwdrvtypes.h
//  GunwOS
//
//  Created by Artur Danielewski on 05.01.2025.
//

#ifndef GUNWOS_GUNWDRVTYPES_H
#define GUNWOS_GUNWDRVTYPES_H

/*
    Device driver error codes
*/
enum gnwDriverError {
    GDRE_NONE               =  0,
    GDRE_UNINITIALIZED      = -1,   // Driver initialization failed or driver expected to be initialized before requested operation
    GDRE_OPERATION_FAILED   = -2,   // Driver operation failed
    GDRE_IRQ_CONFLICT       = -3,   // Driver caused IRQ conflict with another driver previously installed
    GDRE_IRQ_INVALID        = -4,
    GDRE_LIMIT_REACHED      = -5,
    GDRE_ISR_MISSING        = -6,
    GDRE_INVALID_DESCRIPTOR = -7,
    GDRE_INVALID_ARGUMENT   = -8,
    GDRE_NOT_FOUND          = -9,
    GDRE_NOT_SUPPORTED      = -10,
    GDRE_UNKNOWN            = -11
};

#endif // GUNWOS_GUNWDRVTYPES_H
