//
//  gunwctrltypes.h
//  GunwOS
//
//  Created by Artur Danielewski on 24.11.2024.
//

#ifndef GUNWOS_GUNWCTRLTYPES_H
#define GUNWOS_GUNWCTRLTYPES_H

enum gnwCtrlError {
    GCE_NONE = 0,
    GCE_INVALID_ARGUMENT,
    GCE_HEADER_INVALID,
    GCE_NOT_FOUND,
    GCE_OPERATION_FAILED,
    GCE_UNKNOWN
};

#endif // GUNWOS_GUNWCTRLTYPES_H
