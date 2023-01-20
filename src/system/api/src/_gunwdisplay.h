//
//  _gunwdisplay.h
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2023.
//

#ifndef GUNWOS__GUNWDISPLAY_H
#define GUNWOS__GUNWDISPLAY_H

#include <gunwdisplay.h>
#include <gunwdev.h>
#include <gunwfug.h>
#include <stdgunw/mem.h>
#include <scl_def.h>

#define CHECKPTR(PTR) { if (!PTR) { fug(NULLPTR); __builtin_unreachable(); } }

enum displayType {
    TEXT,
    GRAPHICS
};

void fillDisplayDescriptorWithUHA(const struct gnwDeviceUHADesc * const uha,
                                  struct gnwDisplayDescriptor * const desc);

enum gnwDeviceError getDisplay(enum displayType type, 
                               struct gnwDisplayDescriptor * const displayDescriptor);
                               
enum gnwDeviceError attachToDisplay(const enum displayType type, 
                                    const uint_32 displayId, 
                                    struct gnwDeviceUHADesc * const uha);

enum gnwDeviceError pushFrame(const size_t deviceId,
                              const void * const buffer);
                              
#endif // GUNWOS__GUNWDISPLAY_H
