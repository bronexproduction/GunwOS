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
        Header range (values in bytes)
    */
    range_size_t headerRange;

    /*
        Returns file directory range (values in bytes)
        based on header 
    */
    range_size_t (*directoryRange)(const uint_8 * const headerBytes);

    /*
        Checks if header contains valid
        file system information
    */
    bool (*detect)(const uint_8 * const headerBytes);
};

#endif // GUNWOS_GUNWFILESYS_H
