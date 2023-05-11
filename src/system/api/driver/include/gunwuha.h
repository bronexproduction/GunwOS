//
//  gunwuha.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_H
#define GUNWOS_GUNWUHA_H

#include <gunwuhadesc.h>
#include <uha/gunwuha_system.h>
#include <uha/gunwuha_mem.h>
#include <uha/gunwuha_keyboard.h>
#include <uha/gunwuha_mouse.h>
#include <uha/gunwuha_fdc.h>
#include <uha/gunwuha_storctrl.h>
#include <uha/gunwuha_char.h>
#include <uha/gunwuha_display.h>
#include <uha/gunwuha_event.h>

/*
    Hardware-specific API
    Not available directly for user-level processes

    Set of routines and information
    expected to be provided by hardware driver
    specific for the exact gnwDeviceType
*/
struct gnwDeviceUHA {
    struct gnwDeviceUHA_system system;      // DEV_TYPE_SYSTEM
    struct gnwDeviceUHA_mem mem;            // DEV_TYPE_MEM
    struct gnwDeviceUHA_keyboard keyboard;  // DEV_TYPE_KEYBOARD
    struct gnwDeviceUHA_mouse mouse;        // DEV_TYPE_MOUSE
    struct gnwDeviceUHA_fdc fdc;            // DEV_TYPE_FDC
    struct gnwDeviceUHA_storCtrl storCtrl;  // DEV_TYPE_STORAGE
    struct gnwDeviceUHA_charIn charIn;      // DEV_TYPE_CHAR_IN
    struct gnwDeviceUHA_charOut charOut;    // DEV_TYPE_CHAR_OUT
    struct gnwDeviceUHA_display display;    // DEV_TYPE_DISPLAY
    struct gnwDeviceUHA_event event;        // event emitting devices
};

#ifdef _GUNWAPI_KERNEL

/*
    Extracts UHA descriptor from UHA structure
*/
extern struct gnwDeviceUHADesc uhaGetDesc(const size_t identifier,
                                          const enum gnwDeviceType type,
                                          const struct gnwDeviceUHA api);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWUHA_H
