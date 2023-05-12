//
//  usrfunc.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#ifndef USRFUNC_H
#define USRFUNC_H

#include <gunwdrv.h>
#include <gunwdev.h>
#include <gunwipc.h>
#include <src/_gunwctrl.h>
#include <src/_gunwrlp.h>
#include <gunwfug.h>

/*
    User-level system calls
*/

/*
    Code - 0x00
    Function - START
*/
__attribute__((naked)) void k_scr_start(const struct gnwCtrlStartDescriptor * const);

/*
    Code - 0x01
    Function - none
*/

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
__attribute__((naked)) void k_scr_waitForEvent();

/*
    Code - 0x05
    Function - TIME_MS
*/
__attribute__((naked)) time_t k_scr_timeMs();

/*
    Code - 0x06
    Function - IPC_SEND
*/
__attribute__((naked)) enum gnwIpcError k_scr_ipcSend(const struct gnwIpcSenderQuery * const queryPtr);

/*
    Code - 0x07
    Function - IPC_REGISTER
*/
__attribute__((naked)) enum gnwIpcError k_scr_ipcRegister(const struct gnwIpcHandlerDescriptor * const desc);

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
                                                             const ptr_t,
                                                             const range_addr_t * const);

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
                                                           const gnwDeviceEventListener listener,
                                                           const gnwDeviceEventDecoder decoder);

/*
    Code - 0x0f
    Function - DEV_GET_PARAM
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devGetParam(const size_t deviceId,
                                                             const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                                             size_t * const result);

/*
    Code - 0x10
    Function - DEV_SET_PARAM
*/
__attribute__((naked)) enum gnwDeviceError k_scr_devSetParam(const size_t deviceId,
                                                             const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                                             const size_t value);

/*
    Code - 0x11
    Function - RUNLOOP_GET_ITEM
*/
__attribute__((naked)) enum gnwRunLoopError k_scr_runLoopGetItem(struct gnwRunLoopDispatchItem * const itemPtr);

/*
    Code - 0x12
    Function - RUNLOOP_GET_DATA
*/
__attribute__((naked)) enum gnwRunLoopError k_scr_runLoopGetData(ptr_t dataBufferPtr);

#endif // USRFUNC_H
