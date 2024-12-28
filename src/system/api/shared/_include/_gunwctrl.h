//  _gunwctrl.h
//  GunwOS
//
//  Created by Artur Danielewski on 12.05.2023.
//

#ifndef GUNWOS__GUNWCTRL_H
#define GUNWOS__GUNWCTRL_H

#include <types.h>
#include "../include/gunwctrltypes.h"

struct gnwCtrlStartDescriptor {
    const data_t pathData;
    enum gnwCtrlError * errorPtr;
};

#endif // GUNWOS__GUNWCTRL_H
