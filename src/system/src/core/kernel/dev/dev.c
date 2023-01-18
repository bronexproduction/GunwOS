//
//  dev.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include <gunwdev.h>
#include "../hal/int/irq.h"
#include "../../log/log.h"

#define MAX_DEVICES 8

struct device {
    /*
        Driver descriptor
    */
    struct gnwDeviceDescriptor desc;

    /*
        Initialization mark
    */
    bool initialized;

    /*
        Driver start mark
    */
    bool started;

    /*
        Device holder identifier

        Identifier of the process allowed to access the device
        or 0 if none allowed (kernel only)
    */
    size_t holder;
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

static bool validateId(size_t id) {
    return id < MAX_DEVICES;
}

enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const descriptor) {
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

    const struct gnwDriverConfig *driverDesc = &(descriptor->driver.descriptor);

    if (driverDesc->irq >= DEV_IRQ_LIMIT) {
        LOG_FATAL("Invalid IRQ value");
        return IRQ_INVALID;
    }

    if (k_hal_isIRQRegistered(driverDesc->irq)) {
        LOG_FATAL("IRQ conflict");
        return IRQ_CONFLICT;
    }

    struct device dev = { *descriptor, 0, 0, 0 };

    dev.initialized = (driverDesc->init ? driverDesc->init() : 1);
    if (!dev.initialized) {
        LOG_ERR("Driver init failed");
        return UNINITIALIZED;
    }
    
    enum gnwDriverError e = NO_ERROR;

    if (driverDesc->isr) {
        extern enum gnwDriverError k_hal_install(const struct gnwDriverConfig);
        e = k_hal_install(descriptor->driver.descriptor);   
    }

    if (e != NO_ERROR) {
        LOG_ERR("Error: Driver initialization failed");
        return e;
    }

    *id = devicesCount;
    devices[devicesCount++] = dev;

    return NO_ERROR;
}

enum gnwDriverError k_dev_start(size_t id) {
    if (!validateId(id)) {
        LOG_FATAL("Device identifier invalid")
        return UNKNOWN;
    }

    struct device *dev = &devices[id];

    c_trm_puts("Device manager: ");
    c_trm_puts(dev->desc.name);
    c_trm_puts(" starting\n");

    if (!dev->initialized) {
        LOG_FATAL("Trying to start uninitialized driver");
        return UNINITIALIZED;
    }

    bool (*start)(void) = dev->desc.driver.descriptor.start;

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

enum gnwDeviceError k_dev_getById(const size_t id, struct gnwDeviceUHADesc * const desc) {
    if (!validateId(id) || id >= devicesCount) {
        LOG_FATAL("Device identifier invalid")
        return GDE_UNKNOWN;
    }
    
    if (!desc) {
        LOG_FATAL("Device descriptor descriptor over limit");
        return GDE_UNKNOWN;
    }

    *desc = uhaGetDesc(id, devices[id].desc.api);
    return GDE_NONE;
}

enum gnwDeviceError k_dev_getByType(enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc) {
    if (!desc) {
        LOG_FATAL("Device descriptor descriptor over limit");
        return GDE_UNKNOWN;
    }

    for (size_t index = 0; index < MAX_DEVICES; ++index) {
        if (devices[index].desc.type == type) {
            return k_dev_getById(index, desc);
        }
    }

    return GDE_NOT_FOUND;
}

enum gnwDeviceError k_dev_acquireHold(size_t processId, size_t deviceId) {
    if (!validateId(deviceId) || deviceId >= devicesCount) {
        LOG_FATAL("Device identifier invalid")
        return GDE_UNKNOWN;
    }

    devices[deviceId].holder = processId;

    return GDE_NONE;
}
