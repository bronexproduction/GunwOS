//
//  dev.h
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2023.
//

#ifndef DEV_H
#define DEV_H

#include <gunwdev.h>
#include <driver/gunwdrv.h>
#include <src/_gunwrlp.h>

enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const descriptor);
enum gnwDriverError k_dev_start(size_t id);

enum gnwDeviceError k_dev_getById(const size_t id, struct gnwDeviceUHADesc * const desc);
enum gnwDeviceError k_dev_getByType(const enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc);

enum gnwDeviceError k_dev_getUHAForId(const size_t id, struct gnwDeviceUHA * const uha);

enum gnwDeviceError k_dev_acquireHold(const procId_t processId, const size_t deviceId);
void k_dev_releaseHold(const procId_t processId, const size_t deviceId);

enum gnwDeviceError k_dev_writeMem(const procId_t processId, 
                                   const size_t deviceId,
                                   const void * const buffer);
enum gnwDeviceError k_dev_writeChar(const procId_t processId, 
                                    const size_t deviceId,
                                    const char character);

enum gnwDeviceError k_dev_listen(const procId_t processId, 
                                 const size_t deviceId, 
                                 const union gnwEventListener listener,
                                 struct gnwRunLoop * const runLoopPtr);

enum gnwDeviceError k_dev_emit_void(const int_32 type);
enum gnwDeviceError k_dev_emit_u8(const int_32 type,
                                  const int_8 data);

void k_dev_procCleanup(const procId_t procId);

#endif // DEV_H
