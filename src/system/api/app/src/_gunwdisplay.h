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
#include <mem.h>

enum DisplayType {
    TEXT,
    GRAPHICS
};

void fillDisplayDescriptor(const size_t identifier,
                           const enum gnwDeviceUHA_display_format format,
                           const point_t dimensions,
                           struct gnwDisplayDescriptor * const desc);

enum gnwDeviceError getDisplay(enum DisplayType type, 
                               struct gnwDisplayDescriptor * const displayDescriptor);
                               
enum gnwDeviceError attachToDisplay(const enum DisplayType type, 
                                    const struct gnwDisplayDescriptor * const displayDescriptor);

enum gnwDeviceError pushFrame(const size_t deviceId,
                              const ptr_t buffer,
                              const size_t bufferSizeBytes);
                              
#endif // GUNWOS__GUNWDISPLAY_H
