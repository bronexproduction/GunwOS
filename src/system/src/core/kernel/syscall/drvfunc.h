//
//  drvfunc.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#ifndef DRVFUNC_H
#define DRVFUNC_H

#include <gunwdrv.h>
#include <gunwfug.h>

/*
    Driver-level system calls
*/

/*
    Code - 0x01
    Function - RDB
*/
__attribute__((naked)) void k_scr_rdb(); 

/*
    Code - 0x02
    Function - WRB
*/
__attribute__((naked)) void k_scr_wrb();

/*
    Code - 0x06
    Function - DEV_INSTALL
*/
__attribute__((naked)) enum gnwDriverError k_scr_devInstall(size_t * const id, const struct gnwDeviceDescriptor * const);

/*
    Code - 0x07
    Function - DEV_START
*/
__attribute__((naked)) enum gnwDriverError k_scr_devStart(size_t id);

#endif // DRVFUNC_H
