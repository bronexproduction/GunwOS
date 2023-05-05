//
//  external.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#ifndef EXTERNAL_H
#define EXTERNAL_H

#include "../opmode.h"
#include <types.h>

void externalDisable();
void externalSetMode(const enum modeOfOperation mode, uint_8 * const regContext);
void externalEnable(uint_8 regContext);

#endif // EXTERNAL_H
