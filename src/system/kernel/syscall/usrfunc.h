//
//  usrfunc.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#ifndef USRFUNC_H
#define USRFUNC_H

#include <driver/gunwdrv.h>
#include <gunwdev.h>
#include <gunwipc.h>
#include <gunwctrl.h>
#include <src/_gunwrlp.h>

/*
    User-level system calls
*/

/*
    Code - 0x00
    Function - START
*/
__attribute__((naked)) enum gnwCtrlError k_scr_start(const char * const path,
                                                     const size_t pathLen);

/*
    Code - 0x01
    Function - DEBUG_PRINT
*/
__attribute__((naked)) void k_scr_debugPrint(const char * const buffer,
                                             const size_t bufLen);

/*
    Code - 0x02
    Function - DEV_CHAR_WRITE
*/
__attribute__((naked)) void k_scr_devCharWrite(const uint_32 deviceId, 
                                               const char character);

/*
    Code - 0x03
    Function - EXIT
*/
__attribute__((naked)) void k_scr_exit(const int_32 status);

/*
    Code - 0x04
    Function - WAIT_FOR_EVENT
*/
__attribute__((naked)) void k_scr_waitForEvent(const struct gnwRunLoop * const runLoop);

/*
    Code - 0x05
    Function - TIME_MS
*/
__attribute__((naked)) time_t k_scr_timeMs();

/*
    Code - 0x06
    Function - IPC_SEND
*/
__attribute__((naked)) enum gnwIpcError k_scr_ipcSend(const char * const path, const char c);

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
    Function - DEV_MEM_WRITE
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devMemWrite(const size_t,
                                                             const void * const);

/*
    Code - 0x0d
    Function - FUG
*/
__attribute__((naked)) void k_scr_fug(const enum gnwFugCode);

/*
    Code - 0x0e
    Function - DEV_LISTEN
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devListen(const size_t identifier,
                                                           const union gnwEventListener listener,
                                                           const struct gnwRunLoop * const runLoop);

#endif // USRFUNC_H
