//
//  usrfunc.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#ifndef USRFUNC_H
#define USRFUNC_H

#include <gunwdrv.h>
#include <gunwfug.h>

/*
    User-level system calls
*/

/*
    Code - 0x01
    Function - DEBUG_PRINT
*/
__attribute__((naked)) void k_scr_debugPrint(const char * const buffer);

/*
    Code - 0x02
    Function - CHAR_OUT_WRITE
*/
__attribute__((naked)) void k_scr_charOutWrite(uint_32 deviceId, const char character);

/*
    Code - 0x03
    Function - EXIT
*/
__attribute__((naked)) void k_scr_exit();

/*
    Code - 0x05
    Function - SLEEPMS
*/
__attribute__((naked)) void k_scr_sleepms();

/*
    Code - 0x08
    Function - DEV_GET_BY_ID
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devGetById(const size_t id, struct gnwDeviceUHADesc * const);

/*
    Code - 0x09
    Function - DEV_GET_BY_TYPE
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devGetByType(const enum gnwDeviceType, struct gnwDeviceUHADesc * const);

/*
    Code - 0x0a
    Function - DEV_ACQUIRE
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devAcquire(const uint_32);

/*
    Code - 0x0b
    Function - DEV_RELEASE
*/
__attribute__((naked)) void k_scr_devRelease(const uint_32);

/*
    Code - 0x0c
    Function - DEV_WRITE
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devWrite(const size_t,
                                                          const void * const);

/*
    Code - 0x0d
    Function - FUG
*/
__attribute__((naked)) void k_scr_fug(enum gnwFugCode);

#endif // USRFUNC_H
