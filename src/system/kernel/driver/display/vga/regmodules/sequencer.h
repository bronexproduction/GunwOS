//
//  sequencer.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//

#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "../opmode.h"
#include <types.h>

void sequencerDisable(const enum modeOfOperation mode, const bool memOver64K);
void sequencerSetMode(const enum modeOfOperation mode, const bool memOver64K);
void sequencerEnable(const enum modeOfOperation mode, const bool memOver64K);

#endif // SEQUENCER_H
