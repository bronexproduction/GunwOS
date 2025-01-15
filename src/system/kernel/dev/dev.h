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

enum gnwDriverError k_dev_init_async(const procId_t operatorProcId, const procId_t requesterProcId, enum gnwDriverError * const vErrorPtr);
void k_dev_init_report(const procId_t operatorProcId, const size_t deviceId, const bool success);

enum gnwDriverError k_dev_start(const size_t id);
enum gnwDriverError k_dev_start_async(const procId_t operatorProcId, const procId_t requesterProcId, enum gnwDriverError * const vErrorPtr);
void k_dev_start_report(const procId_t operatorProcId, const size_t deviceId, const bool success);

enum gnwDeviceError k_dev_getById(const size_t id, struct gnwDeviceUHADesc * const desc);
enum gnwDeviceError k_dev_getByOperator(const procId_t operator, struct gnwDeviceUHADesc * const desc);
enum gnwDeviceError k_dev_getByType(const enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc);

enum gnwDeviceError k_dev_getUHAForId(const size_t id, struct gnwDeviceUHA * const uha);

bool k_dev_mmioRangeAllowed(const procId_t procId, const addr_t physMemStart, const size_t sizeBytes);

enum gnwDeviceError k_dev_acquireHold(const procId_t processId, const size_t deviceId);
void k_dev_releaseHold(const procId_t processId, const size_t deviceId);

void k_dev_writeMem(const procId_t procId, 
                    const size_t deviceId,
                    const struct gnwDeviceMemWriteQuery query,
                    enum gnwDeviceError * const vErrorPtr);
enum gnwDeviceError k_dev_writeChar(const procId_t processId, 
                                    const size_t deviceId,
                                    const char character);

enum gnwDeviceError k_dev_listen(const procId_t processId, 
                                 const size_t deviceId, 
                                 const gnwDeviceEventListener listener,
                                 const gnwDeviceEventDecoder decoder);

void k_dev_getParam(const procId_t procId,
                    const size_t deviceId,
                    const struct gnwDeviceGetParamQuery query,
                    size_t * const vReplyPtr,
                    enum gnwDeviceError * const vErrorPtr);
void k_dev_getParam_reply(const procId_t operatorProcId,
                          const size_t deviceId,
                          const bool success,
                          const size_t result);
void k_dev_setParam(const procId_t procId,
                    const size_t deviceId,
                    const struct gnwDeviceSetParamQuery query,
                    enum gnwDeviceError * const vErrorPtr);
void k_dev_setParam_reply(const procId_t operatorProcId,
                                         const size_t deviceId,
                                         const bool success);

enum gnwDeviceError k_dev_emit(const procId_t procId, const struct gnwDeviceEvent * const eventPtr);

void k_dev_procCleanup(const procId_t procId);

#endif // DEV_H
