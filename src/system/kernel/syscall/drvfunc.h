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
    Code - 0x00
    Function - RDB
*/
__attribute__((naked)) void k_scr_rdb(); 

/*
    Code - 0x01
    Function - WRB
*/
__attribute__((naked)) void k_scr_wrb(const int_8 data);

/*
    Code - 0x02
    Function - EMIT_VOID
*/
__attribute__((naked)) enum gnwDeviceError k_scr_emit(const struct gnwDeviceEvent * const event);

#endif // DRVFUNC_H
