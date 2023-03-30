//
//  drive.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#ifndef DRIVE_H
#define DRIVE_H

#include <types.h>

#define MAX_DRIVES 8

extern struct k_stor_drive {
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
} k_stor_drives[MAX_DRIVES];

bool k_stor_drive_validateId(const size_t driveId);

#endif // DRIVE_H
