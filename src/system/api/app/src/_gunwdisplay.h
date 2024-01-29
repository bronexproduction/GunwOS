//
//  _gunwdisplay.h
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2023.
//

#ifndef GUNWOS__GUNWDISPLAY_H
#define GUNWOS__GUNWDISPLAY_H

#include "../include/gunwdisplay.h"

enum gnwDeviceError getDisplay(enum gnwDisplayType type, 
                               struct gnwDisplayDescriptor * const displayDescriptor);
                               
enum gnwDeviceError attachToDisplay(const enum gnwDisplayType type, 
                                    const struct gnwDisplayDescriptor * const displayDescriptor);

enum gnwDeviceError pushFrame(const size_t displayId,
                              const ptr_t buffer,
                              const size_t bufferSizeBytes);
                              
#endif // GUNWOS__GUNWDISPLAY_H
