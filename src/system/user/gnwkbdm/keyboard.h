//
//  keyboard.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <types.h>
#include <gunwdisplay.h>

bool display_init();

enum gnwDeviceError display_getDisplay(const enum gnwDisplayType type,
                                       struct gnwDisplayDescriptor * const displayDescriptor);
enum gnwDeviceError display_attachToDisplay(const procId_t procId,
                                            const enum gnwDisplayType type, 
                                            const struct gnwDisplayDescriptor * const displayDescriptor);
enum gnwDeviceError display_pushFrame(const procId_t procId,
                                      const size_t displayId,
                                      const ptr_t buffer,
                                      const range_addr_t inputRange);
#endif // KEYBOARD_H
