//
//  gunwdrv.h
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2021.
//

#ifndef GUNWOS_GUNWDRV_H
#define GUNWOS_GUNWDRV_H

#include <types.h>
#include <defs.h>
#include <gunwdev.h>
#include "gunwuha.h"

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_EMPTY(TYPE) { GNW_UHA_ ## TYPE ## _DESC_EMPTY, GNW_UHA_ ## TYPE ## _ROUTINE_EMPTY }

#endif // _GUNWAPI_KERNEL

/*  Device driver configuration

    Provides the kernel with entry points for general device operations
*/
struct gnwDriverConfig {
   
/*  Pointer to device initialization routine

    Called when device is being registered

    Expected to call drvInitReport on finish
    
    NOTE: In this phase the interrupts are DISABLED */
    void (*init)();

/*
    Pointer to device start routine

    Expected to call drvStartReport on finish

    Called after device is being initialized */
    void (*start)();

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
    drivers must provide an instance of this type
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

/*
    Report driver initialization status
*/
extern void drvInitReport(bool success);

/*
    Report driver startup status
*/
extern void drvStartReport(bool success);

/*
    struct gnwDeviceGetParamQuery decoder

    To be used as getParamDecoder parameter value in system UHA
*/
void gnwDeviceGetParamQuery_decode(const ptr_t, struct gnwDeviceGetParamQuery * const);

/*
    struct gnwDeviceSetParamQuery decoder

    To be used as setParamDecoder parameter value in system UHA
*/
void gnwDeviceSetParamQuery_decode(const ptr_t, struct gnwDeviceSetParamQuery * const);

/*
    struct gnwDeviceMemWriteQuery decoder

    To be used as writeDecoder parameter value in system UHA
*/
void gnwDeviceMemWriteQuery_decode(const ptr_t, struct gnwDeviceMemWriteQuery * const);

#endif // GUNWOS_GUNWDRV_H
