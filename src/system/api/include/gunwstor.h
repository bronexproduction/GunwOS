//
//  gunwstor.h
//  GunwOS
//
//  Created by Artur Danielewski on 29.01.2021.
//

#ifndef GUNWOS_GUNWSTOR_H
#define GUNWOS_GUNWSTOR_H

/*
    CHS address structure
    (Cylinder - Head - Sector)
*/
__attribute__((packed)) struct gnwStorCHS {
    /* 
        Cylinder/Cylinder count
    */
    uint_16 c;

    /* 
        Head/Heads-per-cylinder count
    */
    uint_8 h;

    /* 
        Sector/Sectors per track count
    */
    uint_8 s;
};

/*
    Drive format descriptor
*/
struct gnwStorGeometry {

    /*
        CHS-style format data
    */
    struct gnwStorCHS chs;

    /*
        LBA size
    */
    size_t lba;

    /* 
        Sector size (in bytes)
    */
    uint_16 sectSize;
};

/*
    Storage operations error codes
*/
enum gnwStorErrorCode {
    NONE = 0,
    DRIVE_NOT_PRESENT,
    API_UNAVAILABLE,
    INVALID_FORMAT,
    COMMAND_FAILED,
    BUFFER_OVERFLOW
};

struct gnwStorError {
    enum gnwStorErrorCode code;
    uint_32 internalCode;
};

#endif // GUNWOS_GUNWSTOR_H
