//
//  gunwdrv.h
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2021.
//

#ifndef GUNWOS_GUNWDRV_H
#define GUNWOS_GUNWDRV_H

#include <stdgunw/types.h>
#include "uha/gunwuha_system.h"
#include "uha/gunwuha_keyboard.h"
#include "uha/gunwuha_fdc.h"
#include "uha/gunwuha_drivectrl.h"
#include "uha/gunwuha_char.h"
#include "uha/gunwuha_display.h"

/*
    Device driver error codes
*/
enum gnwDriverError {
    NO_ERROR        = 0,
    UNINITIALIZED   = 1,        // Driver initialization failed or driver expected to be initialized before requested operation
    START_FAILED    = 2,        // Driver start failed
    IRQ_CONFLICT    = 3,        // Driver caused IRQ conflict with another driver previously installed
    IRQ_INVALID     = 4,
    LIMIT_REACHED   = 5,
    ISR_MISSING     = 6,
    UNKNOWN         = 0xFFFFFFFF
};

/*
    Device type
*/
enum gnwDeviceType {
    DEV_TYPE_SYSTEM,
    DEV_TYPE_KEYBOARD,
    DEV_TYPE_FDC,
    DEV_TYPE_CHAR_IN,
    DEV_TYPE_CHAR_OUT,
    DEV_TYPE_DISPLAY,
    DEV_TYPE_UNKNOWN    = 0xFFFFFFFF
};

/*
    Hardware-specific API

    Set of routines expected to be provided by hardware driver
    specific for the exact gnwDeviceType
*/
struct gnwDeviceUHA {
    struct gnwDeviceUHA_system system;      // DEV_TYPE_SYSTEM
    struct gnwDeviceUHA_keyboard keyboard;  // DEV_TYPE_KEYBOARD
    struct gnwDeviceUHA_fdc fdc;            // DEV_TYPE_FDC
    struct gnwDeviceUHA_driveCtrl storage;  // for storage devices
    struct gnwDeviceUHA_char_in charIn;     // DEV_TYPE_CHAR_IN
    struct gnwDeviceUHA_char_out charOut;   // DEV_TYPE_CHAR_OUT
    struct gnwDeviceUHA_display display;    // DEV_TYPE_DISPLAY
};

/*  Device driver descriptor

    Provides the kernel with entry points for general device operations
    
    In order to register a device in kernel
    an instance of this type to be prepared
    and passed to k_hal_install()  */
struct gnwDriverDesc {
   
/*  Pointer to device initialization routine

    Called when device is being registered

    Returns 1 on success, 0 otherwise
    
    NOTE: In this phase the interrupts are DISABLED */
    uint_8 (*init)();

/*
    Pointer to device start routine

    Returns 1 on success, 0 otherwise

    Called after device is being initialized */
    uint_8 (*start)();

/*  Pointer to device interrupt service routine

    Expected to be called in event of interrupt from PIC
    meaning that the device triggered an event  */
    void (*isr)();
    
/*  IRQ request number  */
    uint_8 irq;
};

/*
    Device I/O information
*/
struct gnwDeviceIO {
    /*
        Bus base address
    */
    uint_16 busBase;
};

struct gnwDeviceDriver {
    /*
        I/O information
    */
    struct gnwDeviceIO io;

    /*
        Driver descriptor
    */
    struct gnwDriverDesc descriptor;
};

/*
    Device descriptor
*/
struct gnwDeviceDescriptor {
    /*
        Device type
    */
    enum gnwDeviceType type;

    /*
        Hardware-specific API
    */
    struct gnwDeviceUHA api;

    /*
        Device driver
    */
    struct gnwDeviceDriver driver;

    /*
        Device friendly name
    */
    char *name;
};

#endif // GUNWOS_GUNWDRV_H
