//
//  dev.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include <gunwdev.h>
#include <stdgunw/mem.h>
#include <hal/int/irq.h>
#include <error/panic.h>
#include <hal/proc/proc.h>

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
    procId_t holder;
};

static struct device devices[MAX_DEVICES];
static uint_32 devicesCount;

extern uint_8 k_hal_isIRQRegistered(uint_8 num);

static uint_8 validate(const struct gnwDeviceDescriptor * const descriptor) {
    if (!descriptor) {
        OOPS("Driver descriptor pointer invalid"); 
        return 0;
    }
    if (!descriptor->name) {
        OOPS("Driver descriptor invalid"); 
        return 0;
    }

    return 1;
}

static bool validateId(size_t id) {
    return id < MAX_DEVICES;
}

enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const descriptor) {
    if (!id) {
        OOPS("Identifier storage cannot be nullptr");
        return UNKNOWN;
    }
    if (devicesCount >= MAX_DEVICES) {
        return LIMIT_REACHED;
    }
    if (!validate(descriptor)) {
        return UNKNOWN;
    }

    const struct gnwDriverConfig *driverDesc = &(descriptor->driver.descriptor);

    if (driverDesc->irq >= DEV_IRQ_LIMIT) {
        OOPS("Invalid IRQ value");
        return IRQ_INVALID;
    }

    if (driverDesc->isr && k_hal_isIRQRegistered(driverDesc->irq)) {
        OOPS("IRQ conflict");
        return IRQ_CONFLICT;
    }

    struct device dev = { *descriptor, 0, 0, NONE_PROC_ID };

    dev.initialized = (driverDesc->init ? driverDesc->init() : 1);
    if (!dev.initialized) {
        OOPS("Driver init failed");
        return UNINITIALIZED;
    }
    
    enum gnwDriverError e = NO_ERROR;

    if (driverDesc->isr) {
        extern enum gnwDriverError k_hal_install(const struct gnwDriverConfig);
        e = k_hal_install(descriptor->driver.descriptor);   
    }

    if (e != NO_ERROR) {
        OOPS("Error: Driver initialization failed");
        return e;
    }

    *id = devicesCount;
    devices[devicesCount++] = dev;

    return NO_ERROR;
}

enum gnwDriverError k_dev_start(size_t id) {
    if (!validateId(id)) {
        OOPS("Device identifier invalid")
        return UNKNOWN;
    }

    struct device *dev = &devices[id];

    if (!dev->initialized) {
        OOPS("Trying to start uninitialized driver");
        return UNINITIALIZED;
    }

    bool (*start)(void) = dev->desc.driver.descriptor.start;

    dev->started = (start ? start() : 1);
    if (!dev->started) {
        OOPS("Error: Driver startup failed");
        return START_FAILED;
    }

    return NO_ERROR;
}

void k_dev_init() {
}

enum gnwDeviceError k_dev_getById(const size_t id, struct gnwDeviceUHADesc * const desc) {
    if (!validateId(id) || id >= devicesCount) {
        OOPS("Device identifier invalid")
        return GDE_UNKNOWN;
    }
    
    if (!desc) {
        OOPS("Device descriptor nullptr");
        return GDE_UNKNOWN;
    }

    *desc = uhaGetDesc(id, devices[id].desc.api);
    return GDE_NONE;
}

enum gnwDeviceError k_dev_getByType(enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc) {
    if (!desc) {
        OOPS("Device descriptor descriptor over limit");
        return GDE_UNKNOWN;
    }

    for (size_t index = 0; index < MAX_DEVICES; ++index) {
        if (devices[index].desc.type == type) {
            return k_dev_getById(index, desc);
        }
    }

    return GDE_NOT_FOUND;
}

enum gnwDeviceError k_dev_acquireHold(procId_t processId, size_t deviceId) {
    if (!validateId(deviceId) || deviceId >= devicesCount) {
        OOPS("Device identifier invalid")
        return GDE_UNKNOWN;
    }

    if (devices[deviceId].holder != NONE_PROC_ID) {
        return GDE_ALREADY_HELD;
    }

    devices[deviceId].holder = processId;

    return GDE_NONE;
}

void k_dev_releaseHold(procId_t processId, size_t deviceId) {
    if (!validateId(deviceId) || deviceId >= devicesCount) {
        OOPS("Device identifier invalid")
    }

    if (devices[deviceId].holder == processId) {
        devices[deviceId].holder = NONE_PROC_ID;
    }
}

static enum gnwDeviceError devCheck(const procId_t processId, const size_t deviceId) {
    if (!validateId(deviceId) || deviceId >= devicesCount) {
        OOPS("Device identifier invalid")
        return GDE_UNKNOWN;
    }

    #warning TODO: checks
    // * check if process exists
    // * check if device exists
    // * check if process holds the device
    // * check if buffer does not exceed process memory

    struct device *dev = &devices[deviceId];
    if (!dev) {
        return GDE_ID_INVALID;
    }
    if (dev->holder != processId) {
        return GDE_HANDLE_INVALID;
    }
    if (!dev->started) {
        return GDE_INVALID_DEVICE_STATE;
    }
    
    return GDE_NONE;
}

enum gnwDeviceError k_dev_writeMem(const procId_t processId, 
                                   const size_t deviceId,
                                   const void * const buffer) {
    
    if (!buffer) {
        OOPS("Buffer cannot be nullptr");
        return GDE_UNKNOWN;
    }

    const enum gnwDeviceError e = devCheck(processId, deviceId);
    if (e) {
        return e;
    }

    const struct gnwDeviceUHA_mem_routine * const routine = &devices[deviceId].desc.api.mem.routine;
    if (!routine->write) {
        return GDE_INVALID_OPERATION;
    }
    routine->write(buffer);

    return GDE_NONE;
}

enum gnwDeviceError k_dev_writeChar(const procId_t processId, 
                                    const size_t deviceId,
                                    const char character) {

    const enum gnwDeviceError e = devCheck(processId, deviceId);
    if (e) {
        return e;
    }

    const struct gnwDeviceUHA_char_out_routine * const routine = &devices[deviceId].desc.api.charOut.routine;
    if (!routine->isReadyToWrite) {
        return GDE_INVALID_OPERATION;
    }
    if (!routine->isReadyToWrite()) {
        return GDE_INVALID_DEVICE_STATE;
    }
    if (!routine->write) {
        return GDE_INVALID_OPERATION;
    }
    if (!routine->write(character)) {
        return GDE_OPERATION_FAILED;
    }

    return GDE_NONE;
}
