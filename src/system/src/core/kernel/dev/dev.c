//
//  dev.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include <gunwdrv.h>
#include "dev.h"
#include "../hal/int/irq.h"
#include "../../log/log.h"

#define MAX_DEVICES 8

static struct device {
    /*
        Driver descriptor
    */
    struct gnwDeviceDescriptor desc;

    /*
        Initialization mark
    */
    uint_8 initialized;

    /*
        Driver start mark
    */
    uint_8 started;
};

static struct device devices[MAX_DEVICES];
static uint_32 devicesCount;

extern uint_8 k_hal_isIRQRegistered(uint_8 num);

extern int c_trm_putc(const char c);
extern int c_trm_puts(const char * const s);

static uint_8 validate(const struct gnwDeviceDescriptor * const descriptor) {
    if (!descriptor) {
        LOG_FATAL("Fatal error: Driver descriptor pointer invalid"); 
        return 0;
    }
    if (!descriptor->name) {
        LOG_FATAL("Fatal error: Driver description pointer invalid"); 
        return 0;
    }

    return 1;
}

enum gnwDriverError k_dev_install(size_t *id, const struct gnwDeviceDescriptor * const descriptor) {
    if (!id) {
        LOG_FATAL("Identifier storage cannot be nullptr");
        return UNKNOWN;
    }
    if (devicesCount >= MAX_DEVICES) {
        return LIMIT_REACHED;
    }
    if (!validate(descriptor)) {
        return UNKNOWN;
    }

    c_trm_puts("Device manager: Install driver named ");
    #warning What if the name does not have 0 at end?
    c_trm_puts(descriptor->name);
    c_trm_putc('\n');

    struct gnwDriverDesc *driverDesc = &(descriptor->driver.descriptor);

    if (driverDesc->irq >= DEV_IRQ_LIMIT) {
        LOG_FATAL("Invalid IRQ value");
        return IRQ_INVALID;
    }

    if (k_hal_isIRQRegistered(driverDesc->irq)) {
        LOG_FATAL("IRQ conflict");
        return IRQ_CONFLICT;
    }

    struct device dev = { descriptor, 0, 0 };

    dev.initialized = (driverDesc->init ? driverDesc->init() : 1);
    if (!dev.initialized) {
        LOG_ERR("Driver init failed");
        return UNINITIALIZED;
    }
    
    enum gnwDriverError e = NO_ERROR;

    if (driverDesc->isr) {
        extern enum gnwDriverError k_hal_install(const struct gnwDriverDesc);
        e = k_hal_install(descriptor->driver.descriptor);   
    }

    if (e != NO_ERROR) {
        LOG_ERR("Error: Driver initialization failed");
        return e;
    }

    devices[devicesCount++] = dev;

    return NO_ERROR;
}

enum gnwDriverError k_dev_start(size_t id) {
    if (id >= MAX_DEVICES) {
        LOG_FATAL("Device identifier over limit")
        return UNKNOWN;
    }

    static struct device *dev = &devices[id];

    c_trm_puts("Device manager: ");
    c_trm_puts(dev->desc.name);
    c_trm_puts(" starting\n");

    if (!dev->initialized) {
        LOG_FATAL("Trying to start uninitialized driver");
        return UNINITIALIZED;
    }

    void (*start)(void) = dev->desc.driver.descriptor.start;

    dev->started = (start ? start() : 1);
    if (!dev->started) {
        LOG_ERR("Error: Driver startup failed");
        return START_FAILED;
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
