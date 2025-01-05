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
#include <_gunwipc.h>
#include <_gunwctrl.h>
#include <_gunwdev.h>
#include <_gunwrlp.h>
#include <gunwfug.h>

/*
    User-level system calls
*/

/*
    Code - 0x00
    Function - START
*/
void k_scr_start(const struct gnwCtrlStartDescriptor * const);

/*
    Code - 0x01
    Function - LOG
*/
void k_scr_log(const char * const msg,
               const size_t bytes);

/*
    Code - 0x02
    Function - DEV_CHAR_WRITE
*/
void k_scr_devCharWrite(const uint_32 deviceId, 
                        const char character);

/*
    Code - 0x03
    Function - BYE
*/
void k_scr_bye(const int_32 status);

/*
    Code - 0x04
    Function - WAIT_FOR_EVENT
*/
void k_scr_waitForEvent();

/*
    Code - 0x05
    Function - TIME_MS
*/
time_t k_scr_timeMs();

/*
    Code - 0x06
    Function - IPC_SEND
*/
enum gnwIpcError k_scr_ipcSend(const struct gnwIpcSenderQuery * const queryPtr);

/*
    Code - 0x07
    Function - IPC_REGISTER
*/
enum gnwIpcError k_scr_ipcRegister(const struct gnwIpcHandlerDescriptor * const desc);

/*
    Code - 0x08
    Function - DEV_GET_BY_ID
*/
enum gnwDeviceError k_scr_devGetById(const size_t id,
                                     struct gnwDeviceUHADesc * const);

/*
    Code - 0x09
    Function - DEV_GET_BY_TYPE
*/
enum gnwDeviceError k_scr_devGetByType(const enum gnwDeviceType,
                                       struct gnwDeviceUHADesc * const);

/*
    Code - 0x0a
    Function - DEV_ACQUIRE
*/
enum gnwDeviceError k_scr_devAcquire(const uint_32);

/*
    Code - 0x0b
    Function - DEV_RELEASE
*/
void k_scr_devRelease(const uint_32);

/*
    Code - 0x0c
    Function - DEV_MEM_WRITE
*/
enum gnwDeviceError k_scr_devMemWrite(const size_t,
                                      const ptr_t,
                                      const range_addr_t * const);

/*
    Code - 0x0d
    Function - FUG
*/
void k_scr_fug(const enum gnwFugCode);

/*
    Code - 0x0e
    Function - DEV_LISTEN
*/
enum gnwDeviceError k_scr_devListen(const size_t identifier,
                                    const gnwDeviceEventListener listener,
                                    const gnwDeviceEventDecoder decoder);

/*
    Code - 0x0f
    Function - DEV_GET_PARAM
*/
enum gnwDeviceError k_scr_devGetParam(const size_t deviceId,
                                      const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                      size_t * const result);

/*
    Code - 0x10
    Function - DEV_SET_PARAM
*/
enum gnwDeviceError k_scr_devSetParam(const size_t deviceId,
                                      const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                      const size_t value);

/*
    Code - 0x11
    Function - RUNLOOP_GET_ITEM
*/
enum gnwRunLoopError k_scr_runLoopGetItem(struct gnwRunLoopDispatchItem * const itemPtr);

/*
    Code - 0x12
    Function - RUNLOOP_GET_DATA
*/
enum gnwRunLoopError k_scr_runLoopGetData(ptr_t dataBufferPtr);

/*
    Code - 0x13
    Function - IPC_REPLY
*/
enum gnwIpcError k_scr_ipcReply(const ptr_t replyBufferPtr,
                                const size_t replySizeBytes,
                                const struct gnwIpcReplyInfo * infoPtr);

/*
    Code - 0x14
    Function - YIELD
*/
void k_scr_yield();

/*
    Code - 0x15
    Function - MEM_PLZ
*/
enum gnwMemoryError k_scr_memPlz(const size_t pageCount, const addr_t vStart);

/*
    Code - 0x16
    Function - MEM_THX
*/
enum gnwMemoryError k_scr_memThx(const addr_t vStart);

/*
    Code - 0x17
    Function - DEV_INIT
*/
void k_scr_devInit(const procId_t operatorProcId, enum gnwDriverError * vErrorPtr);

/*
    Code - 0x18
    Function - DEV_START
*/
void k_scr_devStart(const procId_t operatorProcId, enum gnwDriverError * vErrorPtr);

#endif // USRFUNC_H
