//
//  crt.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//

#ifndef CRT_H
#define CRT_H

#include "../opmode.h"
#include <types.h>

void crtSetMode(const enum modeOfOperation mode, const bool memOver64K);

#endif // OPMODE_H
