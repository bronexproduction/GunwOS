//
//  gunwdev.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef GUNWOS_GUNWDEV_H
#define GUNWOS_GUNWDEV_H

#include "gunwuhadesc.h"
#include "gunwevent.h"
#include "gunwctrltypes.h"

struct gnwDeviceEvent {
    /*
        Event type (device specific)
    */
    int_32 type;

    /*
        Event data pointer
    */
    ptr_t data;

    /*
        Event data size in bytes
    */
    size_t dataSizeBytes;
};

typedef __attribute__((cdecl)) void (*gnwDeviceEventListener)(const struct gnwDeviceEvent * const);

typedef void (*gnwDeviceEventDecoder)(const ptr_t, struct gnwDeviceEvent * const);

#ifndef _GUNWAPI_KERNEL

void gnwDeviceEvent_decode(const ptr_t, struct gnwDeviceEvent * const);

/*
    Requests device information for given id

    Params:
        * id - id of the device
        * desc - address of the result description (see gunwdrv.h)
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
extern enum gnwDeviceError devGetById(const size_t deviceId,
                                      struct gnwDeviceUHADesc * const desc);

/*
    Requests device information for given type

    Params:
        * type - type of the device (see gunwdrv.h)
        * desc - address of the result description (see gunwdrv.h)
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
extern enum gnwDeviceError devGetByType(const enum gnwDeviceType type, 
                                        struct gnwDeviceUHADesc * const desc);

/*
    Requests parameter value for given device ID, if any

    Params:
        * id - id of the device
        * paramDescriptor - device-specific parameter info (see UHA for a specific device)
        * result - pointer to the result buffer
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
extern enum gnwDeviceError devGetParam(const size_t deviceId, 
                                       const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                       size_t * const result);

/*
    Sets parameter value for given device ID

    Params:
        * id - id of the device
        * paramDescriptor - device-specific parameter info (see UHA for a specific device)
        * value - value to be set for parameter
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)

    Note: In order to set device parameters
          the process has to be exclusive holder of it
*/
extern enum gnwDeviceError devSetParam(const size_t deviceId,
                                       const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                       const size_t value);

/*
    Attempts to take hold of the device

    Params:
        * identifier - device identifier

    Note: In order to use the device
          the process has to be exclusive holder of it

    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
extern enum gnwDeviceError devAcquire(const uint_32 identifier);

/*
    Releases the hold taken on the device (if any)

    Params:
        * identifier - device identifier
*/
extern void devRelease(const uint_32 identifier);

/*
    Write character to character output device

    Parameters:
    * character - Character to be written
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
extern enum gnwDeviceError devCharWrite(const uint_32 deviceId, 
                                        const char character);

/*
    Write to the device

    Params:
        * identifier - device identifier
        * buffer - data buffer pointer
        * devInputBufferRange - Range of bytes in relation to accepted input memory range
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
extern enum gnwDeviceError devMemWrite(const size_t identifier,
                                       const ptr_t buffer,
                                       const range_addr_t * const devInputBufferRange);

/*
    Register a listener to device events

    Params:
        * identifier - device identifier
        * listener - event listener
*/
extern enum gnwDeviceError devListen(const size_t identifier,
                                     const gnwDeviceEventListener listener);

/*
    Installs device driver

    Params:
    * path - path to the driver file
    * ctrlError - result error pointer (errors related to file operations)
    * installError - result error pointer (errors related to device operations)
*/
extern void devInstall(const char * const path,
                       enum gnwCtrlError * ctrlError,
                       enum gnwDeviceInstallError * const installError);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWDEV_H
