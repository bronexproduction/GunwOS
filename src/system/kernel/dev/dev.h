//
//  dev.h
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2023.
//

#ifndef DEV_H
#define DEV_H

#include <gunwdev.h>
#include <gunwdrv.h>
#include <_gunwrlp.h>

void k_dev_init();

enum gnwDriverError k_dev_install(const struct gnwDeviceDescriptor * const descriptorPtr,
                                  size_t * const deviceIdPtr);
enum gnwDriverError k_dev_install_async(const struct gnwDeviceDescriptor * const descriptorPtr,
                                        const procId_t operatorProcId);

enum gnwDriverError k_dev_start(size_t id);

enum gnwDeviceError k_dev_getById(const size_t id, struct gnwDeviceUHADesc * const desc);
enum gnwDeviceError k_dev_getByType(const enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc);

enum gnwDeviceError k_dev_getUHAForId(const size_t id, struct gnwDeviceUHA * const uha);

enum gnwDeviceError k_dev_acquireHold(const procId_t processId, const size_t deviceId);
void k_dev_releaseHold(const procId_t processId, const size_t deviceId);

enum gnwDeviceError k_dev_writeMem(const procId_t processId, 
                                   const size_t deviceId,
                                   const ptr_t bufferPtr,
                                   const range_addr_t devMemRange);
enum gnwDeviceError k_dev_writeChar(const procId_t processId, 
                                    const size_t deviceId,
                                    const char character);

enum gnwDeviceError k_dev_listen(const procId_t processId, 
                                 const size_t deviceId, 
                                 const gnwDeviceEventListener listener,
                                 const gnwDeviceEventDecoder decoder);

enum gnwDeviceError k_dev_getParam(const size_t deviceId,
                                   const struct gnwDeviceParamDescriptor paramDescriptor,
                                   size_t * const resultPtr);
enum gnwDeviceError k_dev_setParam(const procId_t procId,
                                   const size_t deviceId,
                                   const struct gnwDeviceParamDescriptor paramDescriptor,
                                   const size_t value);

enum gnwDeviceError k_dev_emit(const procId_t procId, const struct gnwDeviceEvent * const eventPtr);

void k_dev_procCleanup(const procId_t procId);

#endif // DEV_H
