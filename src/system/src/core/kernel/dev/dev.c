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

extern enum gnwDriverError k_hal_install(const struct gnwDriverDesc);
extern enum gnwDriverError k_hal_start(const struct gnwDriverDesc);

extern int c_trm_putc(const char c);
extern int c_trm_puts(const char * const s);

enum gnwDriverError k_dev_install(const struct gnwDeviceDescriptor * const descriptor) {
    if (!descriptor) {
        LOG_FATAL("Fatal error: Driver descriptor pointer invalid"); 
        return UNKNOWN;
    }

    c_trm_puts("Device manager: Install driver named ");
    c_trm_puts(descriptor->name);
    c_trm_putc('\n');

    if (devicesCount >= MAX_DEVICES) {
        return LIMIT_REACHED;
    }

    enum gnwDriverError e;

    e = k_hal_install(descriptor->driver.descriptor);
    if (e != NO_ERROR) {
        LOG_ERR("Error: Driver initialization failed");
        return e;
    }

    devices[devicesCount++] = *descriptor;  

    return NO_ERROR;
}

enum gnwDriverError k_dev_start(const struct gnwDeviceDescriptor * const descriptor) {
    if (!descriptor) {
        LOG_FATAL("Fatal error: Driver descriptor pointer invalid"); 
        return UNKNOWN;
    }

    c_trm_puts("Device manager: ");
    c_trm_puts(descriptor->name);
    c_trm_puts(" starting\n");

    enum gnwDriverError e;

    e = k_hal_start(descriptor->driver.descriptor);
    if (e != NO_ERROR) {
        LOG_ERR("Error: Driver startup failed");
        return e;
    }

    return NO_ERROR;
}

void k_dev_init() {
    c_trm_puts("Device manager: Init\n");
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
