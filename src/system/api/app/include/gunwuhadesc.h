//
//  gunwuhadesc.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHADESC_H
#define GUNWOS_GUNWUHADESC_H

#include "gunwdevtypes.h"

#include "uha/gunwuha_system_desc.h"
#include "uha/gunwuha_mem_desc.h"
#include "uha/gunwuha_keyboard_desc.h"
#include "uha/gunwuha_mouse_desc.h"
#include "uha/gunwuha_fdc_desc.h"
#include "uha/gunwuha_storctrl_desc.h"
#include "uha/gunwuha_char_desc.h"
#include "uha/gunwuha_display_desc.h"
#include "uha/gunwuha_event_desc.h"

/*
    Hardware-specific descriptor
    Part of UHA available for user-level processes
*/
struct gnwDeviceUHADesc {
    uint_32 identifier;
    enum gnwDeviceType type;
    struct gnwDeviceUHA_system_desc system;       // DEV_TYPE_SYSTEM
    struct gnwDeviceUHA_mem_desc mem;             // DEV_TYPE_MEM
    struct gnwDeviceUHA_keyboard_desc keyboard;   // DEV_TYPE_KEYBOARD
    struct gnwDeviceUHA_mouse_desc mouse;         // DEV_TYPE_MOUSE
    struct gnwDeviceUHA_fdc_desc fdc;             // DEV_TYPE_FDC
    struct gnwDeviceUHA_storCtrl_desc storage;    // DEV_TYPE_STORAGE
    struct gnwDeviceUHA_charIn_desc charIn;       // DEV_TYPE_CHAR_IN
    struct gnwDeviceUHA_charOut_desc charOut;     // DEV_TYPE_CHAR_OUT
    struct gnwDeviceUHA_display_desc display;     // DEV_TYPE_DISPLAY
    struct gnwDeviceUHA_event_desc event;         // event emitting devices
};

#endif // GUNWOS_GUNWUHADESC_H
