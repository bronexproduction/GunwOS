//  _gunwctrl.h
//  GunwOS
//
//  Created by Artur Danielewski on 12.05.2023.
//

#ifndef GUNWOS__GUNWCTRL_H
#define GUNWOS__GUNWCTRL_H

#include "../include/gunwctrl.h"

struct gnwCtrlStartDescriptor {
    const char * pathPtr;
    size_t pathLen;
    enum gnwCtrlError * errorPtr;
};

#endif // GUNWOS__GUNWCTRL_H
