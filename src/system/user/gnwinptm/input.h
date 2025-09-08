//
//  input.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#ifndef INPUT_H
#define INPUT_H

#include <gunwdevtypes.h>

bool input_init();

enum gnwDeviceError input_attach(const procId_t procId);

#endif // INPUT_H
