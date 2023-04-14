//
//  gunwstor.h
//  GunwOS
//
//  Created by Artur Danielewski on 29.01.2021.
//

#ifndef GUNWOS_GUNWSTOR_H
#define GUNWOS_GUNWSTOR_H

#include <types.h>

/*
    Path structure:

        volumeId | GNW_ROOT_PATH_SEPARATOR | fileName | GNW_EXTENSION_SEPARATOR (only if file extension present) | fileExtension (optional)

    Correct path examples:

        * 0:FILENAME.EXT
        * 1:FILENAME

    Note: Only flat file structures supported at the moment
*/ 
#define GNW_ROOT_PATH_SEPARATOR ":"
#define GNW_PATH_COMPONENT_SEPARATOR "/"
#define GNW_EXTENSION_SEPARATOR "."

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
    uint_16 sectSizeBytes;
};

/*
    Storage operations error codes
*/
enum gnwStorErrorCode {
    GSEC_NONE = 0,
    GSEC_DRIVE_NOT_PRESENT,
    GSEC_MEDIA_NOT_PRESENT,
    GSEC_API_UNAVAILABLE,
    GSEC_INVALID_FORMAT,
    GSEC_COMMAND_FAILED,
    GSEC_BUFFER_OVERFLOW,
    GSEC_INVALID_ARGUMENT,
    GSEC_UNKNOWN = -1
};

struct gnwStorError {
    enum gnwStorErrorCode code;
    uint_32 internalCode;
};

/*
    Converts LBA (Logical Block Addressing)
    to CHS (Cylinder - Head - Sector)

    Params:
    * lba - LBA address to be converted
    * hpc - Head-per-cylinder count
    * spt - Sector-per-track count
*/
struct gnwStorCHS lba2chs(const uint_32 lba, const uint_8 hpc, const uint_16 spt);

/*
    Converts CHS (Cylinder - Head - Sector)
    to LBA (Logical Block Addressing)

    Params:
    * c - Cylinder value
    * h - Head value
    * s - Sector value
    * hpc - Head-per-cylinder count
    * spt - Sector-per-track count
*/
size_t chs2lba(const uint_16 c, const uint_8 h, const uint_8 s, const uint_8 hpc, const uint_16 spt);

/*
    Returns size in bytes aligned to full sectors for given geometry

    Params:
    * bytes - number of bytes to be aligned to full sectors
    * geometry - drive geometry to be aligned with
*/
size_t sectorAlignedBytes(const size_t bytes, const struct gnwStorGeometry geometry);

#endif // GUNWOS_GUNWSTOR_H
