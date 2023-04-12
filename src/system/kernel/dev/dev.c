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

#define MAX_DEVICES 8

struct deviceEventListener {
    /*
        Event listener routine
    */
    union gnwEventListener routine;

    /*
        Process' run loop for event dispatch
    */
    struct gnwRunLoop * runLoop;
};

static struct device {
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
        Device events listener

        Note: Listener can be attached only by holder process
    */
    struct deviceEventListener listener;
} devices[MAX_DEVICES];

static uint_32 devicesCount;

extern uint_8 k_hal_isIRQRegistered(uint_8 num);

static bool validateId(size_t id) {
    return id < MAX_DEVICES;
}

static bool validateInstalledId(size_t id) {
    return id < MAX_DEVICES && id < devicesCount;
}

static enum gnwDeviceError validateStartedDevice(const procId_t processId, const size_t deviceId) {
    if (!validateInstalledId(deviceId)) {
        OOPS("Device identifier invalid");
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
        /* listener */ (struct deviceEventListener) {
            /* routine */ (union gnwEventListener) {
                /* _handle */ NULL
            },
            /* runLoop */ nullptr
        }
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
        devices[deviceId].listener.routine._handle = NULL;
        devices[deviceId].listener.runLoop = nullptr;
    }
}

enum gnwDeviceError k_dev_writeMem(const procId_t processId, 
                                   const size_t deviceId,
                                   const void * const buffer) {
    
    if (!buffer) {
        OOPS("Buffer cannot be nullptr");
        return GDE_UNKNOWN;
    }

    const enum gnwDeviceError e = validateStartedDevice(processId, deviceId);
    if (e) {
        return e;
    }

    const struct gnwDeviceUHA_mem_routine * const routine = &devices[deviceId].desc.api.mem.routine;
    if (!routine->write) {
        return GDE_INVALID_OPERATION;
    }
    #warning it is more than dangerous to allow the driver to access the buffer directly, moreover it could be even impossible when driver processes are implemented
    routine->write(buffer);

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
                                            const union gnwEventListener listener,
                                            const struct gnwRunLoop * const runLoopPtr) {
    if (!listener._handle) {
        return GDE_LISTENER_INVALID;
    }

    enum gnwDeviceError err = validateStartedDevice(processId, deviceId);
    if (err) {
        return err;
    }

    if (devices[deviceId].listener.routine._handle) {
        return GDE_ALREADY_SET;
    }

    ptr_t absRunLoopPtr = k_mem_absForProc(processId, (ptr_t)runLoopPtr);
    if (!k_mem_bufInZoneForProc(processId, absRunLoopPtr, sizeof(struct gnwRunLoop))) {
        return GDE_INVALID_PARAMETER;
    }

    return GDE_NONE;
}

enum gnwDeviceError k_dev_listen(const procId_t processId, 
                                 const size_t deviceId, 
                                 const union gnwEventListener listener,
                                 struct gnwRunLoop * const runLoopPtr) {
    enum gnwDeviceError err = validateListener(processId, deviceId, listener, runLoopPtr);
    if (err) {
        return err;
    }

    devices[deviceId].listener.routine = listener;
    devices[deviceId].listener.runLoop = runLoopPtr;
    return GDE_NONE;
}

static enum gnwDeviceError validateEmitter(const size_t * const devIdPtr,
                                           const enum gnwEventFormat format) {
    if (!devIdPtr) {
        return GDE_INVALID_DEVICE_STATE;
    }
    if (!validateInstalledId(*devIdPtr)) {
        OOPS("Unexpected serviced device ID");
    }
    if (!devices[*devIdPtr].started) {
        return GDE_INVALID_DEVICE_STATE;
    }
    if (devices[*devIdPtr].desc.api.event.desc.eventDataFormat != format) {
        return GDE_INVALID_OPERATION;
    }

    return GDE_NONE;
}

static enum gnwDeviceError validateListenerInvocation(const size_t deviceId) {
    struct device *dev = &devices[deviceId];
    if (!dev->listener.routine._handle) {
        return GDE_NOT_FOUND;
    }
    if (dev->holder == NONE_PROC_ID) {
        OOPS("Inconsistent holder listener state");
    }

    return GDE_NONE;
}

enum gnwDeviceError k_dev_emit_void(const int_32 type) {
    enum gnwDeviceError err = validateEmitter(k_hal_servicedDevIdPtr, GEF_U32);
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
    gnwEventListener_32 listener = dev->listener.routine._32;

    enum k_proc_error callbackErr = k_proc_callback_invoke_32(dev->holder, dev->listener.runLoop, (void (*)(int_32))listener, type);
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

enum gnwDeviceError k_dev_emit_u8(const int_32 type,
                                  const int_8 data) {
    enum gnwDeviceError err = validateEmitter(k_hal_servicedDevIdPtr, GEF_U32_U8);
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
    gnwEventListener_32_8 listener = dev->listener.routine._32_8;

    enum k_proc_error callbackErr = k_proc_callback_invoke_32_8(dev->holder, dev->listener.runLoop, (void (*)(int_32, int_8))listener, type, data);
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
                devices[devId].holder = NONE_PROC_ID;
                memnull(&devices[devId].listener, sizeof(struct deviceEventListener)); 
            )
        }
    }
}
