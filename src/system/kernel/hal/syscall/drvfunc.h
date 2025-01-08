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
uint_8 k_scr_rdb(const uint_16 port); 

/*
    Code - 0x01
    Function - WRB
*/
void k_scr_wrb(const uint_16 port, const uint_8 value);

/*
    Code - 0x02
    Function - EMIT
*/
enum gnwDeviceError k_scr_emit(const struct gnwDeviceEvent * const event);

/*
    Code - 0x03
    Function - MMIO_PLZ
*/
ptr_t k_scr_mmioPlz(const size_t sizeBytes, const addr_t physMemStart, enum gnwDeviceError * const vErrPtr);

#endif // DRVFUNC_H
