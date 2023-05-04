//
//  attribute.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "../opmode.h"
#include <types.h>

void attributeSetMode(const enum modeOfOperation mode, const bool memOver64K);
void attributeEnable(const enum modeOfOperation mode);

#endif // ATTRIBUTE_H
