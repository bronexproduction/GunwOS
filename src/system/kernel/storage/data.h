//
//  data.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#ifndef DATA_H
#define DATA_H

#include <driver/gunwfilesys.h>

#define MAX_DRIVES 8
#define MAX_VOLUMES 16
#define MAX_FILESYS 2

extern struct drive {
    /*
        Entry status
    */
    bool used;

    /*
        Drive controller device identifier
    */
    size_t ctrlId;

    /*
        Drive identifier in controller
    */
    size_t driveId;
} drives[MAX_DRIVES];

extern struct volume {
    /*
        Entry status
    */
    bool used;

    /*
        Drive identifier
    */
    size_t driveId;

    /*
        Volume identifier/index on drive
    */
    size_t volumeId;
    
    /*
        File system identifier
    */
    size_t fileSysId;
} volumes[MAX_VOLUMES];

extern struct fileSystem {
    /*
        Entry status
    */
    bool used;

    /*
        File system descriptor
    */
    struct gnwFileSystemDescriptor desc;
} fileSys[MAX_FILESYS];

#endif // DATA_H
