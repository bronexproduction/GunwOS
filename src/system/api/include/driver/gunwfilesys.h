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
#include <gunwstor.h>

struct gnwFileSystemLocation {
    size_t sector;
    size_t allocUnit;
    size_t sizeBytes;
};

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
        Checks if file allocation table is valid

        Params:
        * headerBytes - file system header data
        * headerBytes - file allocation table data

        Result: `true` if valid, `false` otherwise
    */
    bool (*fatVerify)(const uint_8 * const headerBytes,
                      const uint_8 * const fatBytes);

    /*
        Returns number of the first sector of given file

        Params:
        * headerBytes - file system header data
        * fileInfo - information about the related file
        
        Result: First sector info (does not have to be a valid sector/allocation unit number)
    */
    struct gnwFileSystemLocation (*fileStartLocation)(const uint_8 * const headerBytes,
                                                      const struct gnwFileInfo * const fileInfo);
    
    /*
        Returns number of the next sector of the file

        Params:
        * headerBytes - file system header data
        * fatBytes - file system allocation table data
        * currentSector - number of the last read sector
        
        Result: First sector info (does not have to be a valid sector/allocation unit number)
    */
    struct gnwFileSystemLocation (*nextLocation)(const uint_8 * const headerBytes,
                                                 const uint_8 * const fatBytes,
                                                 const struct gnwFileSystemLocation currentLocation);

    /*
        Returns if the location is valid for read operation

        Params:
        * headerBytes - file system header data
        * location - location to be validated
    */
    bool (*isValidForRead)(const uint_8 * const headerBytes,
                           const struct gnwFileSystemLocation location);

    /*
        Returns if the sector number marks an end of file

        Params:
        * location - location to be checked
    */
    bool (*isEOF)(const struct gnwFileSystemLocation location);

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
        Returns value of `bytes` aligned to match allocation unit size

        Params:
        * headerBytes - file system header data
        * bytes - number of bytes to be aligned

        Result: value of `bytes` with an alignment applied
    */
    size_t (*allocUnitAlignedBytes)(const uint_8 * const headerBytes,
                                    const size_t bytes);

    /*
        Checks if header contains valid
        file system information

        Params:
        * headerBytes - file system header data
        * geometry - associated drive geometry

        Result: true if header conforms to the file system requirements
    */
    bool (*detect)(const uint_8 * const headerBytes,
                   const struct gnwStorGeometry geometry);
};

#endif // GUNWOS_GUNWFILESYS_H
