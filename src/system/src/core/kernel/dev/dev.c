//
//  dev.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include <gunwdrv.h>
#include "dev.h"
#include "../../log/log.h"

#define MAX_DEVICES 8

static struct gnwDeviceDescriptor devices[MAX_DEVICES];
static uint_32 devicesCount;

// TODO replace with syscall
extern enum gnwDriverError k_hal_install(struct gnwDriverDesc);
// TODO replace with syscall
extern enum gnwDriverError k_hal_start(struct gnwDriverDesc);

extern struct gnwDriverDesc s_drv_pit();
extern struct gnwDeviceUHA s_drv_pit_uha();
extern struct gnwDriverDesc s_drv_keyboard();
extern struct gnwDeviceUHA s_drv_keyboard_uha();

extern int k_trm_putc(const char c);
extern int k_trm_puts(const char * const s);

enum gnwDriverError k_dev_install(struct gnwDeviceDescriptor descriptor) {

    k_trm_puts("Device manager: Install driver named ");
    k_trm_puts(descriptor.name);
    k_trm_putc('\n');

    if (devicesCount >= MAX_DEVICES) {
        return LIMIT_REACHED;
    }

    enum gnwDriverError e;

    e = k_hal_install(descriptor.driver.descriptor);
    if (e != NO_ERROR) {
        LOG_ERR("Error: Driver initialization failed");
        return e;
    }

    devices[devicesCount++] = descriptor;  

    return NO_ERROR;
}

enum gnwDriverError k_dev_start(struct gnwDeviceDescriptor descriptor) {

    k_trm_puts("Device manager: ");
    k_trm_puts(descriptor.name);
    k_trm_puts(" starting\n");

    enum gnwDriverError e;

    e = k_hal_start(descriptor.driver.descriptor);
    if (e != NO_ERROR) {
        LOG_ERR("Error: Driver startup failed");
        return e;
    }

    return NO_ERROR;
}

struct gnwDeviceDescriptor createDeviceDescriptor(const enum gnwDeviceType deviceType,
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

void k_dev_init() {

    k_trm_puts("Device manager: Init\n");

    enum gnwDriverError e;

    /*
        PIT driver for 8253/8254 chip
    */
    struct gnwDeviceDescriptor pit = createDeviceDescriptor(DEV_TYPE_SYSTEM,
                                                            s_drv_pit_uha(),
                                                            0x40,
                                                            s_drv_pit(),
                                                            "8253/8254 Programmable Interrupt Timer");
    e = k_dev_install(pit);
    if (e != NO_ERROR) { 
        LOG_FATAL("Fatal error: PIT driver installation failed"); 
        return; 
    }
    else {
        e = k_dev_start(pit);
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
    e = k_dev_install(kbd);
    if (e != NO_ERROR) { 
        LOG_FATAL("Fatal error: Keyboard driver installation failed"); 
        return; 
    }
    else {
        e = k_dev_start(kbd);
        if (e != NO_ERROR) { LOG_FATAL("Fatal error: Keyboard driver startup failed"); return; }
    }
}

size_t k_dev_descriptorCount() {
    return MAX_DEVICES;
}

struct gnwDeviceDescriptor k_dev_descriptorFor(const uint_32 descriptorID) {
    if (descriptorID >= MAX_DEVICES) {
        LOG_FATAL("Device descriptor over limit")
        __builtin_unreachable();
    }

    return devices[descriptorID];
}
