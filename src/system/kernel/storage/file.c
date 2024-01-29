//
//  file.c
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#include "file.h"
#include <defs.h>
#include <mem.h>
#include <string.h>
#include "storage.h"
#include "volume.h"
#include "filesys.h"
#include <error/panic.h>

/*
    File loading

    Note: assumes pre-validated pointers
*/
static enum gnwFileErrorCode loadFile(const size_t volumeId,
                                      const uint_8 * const headerBytes,
                                      const struct gnwFileInfo * const fileInfo,
                                      const struct gnwFileSystemDescriptor fsDesc,
                                      const ptr_t dst) {

    /*
        Load FAT
    */

    range_size_t fatRange = fsDesc.fatRange(headerBytes);
    if (!fatRange.length) {
        return GFEC_UNKNOWN;
    }
    uint_8 fatBytes[fatRange.length]; {
        enum k_stor_error err = k_stor_volume_readBytes(volumeId, fatRange, fatBytes);
        if (err != SE_NONE) {
            return GFEC_UNKNOWN;
        }
        if (!fsDesc.fatVerify(headerBytes, fatBytes)) {
            return GFEC_UNKNOWN;
        }
    }

    struct gnwFileSystemLocation currentLocation = fsDesc.fileStartLocation(headerBytes, fileInfo);
    size_t totalBytes = 0;
    const size_t expectedBytes = fsDesc.allocUnitAlignedBytes(headerBytes, fileInfo->sizeBytes);
    const size_t sectorsPerAllocUnit = fsDesc.sectorsPerAllocUnit(headerBytes);
    do {
        /*
            Calculate number of contiguous sectors for given file
        */
        struct gnwFileSystemLocation lastContiguousLocation = currentLocation;
        do {
            const struct gnwFileSystemLocation nextLocation = fsDesc.nextLocation(headerBytes, fatBytes, lastContiguousLocation);
            const bool isContiguous = fsDesc.isContiguous(headerBytes, lastContiguousLocation, nextLocation);
            if (isContiguous) {
                lastContiguousLocation = nextLocation;
            } else {
                break;
            }
        } while (true);

        if (lastContiguousLocation.allocUnit < currentLocation.allocUnit) {
            return GFEC_UNKNOWN;
        }
        const size_t allocUnitsToRead = lastContiguousLocation.allocUnit - currentLocation.allocUnit + 1;
        const size_t allocUnitsLeft = (expectedBytes - totalBytes) / currentLocation.sizeBytes;
        if (allocUnitsToRead > allocUnitsLeft) {
            return GFEC_UNKNOWN;
        }

        /*
            Read contiguous fragment
        */
        const size_t bytesToBeRead = allocUnitsToRead * currentLocation.sizeBytes;
        uint_8 sectorBuffer[bytesToBeRead];
        enum k_stor_error err = k_stor_volume_readSector(volumeId, currentLocation.sector, allocUnitsToRead * sectorsPerAllocUnit, sectorBuffer);
        if (err != SE_NONE) {
            return GFEC_UNKNOWN;
        }

        memcopy(sectorBuffer,
                dst + totalBytes,
                bytesToBeRead);

        totalBytes += bytesToBeRead;
        currentLocation = fsDesc.nextLocation(headerBytes, fatBytes, lastContiguousLocation);
    } while (fsDesc.isValidForRead(headerBytes, currentLocation) && totalBytes < expectedBytes); 

    if (!fsDesc.isEOF(currentLocation)) {
        return GFEC_UNKNOWN;
    }
    if (expectedBytes != totalBytes) {
        return GFEC_UNKNOWN;
    }

    return GFEC_NONE;
}

/*
    Chain of file loading operations

    Note: assumes pre-validated pointers
*/
static enum gnwFileErrorCode loadFileChain(const char * const path, 
                                           const size_t pathLen, 
                                           struct gnwFileInfo * fileInfo,
                                           const ptr_t dst) {
    /*
        Get volume ID from path
    */

    const size_t rootSeparatorLength = strlen(GNW_ROOT_PATH_SEPARATOR);
    const int_32 idLen = strfindl(path, pathLen, GNW_ROOT_PATH_SEPARATOR, rootSeparatorLength);
    if (idLen <= 0) {
        return GFEC_INVALID_PATH;
    }
    bool pathErr;
    size_t volumeId = str2intl(path, idLen, &pathErr);
    if (pathErr) {
        return GFEC_INVALID_PATH;
    }
    if (pathLen <= (idLen + rootSeparatorLength)) {
        return GFEC_INVALID_PATH;
    }
    if (!k_stor_volume_validateId(volumeId)) {
        return GFEC_INVALID_PATH;
    }

    /*
        Get file name from path
    */

    const size_t filenameBufLength = pathLen - idLen - rootSeparatorLength;
    const char * const filenameBuf = path + idLen + rootSeparatorLength;

    size_t fileSysId = k_stor_volumes[volumeId].fileSysId;
    const struct gnwFileSystemDescriptor fsDesc = k_stor_fileSystems[fileSysId].desc;
    
    char fileName[fsDesc.maxFilenameLength];
    char fileExtension[fsDesc.maxExtensionLength];
    memzero(fileName, fsDesc.maxFilenameLength);
    memzero(fileExtension, fsDesc.maxExtensionLength);

    const size_t extSeparatorLength = strlen(GNW_EXTENSION_SEPARATOR);
    const int_32 extSepOffset = strfindl(filenameBuf, filenameBufLength, GNW_EXTENSION_SEPARATOR, extSeparatorLength);
    if (!extSepOffset || extSepOffset < STR_NOT_FOUND) {
        return GFEC_INVALID_PATH;
    }
    if ((size_t)extSepOffset > fsDesc.maxFilenameLength) {
        return GFEC_INVALID_PATH;
    }
    const size_t extensionLength = (extSepOffset == STR_NOT_FOUND) ? 0 : filenameBufLength - extSepOffset - 1;
    if (extensionLength > fsDesc.maxExtensionLength) {
        return GFEC_INVALID_PATH;
    }

    memcopy(filenameBuf, fileName, extSepOffset);
    memcopy(filenameBuf + extSepOffset + 1, fileExtension, extensionLength);

    uint_8 headerBytes[fsDesc.headerRange.length]; {
        enum k_stor_error err = k_stor_volume_readHeader(volumeId, headerBytes);
        if (err != SE_NONE) {
            return GFEC_UNKNOWN;
        }
    }
    
    range_size_t directoryRange = fsDesc.directoryRange(headerBytes);
    uint_8 directoryBytes[directoryRange.length]; {
        enum k_stor_error err = k_stor_volume_readBytes(volumeId, directoryRange, directoryBytes);
        if (err != SE_NONE) {
            return GFEC_UNKNOWN;
        }
    }

    struct gnwFileInfo localFileInfo;
    if (!fileInfo) {
        fileInfo = &localFileInfo;
    }
    enum gnwFileErrorCode err = fsDesc.fileInfo(headerBytes, directoryBytes, fileName, fileExtension, fileInfo);
    if (err == GFEC_NOT_FOUND) {
        return GFEC_NOT_FOUND;
    } else if (err != GFEC_NONE) {
        return GFEC_UNKNOWN;
    }

    if (!dst) {
        return GFEC_NONE;
    }

    return loadFile(volumeId, headerBytes, fileInfo, fsDesc, dst);
}

enum gnwFileErrorCode k_stor_file_getInfo(const char * const path, 
                                          const size_t pathLen, 
                                          struct gnwFileInfo * const fileInfo) {
    if (!path || !fileInfo) {
        OOPS("Nullptr access", GFEC_UNKNOWN);
    }

    return loadFileChain(path, pathLen, fileInfo, nullptr);
}

enum gnwFileErrorCode k_stor_file_load(const char * const path,
                                       const size_t pathLen,
                                       const ptr_t dst) {
    if (!path || !dst) {
        OOPS("Nullptr access", GFEC_UNKNOWN);
    }

    return loadFileChain(path, pathLen, nullptr, dst);
}