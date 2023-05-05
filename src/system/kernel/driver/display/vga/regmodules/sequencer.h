//
//  sequencer.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "../opmode.h"
#include <types.h>

void sequencerDisable(uint_8 * const regContextCMR);
void sequencerSetMode(const enum modeOfOperation mode, uint_8 * const regContextCMR);
void sequencerEnable(uint_8 regContextCMR);

#endif // SEQUENCER_H
