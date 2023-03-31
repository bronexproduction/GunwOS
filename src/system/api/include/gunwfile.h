//
//  gunwfile.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#ifndef GUNWOS_GUNWFILE_H
#define GUNWOS_GUNWFILE_H

#include <types.h>

/*
    File information
*/
struct gnwFileInfo {
    /* 
        File size in bytes
    */
    size_t sizeBytes;
};

/*
    File operations error codes
*/
enum gnwFileErrorCode {
    GFEC_NONE               = 0,
    GFEC_NOT_FOUND          = -1,
    GFEC_INVALID_PATH       = -2,
    GFEC_INVALID_PARAMETER  = -3,
    GFEC_UNKNOWN            = -4
};

#endif // GUNWOS_GUNWFILE_H
