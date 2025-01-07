//
//  dev.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include "dev.h"
#include <mem.h>
#include <gunwdev.h>
#include <_gunwdrv.h>
#include <hal/hal.h>
#include <hal/int/irq.h>
#include <hal/proc/proc.h>
#include <hal/mem/mem.h>
#include <error/panic.h>
#include <hal/criticalsec/criticalsec.h>
#include <log/log.h>
#include <_gunwdev.h>

#define MAX_DEVICES 8

enum deviceStatus {
    NONE            =  0,
    NEW             =  1,
    INITIALIZING    =  2,
    INITIALIZED     =  3,
    STARTING        =  4,
    STARTED         =  5,
    FAILED          = -1
};

struct deviceRequestInfo {
    /*
        Requester process ID
    */
    procId_t procId;

    /*
        Request error pointer
    */
    enum gnwDriverError * vErrPtr;
};

PRIVATE struct device {
    /*
        Driver descriptor
    */
    struct gnwDeviceDescriptor desc;

    /*
        Device status
    */
    enum deviceStatus status;

    /*
        Device holder identifier

        Identifier of the process allowed to access the device

        NONE_PROC_ID for none
    */
    procId_t holder;

    /*
        Device operator process identifier
    */
    procId_t operator;

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

    /*
        Pending device request information

        Used for asynchronous initialization and startup
    */
    struct deviceRequestInfo pendingRequestInfo;
} devices[MAX_DEVICES];

PRIVATE uint_32 devicesCount;

extern uint_8 k_hal_isIRQRegistered(uint_8 num);

PRIVATE bool validateId(size_t id) {
    return id < MAX_DEVICES;
}

void k_dev_init() {
    memzero(devices, sizeof(struct device) * MAX_DEVICES);
    for (size_t i = 0; i < MAX_DEVICES; ++i) {
        devices[i].holder = NONE_PROC_ID;
        devices[i].operator = NONE_PROC_ID;
        devices[i].pendingRequestInfo.procId = NONE_PROC_ID;
    }
}

PRIVATE bool validateInstalledId(size_t id) {
    return id < MAX_DEVICES && id < devicesCount;
}

PRIVATE enum gnwDeviceError validateStartedDevice(const procId_t processId, const size_t deviceId) {
    if (!validateInstalledId(deviceId)) {
        return GDE_UNKNOWN;
    }

    if (!k_proc_idIsUser(processId)) {
        OOPS("Invalid process identifier", GDE_UNKNOWN);
    }

    if (k_proc_getInfo(processId).state != PS_RUNNING) {
        OOPS("Invalid process state", GDE_UNKNOWN);
    }

    struct device *dev = &devices[deviceId];
    if (dev->holder != processId) {
        return GDE_HANDLE_INVALID;
    }
    if (!k_proc_idIsUser(dev->operator) || k_proc_getInfo(dev->operator).type != PT_DRIVER) {
        return GDE_INVALID_DEVICE_STATE;
    }
    if (dev->status != STARTED) {
        return GDE_INVALID_DEVICE_STATE;
    }
    
    return GDE_NONE;
}

PRIVATE enum gnwDriverError devInstallPrepare(const struct gnwDeviceDescriptor * const descriptorPtr,
                                              const procId_t validOperatorProcId,
                                              size_t * const deviceIdPtr,
                                              const struct gnwDriverConfig * * driverDescPtr,
                                              struct device * dev) {
    if (!driverDescPtr) {
        LOG("Driver desc nullptr not allowed");
        return GDRE_UNKNOWN;
    }
    if (!deviceIdPtr) {
        LOG("ID nullptr not allowed");
        return GDRE_UNKNOWN;
    }
    if (!dev) {
        LOG("Device nullptr not allowed");
        return GDRE_UNKNOWN;
    }
    if (devicesCount >= MAX_DEVICES) {
        LOG("Device limit reached");
        return GDRE_LIMIT_REACHED;
    }
    if (!validateDeviceDescriptor(descriptorPtr)) {
        LOG("Device descriptor invalid");
        return GDRE_INVALID_DESCRIPTOR;
    }

    #warning CHECK MEMORY-MAPPED DEVICES FOR OVERLAPS WITH CURRENTLY INSTALLED ONES

    *driverDescPtr = &(descriptorPtr->driver.descriptor);

    if ((*driverDescPtr)->irq >= DEV_IRQ_LIMIT) {
        return GDRE_IRQ_INVALID;
    }

    if ((*driverDescPtr)->isr && k_hal_isIRQRegistered((*driverDescPtr)->irq)) {
        return GDRE_IRQ_CONFLICT;
    }

    *dev = (struct device){ 
        /* desc */ *descriptorPtr, 
        /* status */ NEW,
        /* holder */ NONE_PROC_ID, 
        /* operator */ validOperatorProcId,
        /* listener */ nullptr,
        /* decoder */ nullptr,
        /* pendingRequestInfo */ {
            /* procId */ NONE_PROC_ID,
            /* vErrPtr */ nullptr
        }
    };

    return GDRE_NONE;
}

PRIVATE struct device * deviceForOperator(const procId_t operatorProcId) {
    if (!k_proc_idIsUser(operatorProcId)) {
        OOPS("Unexpected operator process ID", nullptr);
    }
    for (size_t id = 0; id < MAX_DEVICES; ++id) {
        if (devices[id].operator == operatorProcId) {
            return &(devices[id]);
        }
    }

    return nullptr;
}

enum gnwDriverError k_dev_install(const struct gnwDeviceDescriptor * const descriptorPtr,
                                  size_t * const deviceIdPtr) {
    const struct gnwDriverConfig * driverDescPtr = nullptr;
    struct device dev = { 0 };
    enum gnwDriverError error = devInstallPrepare(descriptorPtr,
                                                  KERNEL_PROC_ID,
                                                  deviceIdPtr,
                                                  &driverDescPtr,
                                                  &dev);
    if (error != GDRE_NONE) {
        return error;
    }
    if (!driverDescPtr) {
        LOG("Unexpected driver desc nullptr");
        return GDRE_UNKNOWN;
    }

    struct device * devicePtr;
    CRITICAL_SECTION(
        devicePtr = &(devices[devicesCount]);
        *deviceIdPtr = devicesCount;
        devices[devicesCount++] = dev;
    )

    devicePtr->status = INITIALIZING;

    if (!driverDescPtr->init) {
        devicePtr->status = INITIALIZED;
    } else {
        driverDescPtr->init();
    }

    if (devicePtr->status != INITIALIZED) {
        LOG("Driver init failed");
        return GDRE_UNINITIALIZED;
    }
    
    enum gnwDriverError e = GDRE_NONE;

    if (driverDescPtr->isr) {
        e = k_hal_install(*deviceIdPtr, descriptorPtr->driver.descriptor);   
    }

    if (e != GDRE_NONE) {
        LOG("Error: Driver installation failed");
        return e;
    }

    return GDRE_NONE;
}

enum gnwDriverError k_dev_install_async(const struct gnwDeviceDescriptor * const descriptorPtr,
                                        const procId_t operatorProcId) {

    if (!k_proc_idIsUser(operatorProcId)) {
        LOG("Invalid operator process ID");
        return GDRE_INVALID_ARGUMENT;
    }
    if (k_proc_getInfo(operatorProcId).type != PT_DRIVER) {
        LOG("Invalid operator process type");
        return GDRE_INVALID_ARGUMENT;
    }
    if (deviceForOperator(operatorProcId)) {
        LOG("Operator process ID already in use");
        return GDRE_INVALID_ARGUMENT;
    }

    const struct gnwDriverConfig * driverDescPtr = nullptr;
    struct device dev = { 0 };
    size_t deviceId;
    enum gnwDriverError error = devInstallPrepare(descriptorPtr,
                                                  operatorProcId,
                                                  &deviceId,
                                                  &driverDescPtr,
                                                  &dev);
    if (error != GDRE_NONE) {
        return error;
    }
    if (!driverDescPtr) {
        LOG("Unexpected driver desc nullptr");
        return GDRE_UNKNOWN;
    }

    #warning it can be nice to return device identifier somehow

    devices[devicesCount++] = dev;

    return GDRE_NONE;
}

static enum gnwDriverError deviceInvokeAsync(const procId_t operatorProcId,
                                             const procId_t requesterProcId,
                                             enum gnwDriverError * const vErrPtr,
                                             const enum deviceStatus expectedStatus,
                                             const enum deviceStatus resultStatus,
                                             addr_t vOperationPtrDeviceOffset) {



                                                        /*
                                                        
                                                        
                                                        
                                                        
                                                        
                                                            RETURN GDRE_IGNORED ON NULL 
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        */


    if (!vErrPtr) {
        return GDE_INVALID_PARAMETER;
    }
    if (!k_proc_idIsUser(requesterProcId)) {
        return GDE_INVALID_PARAMETER;
    }
    if ((vOperationPtrDeviceOffset + sizeof(addr_t)) > sizeof(struct device)) {
        return GDE_INVALID_PARAMETER;
    }
    struct device * const dev = deviceForOperator(operatorProcId);
    if (!dev) {
        return GDE_NOT_FOUND;
    }

    if (dev->status != expectedStatus) {
        return GDE_INVALID_DEVICE_STATE;
    }
    if (dev->pendingRequestInfo.procId != NONE_PROC_ID) {
        return GDE_OPERATION_PENDING;
    }
    if (dev->pendingRequestInfo.vErrPtr) {
        return GDE_OPERATION_PENDING;
    }

    const addr_t deviceFuncVAddr = *(addr_t *)((addr_t)dev + vOperationPtrDeviceOffset);
    const enum k_proc_error error = k_proc_callback_invoke_void(operatorProcId,
                                                                (void (*)(void))deviceFuncVAddr);
    if (error != PE_NONE) {
        return GDRE_UNKNOWN;
    }
    
    dev->pendingRequestInfo.procId = requesterProcId;
    dev->pendingRequestInfo.vErrPtr = vErrPtr;
    dev->status = resultStatus;
    return GDRE_NONE;
}

enum gnwDriverError k_dev_init_async(const procId_t operatorProcId,
                                     const procId_t requesterProcId,
                                     enum gnwDriverError * const vErrPtr) {
    return deviceInvokeAsync(operatorProcId,
                             requesterProcId,
                             vErrPtr,
                             NEW,
                             INITIALIZING,
                             OFFSETOF(struct device, desc.driver.descriptor.init));
}

static bool validateReporter(const procId_t operatorProcId, const size_t deviceId) {
    if (!validateId(deviceId)) {
        OOPS("Unexpected device ID", false);
    }
    struct device * const devicePtr = &(devices[deviceId]);
    if (devicePtr->operator != operatorProcId) {
        OOPS("Unexpected operator", false);
    }

    return true;
}

void k_dev_init_report(const procId_t operatorProcId, const size_t deviceId, const bool success) {
    if (!validateReporter(operatorProcId, deviceId)) {
        OOPS("Reporter validation failed",);
        #warning TODO terminate operator process or crash if kernel
    }
    if (devices[deviceId].status != INITIALIZING) {
        OOPS("Unexpected device status",);
        #warning TODO terminate operator process or crash if kernel
    }

    devices[deviceId].status = INITIALIZED;
}

enum gnwDriverError k_dev_start(const size_t id) {
    if (!validateId(id)) {
        LOG("Device identifier invalid");
        return GDRE_UNKNOWN;
    }

    struct device *dev = &devices[id];

    if (dev->operator != KERNEL_PROC_ID) {
        OOPS("Unexpected operator process ID during device startup", GDRE_UNKNOWN);
    }
    if (dev->status != INITIALIZED) {
        OOPS("Trying to start uninitialized driver", GDRE_UNKNOWN);
    }

    dev->status = STARTING;
    
    void (*start)(void) = dev->desc.driver.descriptor.start;
    if (!start) {
        dev->status = STARTED;
    } else {
        start();
    }

    if (dev->status != STARTED) {
        LOG("Error: Driver startup failed");
        dev->status = FAILED;
        return GDRE_START_FAILED;
    }

    return GDRE_NONE;
}

enum gnwDriverError k_dev_start_async(const procId_t operatorProcId,
                                      const procId_t requesterProcId,
                                      enum gnwDriverError * const vErrPtr) {
    return deviceInvokeAsync(operatorProcId,
                             requesterProcId,
                             vErrPtr,
                             INITIALIZED,
                             STARTING,
                             OFFSETOF(struct device, desc.driver.descriptor.start));
}

void k_dev_start_report(const procId_t operatorProcId, const size_t deviceId, const bool success) {
    if (!validateReporter(operatorProcId, deviceId)) {
        OOPS("Reporter validation failed",);
        #warning TODO terminate operator process or crash if kernel
    }
    if (devices[deviceId].status != STARTING) {
        OOPS("Unexpected device status",);
        #warning TODO terminate operator process or crash if kernel
    }

    devices[deviceId].status = STARTED;
}

enum gnwDeviceError k_dev_getById(const size_t id, struct gnwDeviceUHADesc * const desc) {
    if (!validateInstalledId(id)) {
        OOPS("Device identifier invalid", GDE_UNKNOWN);
    }
    
    if (!desc) {
        OOPS("Device descriptor nullptr", GDE_UNKNOWN);
    }
    
    *desc = uhaGetDesc(id, devices[id].desc.type, devices[id].desc.api);
    return GDE_NONE;
}

enum gnwDeviceError k_dev_getByType(const enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc) {
    if (!desc) {
        OOPS("Device descriptor nullptr", GDE_UNKNOWN);
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
        OOPS("Device identifier invalid", GDE_UNKNOWN);
    }
    
    if (!uha) {
        OOPS("UHA descriptor nullptr", GDE_UNKNOWN);
    }

    *uha = devices[id].desc.api;
    return GDE_NONE;
}

enum gnwDeviceError k_dev_acquireHold(const procId_t processId, const size_t deviceId) {
    if (!validateInstalledId(deviceId)) {
        OOPS("Device identifier invalid", GDE_UNKNOWN);
    }

    if (!k_proc_idIsUser(processId)) {
        OOPS("Invalid process identifier", GDE_UNKNOWN);
    }

    if (k_proc_getInfo(processId).state != PS_RUNNING) {
        OOPS("Invalid process state", GDE_UNKNOWN);
    }

    if (devices[deviceId].holder != NONE_PROC_ID) {
        return GDE_ALREADY_HELD;
    }

    devices[deviceId].holder = processId;

    return GDE_NONE;
}

void k_dev_releaseHold(const procId_t processId, const size_t deviceId) {
    if (!validateInstalledId(deviceId)) {
        OOPS("Device identifier invalid",);
    }

    if (devices[deviceId].holder == processId) {
        devices[deviceId].holder = NONE_PROC_ID;
        devices[deviceId].listener = nullptr;
        devices[deviceId].decoder = nullptr;
    }
}

enum gnwDeviceError k_dev_writeMem(const procId_t processId, 
                                   const size_t deviceId,
                                   const ptr_t bufferPtr,
                                   const range_addr_t devMemRange) {
    if (!bufferPtr) {
        OOPS("Buffer cannot be nullptr", GDE_UNKNOWN);
    }

    const enum gnwDeviceError e = validateStartedDevice(processId, deviceId);
    if (e) {
        return e;
    }

    const size_t maxInputSizeBytes = devices[deviceId].desc.api.mem.desc.maxInputSizeBytes;
    if (!maxInputSizeBytes) {
        return GDE_INVALID_OPERATION;
    }

    if (!devMemRange.sizeBytes) {
        return GDE_INVALID_PARAMETER;
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
    routine->write(bufferPtr, devMemRange);

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

enum gnwDeviceError k_dev_listen(const procId_t processId, 
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

    devices[deviceId].listener = listener;
    devices[deviceId].decoder = decoder;
    return GDE_NONE;
}

enum gnwDeviceError k_dev_getParam(const size_t deviceId,
                                   const struct gnwDeviceParamDescriptor paramDescriptor,
                                   size_t * const resultPtr) {
    if (!resultPtr) {
        OOPS("Nullptr", GDE_UNKNOWN);
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
                                                            resultPtr)) {
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

PRIVATE enum gnwDeviceError validateEmitter(const size_t * const devIdPtr) {
    if (!devIdPtr) {
        OOPS("Unexpected null device ID pointer", GDE_UNKNOWN);
    }
    if (!validateInstalledId(*devIdPtr)) {
        OOPS("Unexpected serviced device ID", GDE_UNKNOWN);
    }
    if (devices[*devIdPtr].status != STARTED) {
        return GDE_INVALID_DEVICE_STATE;
    }

    return GDE_NONE;
}

PRIVATE enum gnwDeviceError validateListenerInvocation(const size_t deviceId) {
    struct device *dev = &devices[deviceId];
    if (!dev->listener) {
        return GDE_NOT_FOUND;
    }
    if (!dev->decoder) {
        OOPS("Unexpected null decoder", GDE_UNKNOWN);
    } 
    if (dev->holder == NONE_PROC_ID) {
        OOPS("Inconsistent holder listener state", GDE_UNKNOWN);
    }

    return GDE_NONE;
}

enum gnwDeviceError k_dev_emit(const procId_t procId, const struct gnwDeviceEvent * const eventPtr) {
    if (!eventPtr) {
        OOPS("Nullptr", GDE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)eventPtr, sizeof(struct gnwDeviceEvent))) {
        OOPS("Reserved zone access violation", GDE_UNKNOWN);
    }
    if (!eventPtr->data) {
        OOPS("Nullptr", GDE_UNKNOWN);
    }
    if (!eventPtr->dataSizeBytes) {
        OOPS("Unexpected event data size", GDE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)eventPtr->data, eventPtr->dataSizeBytes)) {
        OOPS("Reserved zone access violation", GDE_UNKNOWN);
    }
    enum gnwDeviceError err = validateEmitter(k_hal_servicedDevIdPtr);
    if (err) {
        return err;
    }
    err = validateListenerInvocation(*k_hal_servicedDevIdPtr);
    if (err != GDE_NONE) {
        return err;
    }

    struct device *dev = &devices[*k_hal_servicedDevIdPtr];
    enum k_proc_error callbackErr = k_proc_callback_invoke_ptr(dev->holder, 
                                                               (gnwEventListener_ptr)dev->listener,
                                                               (ptr_t)eventPtr,
                                                               sizeof(struct gnwDeviceEvent),
                                                               GNW_DEVICEEVENT_ENCODEDSIZE + eventPtr->dataSizeBytes,
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

        #warning TODO remove pendingRequestInfo from all the devices containing given requester procId
        #warning TODO remove operator and device too

    }
}
