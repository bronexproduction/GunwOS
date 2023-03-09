//
//  gunwdrv.h
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2021.
//

#ifndef GUNWOS_GUNWDRV_H
#define GUNWOS_GUNWDRV_H

#include <stdgunw/types.h>
#include <stdgunw/defs.h>
#include <gunwdev.h>
#include <driver/gunwuha.h>

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
    UNKNOWN         = -1
};

/*  Device driver configuration

    Provides the kernel with entry points for general device operations
*/
struct gnwDriverConfig {
   
/*  Pointer to device initialization routine

    Called when device is being registered

    Returns 1 on success, 0 otherwise
    
    NOTE: In this phase the interrupts are DISABLED */
    bool (*init)();

/*
    Pointer to device start routine

    Returns 1 on success, 0 otherwise

    Called after device is being initialized */
    bool (*start)();

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
        Driver configuration
    */
    struct gnwDriverConfig descriptor;
};

/*
    Device descriptor

    In order to register a device in kernel
    an instance of this type to be prepared
    and passed to k_dev_install()  
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
