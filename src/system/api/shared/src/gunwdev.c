//
//  gunwdev.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include "../_include/_gunwdev.h"
#include "../_include/_gunwctrl.h"
#include "../include/gunwdev.h"
#include "../include/gunwfug.h"
#include "../_include/scl_user.h"
#include <mem.h>
#include <string.h>

#ifndef _GUNWAPI_KERNEL

enum gnwDeviceError devGetById(const size_t deviceId,
                               struct gnwDeviceUHADesc * const desc) {
    CHECKPTR(desc);

    SYSCALL_USER_CALL(DEV_GET_BY_ID, deviceId, desc, 0, 0);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devGetByType(const enum gnwDeviceType type,
                                 struct gnwDeviceUHADesc * const desc) {
    CHECKPTR(desc);

    SYSCALL_USER_CALL(DEV_GET_BY_TYPE, type, desc, 0, 0);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devGetParam(const size_t deviceId,
                                const size_t param,
                                const size_t subParam,
                                const size_t paramIndex,
                                size_t * const result) {

    CHECKPTR(result);

    const struct gnwDeviceGetParamQuery query = {
        param,
        subParam,
        paramIndex
    };

    SYSCALL_USER_CALL(DEV_GET_PARAM, deviceId, &query, result, 0);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devSetParam(const size_t deviceId,
                                const size_t param,
                                const size_t subParam,
                                const size_t paramIndex,
                                const size_t value) {

    const struct gnwDeviceSetParamQuery query = {
        param,
        subParam,
        paramIndex,
        value
    };

    SYSCALL_USER_CALL(DEV_SET_PARAM, deviceId, &query, 0, 0);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devAcquire(const uint_32 identifier) {
    SYSCALL_USER_CALL(DEV_ACQUIRE, identifier, 0, 0, 0);

    return SYSCALL_RESULT;
}

void devRelease(const uint_32 identifier) {
    SYSCALL_USER_CALL(DEV_RELEASE, identifier, 0, 0, 0);
}

enum gnwDeviceError devCharWrite(const uint_32 deviceId, 
                                 const char character) {
    SYSCALL_USER_CALL(DEV_CHAR_WRITE, deviceId, character, 0, 0);
    
    return SYSCALL_RESULT;
}

enum gnwDeviceError devMemWrite(const size_t identifier,
                                const ptr_t buffer,
                                const range_addr_t devInputBufferRange) {
    CHECKPTR(buffer);

    const struct gnwDeviceMemWriteQuery query = {
        /* buffer */ buffer,
        /* inputBufferRange */ devInputBufferRange
    };

    SYSCALL_USER_CALL(DEV_MEM_WRITE, identifier, &query, 0, 0);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devListen(const size_t identifier,
                              const gnwDeviceEventListener listener) {
    CHECKPTR(listener);

    SYSCALL_USER_CALL(DEV_LISTEN, identifier, listener, gnwDeviceEvent_decode, 0);

    return SYSCALL_RESULT;
}

extern void log(const char * const msg);

void devInstall(const char * const path,
                enum gnwCtrlError * const ctrlError,
                enum gnwDriverError * const installError) {

    CHECKPTR(path);
    CHECKPTR(ctrlError);
    CHECKPTR(installError);

    *(ctrlError) = GCE_NONE;
    *(installError) = GDRE_NONE;

    /* Load device driver */

    procId_t deviceOperatorProcId;
    struct gnwCtrlStartDescriptor desc = { { (byte_t *)path, strlen(path) }, GET_DRIVER, &deviceOperatorProcId }; 

    { SYSCALL_USER_CALL(START, &desc, 0, 0, 0); }

    if (deviceOperatorProcId < 0) {
        *(ctrlError) = (enum gnwCtrlError)deviceOperatorProcId;
        return;
    }
    
    size_t deviceCount; {
        SYSCALL_USER_CALL(DEV_GET_COUNT_FOR_OPERATOR, deviceOperatorProcId, 0, 0, 0);
        deviceCount = SYSCALL_RESULT;
    }
    if (!deviceCount) {
        *(ctrlError) = (enum gnwCtrlError)GCE_UNKNOWN;
        return;
    }

    size_t deviceIdentifiers[deviceCount]; {
        enum gnwDeviceError error;
        SYSCALL_USER_CALL(DEV_GET_IDS_FOR_OPERATOR, deviceOperatorProcId, deviceIdentifiers, &error, 0);

        if (error != GDE_NONE) {
            *(ctrlError) = (enum gnwCtrlError)GCE_UNKNOWN;
            return;
        }
    }

    /* Initialize devices */
    
    for (size_t deviceIdIndex = 0; deviceIdIndex < deviceCount; ++deviceIdIndex) {  
        { SYSCALL_USER_CALL(DEV_INIT, deviceIdentifiers[deviceIdIndex], installError, 0, 0); }

        if (*(installError) != GDRE_NONE) {
            return;
        }
    }
    
    /* Start devices */

    for (size_t deviceIdIndex = 0; deviceIdIndex < deviceCount; ++deviceIdIndex) {  
        { SYSCALL_USER_CALL(DEV_START, deviceIdentifiers[deviceIdIndex], installError, 0, 0); }

        if (*(installError) != GDRE_NONE) {
            return;
        }
    }
}

void gnwDeviceEvent_decode(const ptr_t dataPtr, struct gnwDeviceEvent * const eventPtr) {
    eventPtr->type = *(int_32 *)dataPtr;
    eventPtr->dataSizeBytes = *(size_t *)(dataPtr + sizeof(int_32));
    eventPtr->data = dataPtr + sizeof(int_32) + sizeof(size_t);
}

#else

void gnwDeviceEvent_encode(const struct gnwDeviceEvent * const eventPtr, ptr_t dataPtr) {
    *(int_32 *)dataPtr = eventPtr->type;
    *(size_t *)(dataPtr + sizeof(int_32)) = eventPtr->dataSizeBytes;
    memcopy(eventPtr->data, dataPtr + sizeof(int_32) + sizeof(size_t), eventPtr->dataSizeBytes);
}

#endif // _GUNWAPI_KERNEL
