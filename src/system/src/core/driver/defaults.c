//
//  defaults.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.01.2023.
//

/*
    Initialization of default drivers
*/

#include <gunwdrv.h>
#include <gunwscl.h>

#include "../log/log.h"

extern struct gnwDriverDesc s_drv_pit();
extern struct gnwDeviceUHA s_drv_pit_uha();
extern struct gnwDriverDesc s_drv_keyboard();
extern struct gnwDeviceUHA s_drv_keyboard_uha();

static struct gnwDeviceDescriptor createDeviceDescriptor(const enum gnwDeviceType deviceType,
                                                         const struct gnwDeviceUHA uha,
                                                         const uint_16 busBase,
                                                         const struct gnwDriverDesc driverDescriptor,
                                                         char * const name) {
    return (struct gnwDeviceDescriptor) { 
        deviceType,             // Device type
        uha,                    // UHA
        {                       // Driver
            {                   // I/O
                busBase         // Bus base address
            },                                              
            driverDescriptor    // Device driver descriptor
        }, 
        name                    // Device friendly name
    };
}

void c_drv_loadDefaults() {
    
    enum gnwDriverError e;

    /*
        PIT driver for 8253/8254 chip
    */
    struct gnwDeviceDescriptor pit = createDeviceDescriptor(DEV_TYPE_SYSTEM,
                                                            s_drv_pit_uha(),
                                                            0x40,
                                                            s_drv_pit(),
                                                            "8253/8254 Programmable Interrupt Timer");
    e = devInstall(&pit);
    if (e != NO_ERROR) { 
        LOG_FATAL("Fatal error: PIT driver installation failed"); 
        return; 
    }
    else {
        e = devStart(&pit);
        if (e != NO_ERROR) { LOG_FATAL("Fatal error: PIT driver startup failed"); return; }
    }

    /*
        Keyboard controller driver for 8042 PS/2 chip
    */
    struct gnwDeviceDescriptor kbd = createDeviceDescriptor(DEV_TYPE_KEYBOARD,
                                                            s_drv_keyboard_uha(),
                                                            0x60,
                                                            s_drv_keyboard(),
                                                            "8042 PS/2 Controller");
    e = devInstall(&kbd);
    if (e != NO_ERROR) { 
        LOG_FATAL("Fatal error: Keyboard driver installation failed"); 
        return; 
    }
    else {
        e = devStart(&kbd);
        if (e != NO_ERROR) { LOG_FATAL("Fatal error: Keyboard driver startup failed"); return; }
    }
}
