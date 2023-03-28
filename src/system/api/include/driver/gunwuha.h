//
//  gunwuha.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_H
#define GUNWOS_GUNWUHA_H

#include <gunwuhadesc.h>
#include <driver/uha/gunwuha_system.h>
#include <driver/uha/gunwuha_mem.h>
#include <driver/uha/gunwuha_keyboard.h>
#include <driver/uha/gunwuha_fdc.h>
#include <driver/uha/gunwuha_drivectrl.h>
#include <driver/uha/gunwuha_char.h>
#include <driver/uha/gunwuha_display.h>
#include <driver/uha/gunwuha_event.h>

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
    struct gnwDeviceUHA_fdc fdc;            // DEV_TYPE_FDC
    struct gnwDeviceUHA_driveCtrl storage;  // DEV_TYPE_STORAGE
    struct gnwDeviceUHA_char_in charIn;     // DEV_TYPE_CHAR_IN
    struct gnwDeviceUHA_char_out charOut;   // DEV_TYPE_CHAR_OUT
    struct gnwDeviceUHA_display display;    // DEV_TYPE_DISPLAY
    struct gnwDeviceUHA_event event;        // event emitting devices
};

/*
    Extracts UHA descriptor from UHA structure
*/
static inline struct gnwDeviceUHADesc uhaGetDesc(const size_t identifier, const struct gnwDeviceUHA api) {
    return (struct gnwDeviceUHADesc) {
        identifier,
        api.system.desc,
        api.mem.desc,
        api.keyboard.desc,
        api.fdc.desc,
        api.storage.desc,
        api.charIn.desc,
        api.charOut.desc,
        api.display.desc,
        api.event.desc
    };
}

#endif // GUNWOS_GUNWUHA_H
