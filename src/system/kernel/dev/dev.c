//
//  dev.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include "dev.h"
#include <mem.h>
#include <gunwdev.h>
#include <src/_gunwdrv.h>
#include <hal/hal.h>
#include <hal/int/irq.h>
#include <hal/proc/proc.h>
#include <hal/mem/mem.h>
#include <error/panic.h>
#include <hal/criticalsec/criticalsec.h>
#include <src/_gunwdev.h>

#define MAX_DEVICES 8

PRIVATE struct device {
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

        NONE_PROC_ID for none
    */
    procId_t holder;

    /*
        Device events listener routine

        Note: Listener can be attached only by holder process
    */
    gnwDeviceEventListener listener;

    /*
        Device event decoder routine

        Note: Set by the listening process
    */
    gnwDeviceEventDecoder decoder;
} devices[MAX_DEVICES];

PRIVATE uint_32 devicesCount;

extern uint_8 k_hal_isIRQRegistered(uint_8 num);

PRIVATE bool validateId(size_t id) {
    return id < MAX_DEVICES;
}

PRIVATE bool validateInstalledId(size_t id) {
    return id < MAX_DEVICES && id < devicesCount;
}

PRIVATE enum gnwDeviceError validateStartedDevice(const procId_t processId, const size_t deviceId) {
    if (!validateInstalledId(deviceId)) {
        return GDE_UNKNOWN;
    }

    #warning TODO: checks
    // * check if process exists
    // * check if buffer does not exceed process memory

    struct device *dev = &devices[deviceId];
    if (dev->holder != processId) {
        return GDE_HANDLE_INVALID;
    }
    if (!dev->started) {
        return GDE_INVALID_DEVICE_STATE;
    }
    
    return GDE_NONE;
}

enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const descriptor) {
    if (!id) {
        OOPS("Identifier storage cannot be nullptr");
        return UNKNOWN;
    }
    if (devicesCount >= MAX_DEVICES) {
        return LIMIT_REACHED;
    }
    if (!validateDeviceDescriptor(descriptor)) {
        return UNKNOWN;
    }

    #warning CHECK MEMORY-MAPPED DEVICES FOR OVERLAPS WITH CURRENTLY INSTALLED ONES

    const struct gnwDriverConfig *driverDesc = &(descriptor->driver.descriptor);

    if (driverDesc->irq >= DEV_IRQ_LIMIT) {
        OOPS("Invalid IRQ value");
        return IRQ_INVALID;
    }

    if (driverDesc->isr && k_hal_isIRQRegistered(driverDesc->irq)) {
        OOPS("IRQ conflict");
        return IRQ_CONFLICT;
    }

    struct device dev = { 
        /* desc */ *descriptor, 
        /* initialized */ false, 
        /* started */ false, 
        /* holder */ NONE_PROC_ID, 
        /* listener */ nullptr,
        /* decoder */ nullptr
    };

    dev.initialized = (driverDesc->init ? driverDesc->init() : 1);
    if (!dev.initialized) {
        OOPS("Driver init failed");
        return UNINITIALIZED;
    }
    
    enum gnwDriverError e = NO_ERROR;

    *id = devicesCount;

    if (driverDesc->isr) {
        e = k_hal_install(*id, descriptor->driver.descriptor);   
    }

    if (e != NO_ERROR) {
        OOPS("Error: Driver initialization failed");
        return e;
    }

    devices[devicesCount++] = dev;

    return NO_ERROR;
}

enum gnwDriverError k_dev_start(size_t id) {
    if (!validateId(id)) {
        OOPS("Device identifier invalid");
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
    if (!validateInstalledId(id)) {
        OOPS("Device identifier invalid");
        return GDE_UNKNOWN;
    }
    
    if (!desc) {
        OOPS("Device descriptor nullptr");
        return GDE_UNKNOWN;
    }
    
    *desc = uhaGetDesc(id, devices[id].desc.type, devices[id].desc.api);
    return GDE_NONE;
}

enum gnwDeviceError k_dev_getByType(const enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc) {
    if (!desc) {
        OOPS("Device descriptor nullptr");
        return GDE_UNKNOWN;
    }

    for (size_t index = 0; index < MAX_DEVICES; ++index) {
        if (devices[index].desc.type & type) {
            return k_dev_getById(index, desc);
        }
    }

    return GDE_NOT_FOUND;
}

enum gnwDeviceError k_dev_getUHAForId(const size_t id, struct gnwDeviceUHA * const uha) {
    if (!validateInstalledId(id)) {
        OOPS("Device identifier invalid");
        return GDE_UNKNOWN;
    }
    
    if (!uha) {
        OOPS("UHA descriptor nullptr");
        return GDE_UNKNOWN;
    }

    *uha = devices[id].desc.api;
    return GDE_NONE;
}

enum gnwDeviceError k_dev_acquireHold(const procId_t processId, const size_t deviceId) {
    if (!validateInstalledId(deviceId)) {
        OOPS("Device identifier invalid");
        return GDE_UNKNOWN;
    }

    if (devices[deviceId].holder != NONE_PROC_ID) {
        return GDE_ALREADY_HELD;
    }

    devices[deviceId].holder = processId;

    return GDE_NONE;
}

void k_dev_releaseHold(const procId_t processId, const size_t deviceId) {
    if (!validateInstalledId(deviceId)) {
        OOPS("Device identifier invalid");
    }

    if (devices[deviceId].holder == processId) {
        devices[deviceId].holder = NONE_PROC_ID;
        devices[deviceId].listener = nullptr;
        devices[deviceId].decoder = nullptr;
    }
}

enum gnwDeviceError k_dev_writeMem(const procId_t processId, 
                                   const size_t deviceId,
                                   const ptr_t buffer,
                                   const range_addr_t devMemRange) {
    if (!buffer) {
        OOPS("Buffer cannot be nullptr");
        return GDE_UNKNOWN;
    }

    const enum gnwDeviceError e = validateStartedDevice(processId, deviceId);
    if (e) {
        return e;
    }

    const size_t maxInputSizeBytes = devices[deviceId].desc.api.mem.desc.maxInputSizeBytes;
    if (!maxInputSizeBytes) {
        return GDE_INVALID_OPERATION;
    }

    if (devMemRange.offset >= maxInputSizeBytes) {
        return GDE_INVALID_PARAMETER;
    }
    const size_t devBytesLeft = maxInputSizeBytes - devMemRange.offset;
    if (devMemRange.sizeBytes > devBytesLeft) {
        return GDE_INVALID_PARAMETER;
    }

    const struct gnwDeviceUHA_mem_routine * const routine = &devices[deviceId].desc.api.mem.routine;
    if (!routine->write) {
        return GDE_INVALID_OPERATION;
    }
    #warning it is more than dangerous to allow the driver to access the buffer directly, moreover it could be even impossible when driver processes are implemented
    routine->write(buffer, devMemRange);

    return GDE_NONE;
}

enum gnwDeviceError k_dev_writeChar(const procId_t processId, 
                                    const size_t deviceId,
                                    const char character) {

    const enum gnwDeviceError e = validateStartedDevice(processId, deviceId);
    if (e) {
        return e;
    }

    const struct gnwDeviceUHA_charOut_routine * const routine = &devices[deviceId].desc.api.charOut.routine;
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

static enum gnwDeviceError validateListener(const procId_t processId, 
                                            const size_t deviceId, 
                                            const gnwDeviceEventListener listener,
                                            const gnwDeviceEventDecoder decoder) {
    if (!listener) {
        return GDE_LISTENER_INVALID;
    }
    if (!decoder) {
        return GDE_DECODER_INVALID;
    }

    enum gnwDeviceError err = validateStartedDevice(processId, deviceId);
    if (err) {
        return err;
    }

    if (devices[deviceId].listener) {
        return GDE_ALREADY_SET;
    }

    return GDE_NONE;
}

enum gnwDeviceError k_dev_listen(const procId_t processId, 
                                 const size_t deviceId, 
                                 const gnwDeviceEventListener listener,
                                 const gnwDeviceEventDecoder decoder) {
    enum gnwDeviceError err = validateListener(processId, deviceId, listener, decoder);
    if (err) {
        return err;
    }

    devices[deviceId].listener = listener;
    devices[deviceId].decoder = decoder;
    return GDE_NONE;
}

enum gnwDeviceError k_dev_getParam(const size_t deviceId,
                                   const struct gnwDeviceParamDescriptor paramDescriptor,
                                   size_t * const absResult) {
    if (!absResult) {
        OOPS("Nullptr");
        return GDE_UNKNOWN;
    }

    if (!validateInstalledId(deviceId)) {
        return GDE_ID_INVALID;
    }

    if (!devices[deviceId].desc.api.system.routine.getParam) {
        return GDE_INVALID_OPERATION;
    }

    if (!devices[deviceId].desc.api.system.routine.getParam(paramDescriptor.param,
                                                            paramDescriptor.subParam,
                                                            paramDescriptor.paramIndex,
                                                            absResult)) {
        return GDE_OPERATION_FAILED;
    }

    return GDE_NONE;
}

enum gnwDeviceError k_dev_setParam(const procId_t procId,
                                   const size_t deviceId,
                                   const struct gnwDeviceParamDescriptor paramDescriptor,
                                   const size_t value) {
    const enum gnwDeviceError err = validateStartedDevice(procId, deviceId);
    if (err != GDE_NONE) {
        return err;
    }

    if (!devices[deviceId].desc.api.system.routine.setParam) {
        return GDE_INVALID_OPERATION;
    }

    if (!devices[deviceId].desc.api.system.routine.setParam(paramDescriptor.param,
                                                            paramDescriptor.subParam,
                                                            paramDescriptor.paramIndex,
                                                            value)) {
        return GDE_OPERATION_FAILED;
    }

    return GDE_NONE;
}

static enum gnwDeviceError validateEmitter(const size_t * const devIdPtr) {
    if (!devIdPtr) {
        return GDE_INVALID_DEVICE_STATE;
    }
    if (!validateInstalledId(*devIdPtr)) {
        OOPS("Unexpected serviced device ID");
        return GDE_UNKNOWN;
    }
    if (!devices[*devIdPtr].started) {
        return GDE_INVALID_DEVICE_STATE;
    }

    return GDE_NONE;
}

static enum gnwDeviceError validateListenerInvocation(const size_t deviceId) {
    struct device *dev = &devices[deviceId];
    if (!dev->listener) {
        return GDE_NOT_FOUND;
    }
    if (!dev->decoder) {
        return GDE_UNKNOWN;
    } 
    if (dev->holder == NONE_PROC_ID) {
        OOPS("Inconsistent holder listener state");
        return GDE_UNKNOWN;
    }

    return GDE_NONE;
}

enum gnwDeviceError k_dev_emit(const struct gnwDeviceEvent * const eventPtr) {
    if (!eventPtr) {
        OOPS("Nullptr");
        return GDE_UNKNOWN;
    }
    enum gnwDeviceError err = validateEmitter(k_hal_servicedDevIdPtr);
    if (err) {
        return err;
    }
    err = validateListenerInvocation(*k_hal_servicedDevIdPtr);
    if (err == GDE_NOT_FOUND) {
        return GDE_NONE;
    } else if (err) {
        return err;
    }

    struct device *dev = &devices[*k_hal_servicedDevIdPtr];
    enum k_proc_error callbackErr = k_proc_callback_invoke_ptr(dev->holder, 
                                                               (gnwEventListener_ptr)dev->listener,
                                                               (ptr_t)eventPtr,
                                                               eventPtr->dataSizeBytes + sizeof(struct gnwDeviceEvent),
                                                               sizeof(struct gnwDeviceEvent),
                                                               (gnwRunLoopDataEncodingRoutine)gnwDeviceEvent_encode,
                                                               (gnwRunLoopDataEncodingRoutine)dev->decoder);
    switch (callbackErr) {
    case PE_NONE:
        return GDE_NONE;
    case PE_IGNORED:
        return GDE_HANDLE_INVALID;
    case PE_ACCESS_VIOLATION:
        return GDE_LISTENER_INVALID;
    default:
        return GDE_UNKNOWN;
    }
}

void k_dev_procCleanup(const procId_t procId) {
    for (size_t devId = 0; devId < MAX_DEVICES; ++devId) {
        if (devices[devId].holder == procId) {
            CRITICAL_SECTION(
                k_dev_releaseHold(procId, devId);
            )
        }
    }
}
