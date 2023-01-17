//
//  func.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include <gunwdrv.h>
#include <gunwfug.h>

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
    Code - 0x03
    Function - EXIT
*/
__attribute__((naked)) void k_scr_exit();

/*
    Code - 0x04
    Function - DISPATCH
*/
__attribute__((naked)) void k_scr_dispatch();

/*
    Code - 0x05
    Function - SLEEPMS
*/
__attribute__((naked)) void k_scr_sleepms();

/*
    Code - 0x06
    Function - DEV_INSTALL
*/
__attribute__((naked)) enum gnwDriverError k_scr_devInstall(const struct gnwDeviceDescriptor * const);

/*
    Code - 0x07
    Function - DEV_START
*/
__attribute__((naked)) enum gnwDriverError k_scr_devStart(const struct gnwDeviceDescriptor * const);

/*
    Code - 0x08
    Function - DEV_GET
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devGet(const enum gnwDeviceType, struct gnwDeviceUHADesc * const);

/*
    Code - 0x09
    Function - DEV_ACQUIRE
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devAcquire(const uint_32);

/*
    Code - 0x0a
    Function - FUG
*/
__attribute__((naked)) void k_scr_fug(enum gnwFugCode);
