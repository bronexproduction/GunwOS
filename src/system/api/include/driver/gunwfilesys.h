//
//  gunwfilesys.h
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#ifndef GUNWOS_GUNWFILESYS_H
#define GUNWOS_GUNWFILESYS_H

#include <types.h>
#include <gunwfile.h>

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
        Maximum length of file name
    */
    size_t maxFilenameLength;
    
    /*
        Maximum length of file extension
    */
    size_t maxExtensionLength;

    /*
        Returns file directory range (values in bytes)
        based on header

        Params:
        * headerBytes - file system header data

        Result: range of file system directory in bytes
    */
    range_size_t (*directoryRange)(const uint_8 * const headerBytes);

    /*
        Returns file allocation table range (values in bytes)
        based on header

        Params:
        * headerBytes - file system header data

        Result: range of file system allocation table in bytes
    */
    range_size_t (*fatRange)(const uint_8 * const headerBytes);

    /*
        Returns file information for given filename and directory data

        Params:
        * headerBytes - file system header data
        * directoryBytes - file system directory data
        * name - file name (assumed size == maxFilenameLength)
        * extension - file extension (assumed size == maxExtensionLength)
        * fileInfo - result file information
        
        Result: error code, if any, otherwise GFEC_NONE
    */
    enum gnwFileErrorCode (*fileInfo)(const uint_8 * const headerBytes,
                                      const uint_8 * const directoryBytes, 
                                      const char * const name, 
                                      const char * const extension, 
                                      struct gnwFileInfo * const fileInfo);

    /*
        Checks if header contains valid
        file system information

        Params:
        * headerBytes - file system header data

        Result: true if header conforms to the file system requirements
    */
    bool (*detect)(const uint_8 * const headerBytes);
};

#endif // GUNWOS_GUNWFILESYS_H
