//
//  ops.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.02.2025.
//

#ifndef OPS_H
#define OPS_H

#include "data.h"

bool inputEnable();
bool reset();
bool setDefaults();
bool enablePacketStreaming();
bool enableInterrupt();

bool readMovementData(struct movementDataPacket * const data);

#endif // OPS_H
