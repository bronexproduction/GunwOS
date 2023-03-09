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

enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const descriptor);
enum gnwDriverError k_dev_start(size_t id);

enum gnwDeviceError k_dev_acquireHold(procId_t processId, size_t deviceId);
void k_dev_releaseHold(size_t, size_t);

enum gnwDeviceError k_dev_writeMem(const procId_t processId, 
                                   const size_t deviceId,
                                   const void * const buffer);
enum gnwDeviceError k_dev_writeChar(const procId_t processId, 
                                    const size_t deviceId,
                                    const char character);

enum gnwDeviceError k_dev_listen(const size_t processId, 
                                 const size_t deviceId, 
                                 const struct gnwDeviceEventListener * const listener);

enum gnwDeviceError k_dev_emit_void(const int_32 type);
enum gnwDeviceError k_dev_emit_u8(const int_32 type,
                                  const uint_8 data);

#endif // DEV_H
