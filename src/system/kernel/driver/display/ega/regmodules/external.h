//
//  external.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//

#ifndef EXTERNAL_H
#define EXTERNAL_H

#include "../opmode.h"

void externalDisable(const enum modeOfOperation mode);
void externalSetMode(const enum modeOfOperation mode);
void externalEnable(const enum modeOfOperation mode);

#endif // EXTERNAL_H
