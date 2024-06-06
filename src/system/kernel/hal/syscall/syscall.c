//
//  syscall.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include "syscall.h"

#include <types.h>
#include <mem.h>
#include <error/panic.h>

#include "drvfunc.h"
#include "usrfunc.h"
#include "params.h"

#define DRIVER_SYSCALL_COUNT 3
#define USER_SYSCALL_COUNT 21

/*
    Array of pointers to driver syscall handlers

    Array index corresponds to syscall function code
*/
static void (*syscallReg_DRIVER[DRIVER_SYSCALL_COUNT])() = {
    /* 0x00 */ (void *)k_scr_rdb,
    /* 0x01 */ (void *)k_scr_wrb,
    /* 0x02 */ (void *)k_scr_emit
};

/*
    Array of pointers to user-level syscall handlers

    Array index corresponds to syscall function code
*/
static void (*syscallReg_USER[USER_SYSCALL_COUNT])() = {
    /* 0x00 */ (void *)k_scr_start,
    /* 0x01 */ (void *)k_scr_log,
    /* 0x02 */ (void *)k_scr_devCharWrite,
    /* 0x03 */ (void *)k_scr_bye,
    /* 0x04 */ (void *)k_scr_waitForEvent,
    /* 0x05 */ (void *)k_scr_timeMs,
    /* 0x06 */ (void *)k_scr_ipcSend,
    /* 0x07 */ (void *)k_scr_ipcRegister,
    /* 0x08 */ (void *)k_scr_devGetById,
    /* 0x09 */ (void *)k_scr_devGetByType,
    /* 0x0a */ (void *)k_scr_devAcquire,
    /* 0x0b */ (void *)k_scr_devRelease,
    /* 0x0c */ (void *)k_scr_devMemWrite,
    /* 0x0d */ (void *)k_scr_fug,
    /* 0x0e */ (void *)k_scr_devListen,
    /* 0x0f */ (void *)k_scr_devGetParam,
    /* 0x10 */ (void *)k_scr_devSetParam,
    /* 0x11 */ (void *)k_scr_runLoopGetItem,
    /* 0x12 */ (void *)k_scr_runLoopGetData,
    /* 0x13 */ (void *)k_scr_ipcReply,
    /* 0x14 */ (void *)k_scr_yield,
};

/*
    Driver syscall interrupt (int 0x45) request global service routine

    NOTE: Function number has to be put on top of the caller process stack
*/
void k_scl_syscall_DRIVER(const ptr_t refEsp) {
    ptr_t functionCodePtr = (ptr_t)FUNC_CODE_PTR;
    size_t functionCode = *functionCodePtr;
    
    if (functionCode >= DRIVER_SYSCALL_COUNT) {
        OOPS_NBR("Requested syscall function code over limit");
    } else if (!syscallReg_DRIVER[functionCode]) {
        OOPS_NBR("Syscall function code unavailable");
    } else {
        syscallReg_DRIVER[functionCode](refEsp);
    }
}

/*
    User-level syscall interrupt (int 0x69) request global service routine

    NOTE: Function number has to be put on top of the caller process stack
*/
void k_scl_syscall_USER(const ptr_t refEsp) {
    ptr_t functionCodePtr = (ptr_t)FUNC_CODE_PTR;
    size_t functionCode = *functionCodePtr;
    
    if (functionCode >= USER_SYSCALL_COUNT) {
        OOPS_NBR("Requested syscall function code over limit");
    } else if (!syscallReg_USER[functionCode]) {
        OOPS_NBR("Syscall function code unavailable");
    } else {
        syscallReg_USER[functionCode](refEsp);
    }
}
