//
//  attribute.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "../opmode.h"
#include <types.h>

void attributeSetMode(const enum modeOfOperation mode);
void attributeEnable();

#endif // ATTRIBUTE_H
