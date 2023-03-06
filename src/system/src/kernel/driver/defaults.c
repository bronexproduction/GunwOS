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

#include <dev/dev.h>
#include <log/log.h>

extern struct gnwDeviceDescriptor k_drv_display_descriptor();
extern struct gnwDeviceDescriptor k_drv_pit_descriptor();
extern struct gnwDeviceDescriptor k_drv_keyboard_descriptor();

void k_drv_loadMinimal() {
    
    enum gnwDriverError e;
    size_t id;

    /*
        Default text mode display driver
    */
    struct gnwDeviceDescriptor display = k_drv_display_descriptor();
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
    struct gnwDeviceDescriptor pit = k_drv_pit_descriptor();
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
    struct gnwDeviceDescriptor kbd = k_drv_keyboard_descriptor();
    e = k_dev_install(&id, &kbd);
    if (e != NO_ERROR) { 
        LOG_FATAL("Fatal error: Keyboard driver installation failed"); 
        return; 
    }
    else {
        e = k_dev_start(id);
        if (e != NO_ERROR) { LOG_FATAL("Fatal error: Keyboard driver startup failed"); return; }
    }

    extern void k_trm_init();
    k_trm_init();
}
