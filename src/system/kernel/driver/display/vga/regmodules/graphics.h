//
//  graphics.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../opmode.h"
#include <types.h>

void graphicsSetMode(const enum modeOfOperation mode, const bool memOver64K);

#endif // GRAPHICS_H
