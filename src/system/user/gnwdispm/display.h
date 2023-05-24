//
//  display.h
//  GunwOS
//
//  Created by Artur Danielewski on 05.05.2023.
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include <types.h>
#include <gunwdisplay.h>

bool display_init();

enum gnwDeviceError display_getDisplay(const enum gnwDisplayType type,
                                       struct gnwDisplayDescriptor * const displayDescriptor);
enum gnwDeviceError display_attachToDisplay(const enum gnwDisplayType type, 
                                            const struct gnwDisplayDescriptor * const displayDescriptor);
enum gnwDeviceError display_pushFrame(const size_t displayId,
                                      const ptr_t buffer,
                                      const range_addr_t inputRange);
#endif // DISPLAY_H
