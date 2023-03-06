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
#include <gunwdev.h>

#include "../log/log.h"

extern struct gnwDeviceDescriptor c_drv_display_descriptor();
extern struct gnwDeviceDescriptor c_drv_pit_descriptor();
extern struct gnwDeviceDescriptor c_drv_keyboard_descriptor();

void c_drv_loadDefaults() {
    
    enum gnwDriverError e;
    size_t id;

    #warning it should not exist here for a long time, some drivers should be parts of the kernel
    extern enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const descriptor);
    extern enum gnwDriverError k_dev_start(size_t id);

    /*
        Default text mode display driver
    */
    struct gnwDeviceDescriptor display = c_drv_display_descriptor();
    e = k_dev_install(&id, &display);
    if (e != NO_ERROR) { 
        LOG_FATAL("Fatal error: Display driver installation failed"); 
        return; 
    }
    else {
        e = k_dev_start(id);
        if (e != NO_ERROR) { LOG_FATAL("Fatal error: Display driver startup failed"); return; }
    }
    
    /*
        PIT driver for 8253/8254 chip
    */
    struct gnwDeviceDescriptor pit = c_drv_pit_descriptor();
    e = k_dev_install(&id, &pit);
    if (e != NO_ERROR) { 
        LOG_FATAL("Fatal error: PIT driver installation failed"); 
        return; 
    }
    else {
        e = k_dev_start(id);
        if (e != NO_ERROR) { LOG_FATAL("Fatal error: PIT driver startup failed"); return; }
    }

    /*
        Keyboard controller driver for 8042 PS/2 chip
    */
    struct gnwDeviceDescriptor kbd = c_drv_keyboard_descriptor();
    e = k_dev_install(&id, &kbd);
    if (e != NO_ERROR) { 
        LOG_FATAL("Fatal error: Keyboard driver installation failed"); 
        return; 
    }
    else {
        e = k_dev_start(id);
        if (e != NO_ERROR) { LOG_FATAL("Fatal error: Keyboard driver startup failed"); return; }
    }
}
