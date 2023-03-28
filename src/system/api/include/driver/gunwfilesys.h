//
//  gunwfilesys.h
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#ifndef GUNWOS_GUNWFILESYS_H
#define GUNWOS_GUNWFILESYS_H

#include <types.h>

/*
    File system descriptor

    In order to register a file system in kernel
    drivers must provide an instance of this type
*/
struct gnwFileSystemDescriptor {
    /*
        Header offset from start of the volume in bytes
    */
    size_t headerOffset;

    /*
        Header size in bytes
    */
    size_t headerSize;

    /*
        Checks if header contains valid
        file system information
    */
    bool (*detect)(const uint_8 * const headerBytes);
};

#endif // GUNWOS_GUNWFILESYS_H
