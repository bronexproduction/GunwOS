//
//  drvfunc.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#ifndef DRVFUNC_H
#define DRVFUNC_H

#include <driver/gunwdrv.h>
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
__attribute__((naked)) void k_scr_wrb();

/*
    Code - 0x02
    Function - EMIT_VOID
*/
__attribute__((naked)) void k_scr_emit_void(const int_32 type);

/*
    Code - 0x03
    Function - EMIT_U8
*/
__attribute__((naked)) void k_scr_emit_u8(const int_32 type, const uint_8 data);

#endif // DRVFUNC_H
