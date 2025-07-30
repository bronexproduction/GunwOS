//
//  mouse.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.07.2025.
//

#ifndef MOUSE_H
#define MOUSE_H

#include "data.h"

void init_mouse();
void start_mouse();
void emitMouseEvents(const struct eventDataPacket);

#endif // MOUSE_H
