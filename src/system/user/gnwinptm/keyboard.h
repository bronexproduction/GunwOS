//
//  keyboard.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <gunwdevtypes.h>

bool keyboard_init();

enum gnwDeviceError keyboard_attach(const procId_t procId);

#endif // KEYBOARD_H
