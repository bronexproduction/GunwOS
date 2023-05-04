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

void crtDisable(const enum modeOfOperation mode, const bool memOver64K, uint_8 * const regContextMCR, uint_8 * const regContextVRE);
void crtSetMode(const enum modeOfOperation mode, const bool memOver64K, uint_8 * const regContextMCR, uint_8 * const regContextVRE);
void crtEnable(const enum modeOfOperation mode, const bool memOver64K, uint_8 regContextMCR, uint_8 regContextVRE);

#endif // OPMODE_H
