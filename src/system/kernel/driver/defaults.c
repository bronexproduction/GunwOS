//
//  defaults.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.01.2023.
//

/*
    Initialization of default drivers
*/

#include <driver/gunwdrv.h>
#include <gunwdev.h>

#include <dev/dev.h>
#include <error/panic.h>

#define MSG_INSTALL_FAIL(NAME) "Fatal error: ## NAME ## driver installation failed"
#define MSG_START_FAIL(NAME) "Fatal error: ## NAME ## driver startup failed"
#define MSGS_FAIL(NAME) MSG_INSTALL_FAIL(NAME), MSG_START_FAIL(NAME)

static void loadSingle(struct gnwDeviceDescriptor (*descProvider)(),
                       const char * const installFailureMsg,
                       const char * const startFailureMsg) {
    enum gnwDriverError e;
    size_t id;
    const struct gnwDeviceDescriptor desc = descProvider();
    e = k_dev_install(&id, &desc);
    if (e != NO_ERROR) { 
        OOPS(installFailureMsg); 
    }

    e = k_dev_start(id);
    if (e != NO_ERROR) { 
        OOPS(startFailureMsg);
    }
}

void k_drv_loadMinimal() {

    /*
        Default text mode display
    */
    extern struct gnwDeviceDescriptor k_drv_display_descriptor();
    loadSingle(k_drv_display_descriptor, MSGS_FAIL(Display));
    
    /*
        PIT driver for 8253/8254 chip
    */
    extern struct gnwDeviceDescriptor k_drv_pit_descriptor();
    loadSingle(k_drv_pit_descriptor, MSGS_FAIL(PIT));

    /*
        Keyboard controller driver for 8042 PS/2 chip
    */
    extern struct gnwDeviceDescriptor k_drv_keyboard_descriptor();
    loadSingle(k_drv_keyboard_descriptor, MSGS_FAIL(Keyboard));

    #warning POSTPONED UNTIL DRIVERS MOVED TO SEPARATE PROCESSES
    // /*
    //     Default character output - terminal
    // */
    // extern struct gnwDeviceDescriptor k_drv_terminal_descriptor();
    // loadSingle(k_drv_terminal_descriptor, MSGS_FAIL(Terminal));
}
